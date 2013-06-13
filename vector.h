#ifndef __VECTOR__H__
#define __VECTOR__H__

#include <string>
#include <vector>

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
  explicit Vector(size_t size) {
    data_.resize(size);
  }

  struct Stream {
    typedef int Iterator;

    Stream(const Vector& vector) : vector_(vector) { }
    size_t size() const { return vector_.size(); }
    int begin() const { return 0; }
    bool end(int i) const { return i >= size(); }
    double get(int i) const { return vector_[i]; }

    const Vector& vector_;
  };

  Stream ToStream() const {
    return Stream(*this);
  }

  size_t size() const {
    return data_.size();
  }

  double operator[](size_t i) const {
    return data_[i];
  }

  double& operator[](size_t i) {
    return data_[i];
  }

  double length() {
    return Fold<Plus>(ZipWith<Mul>(ToStream(), ToStream()), 0);
  }

  std::vector<double> data_;
};

template<class S>
class VectorFromStream {
 public:
  typedef S Stream;
  VectorFromStream(const Stream& stream) : stream_(stream) { }

  operator Vector() const {
    printf("VectorFromStream->Vector\n");
    size_t size = stream_.size();
    Vector result(size);

    int i = 0;
    for (auto it = stream_.begin(); !stream_.end(it); ++it, ++i) {
      result[i] = stream_.get(it);
    }

    return result;
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

#endif
