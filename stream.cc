#include <string.h>
#include <stdio.h>
#include <assert.h>


template<typename Op, class Stream1, class Stream2>
struct ZippedStream {
	typedef typename Stream1::Iterator Iterator1;
	typedef typename Stream2::Iterator Iterator2;

	ZippedStream(const Stream1& s1, const Stream2& s2) : op_(), s1_(s1), s2_(s2) { }

	const Op op_;
	const Stream1 s1_;
	const Stream2 s2_;

	size_t size() const {
		size_t size1 = s1_.size();
		size_t size2 = s2_.size();
		if (size1 < size2) {
			return size1;
		} else {
			return size2;
		}
	}

	struct Iterator {
		Iterator(Iterator1 it1, Iterator2 it2) : it1_(it1), it2_(it2) { }
		Iterator1 it1_;
		Iterator2 it2_;

		void operator++() {
			++it1_;
			++it2_;
		}
	};

	Iterator begin() const { return Iterator(s1_.begin(), s2_.begin()); }
	bool end(const Iterator& it) const {
		return s1_.end(it.it1_) || s2_.end(it.it2_);
	}

	double get(const Iterator& it) const {
		return op_(s1_.get(it.it1_), s2_.get(it.it2_));
	}
};

template<typename Op, class Stream1, class Stream2>
ZippedStream<Op, Stream1, Stream2> ZipWith(const Stream1& s1, const Stream2& s2) {
	return ZippedStream<Op, Stream1, Stream2>(s1, s2);
}

template<typename Op, class Stream>
double Fold(const Stream& stream, double init) {
	Op op;
	double result = init;
	for (auto it = stream.begin(); !stream.end(it); ++it) {
		result = op(result, stream.get(it));
	}
	return result;
}

//-----------------------

struct Plus {
	double operator()(double d1, double d2) const { return d1 + d2; }
};

struct Mul {
	double operator()(double d1, double d2) const { return d1 * d2; }
};

//-----------------------

class Vector {
 public:
 	Vector(size_t size, double* const data) : size_(size), data_(data) { }
 	explicit Vector(size_t size) : size_(size), data_(new double[size]) {
 		memset(data_, 0, sizeof(double) * size);
 	}
 	// ~Vector() { delete[] data_; }

 	struct Stream {
 		typedef int Iterator;

 		Stream(size_t size, double* const data) : size_(size), data_(data) { }
 		size_t size() const { return size_; }
 		int begin() const { return 0; }
 		bool end(int i) const { return i >= size_; }
 		double get(int i) const { return data_[i]; }

	 	const size_t size_;
	 	double* const data_;
 	};

 	Stream ToStream() const {
 		return Stream(size_, data_);
 	}

 	double length() {
 		return Fold<Plus>(ZipWith<Mul>(ToStream(), ToStream()), 0);
 	}

 	const size_t size_;
 	double* const data_;
};

template<class S>
class VectorFromStream {
 public:
 	typedef S Stream;
 	VectorFromStream(const Stream& stream) : stream_(stream) { }

 	operator Vector() const {
		printf("VectorFromStream->Vector\n");
 		size_t size = stream_.size();
 		double* data = new double[size];

 		int i = 0;
 		for (auto it = stream_.begin(); !stream_.end(it); ++it, ++i) {
 			data[i] = stream_.get(it);
 		}

 		return Vector(size, data);
 	}

 	Stream ToStream() const { return stream_; }

 	double length() {
 		return Fold<Plus>(ZipWith<Mul>(stream_, stream_), 0);
 	}

 	const Stream& stream_;
};

template<class Stream>
struct FromStream {
	typedef VectorFromStream<Stream> Result;

	Result operator()(const Stream& s) {
		return VectorFromStream<Stream>(s);
	}
};

template<typename Vector1, typename Vector2>
typename FromStream<ZippedStream<Plus, typename Vector1::Stream, typename Vector2::Stream> >::Result
operator+(const Vector1& v1, const Vector2& v2) {
 		return FromStream<ZippedStream<Plus, typename Vector1::Stream, typename Vector2::Stream> >()(ZipWith<Plus>(v1.ToStream(), v2.ToStream()));
}

int main(int argc, const char* argv[]) {
	Vector a(1024), b(1024), c(1024);
	auto d = a + b + c;
	printf("Length: %f\n", d.length());
}
