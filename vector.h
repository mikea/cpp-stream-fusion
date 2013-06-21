#ifndef __VECTOR__H__
#define __VECTOR__H__

#include <string>
#include <vector>

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>
#include <boost/concept/requires.hpp>

// Defines Stream concept.
template<class X>
struct IsStream {
 public:
  // A stream is something
  BOOST_CONCEPT_USAGE(IsStream) {
    // With size() method
    size_t size = x.size();
    // begin() method returning ::Iterator type
    typename X::Iterator i = x.begin();
    // ++ iterator advancing operator
    ++i;
    // and end() checker
    bool atEnd = x.end(i);
    // and subscript operator.
    double d = x[i];
  }

 private:
  X x;
};

template<class X>
struct IsOpObject {
 public:
  BOOST_CONCEPT_USAGE(IsOpObject) {
    double d1, d2;
    double r = x(d1, d2);
  }

 private:
  X x;
};

template<typename Op, class Stream1, class Stream2>
struct ZippedStream {
  BOOST_CONCEPT_ASSERT((IsOpObject<Op>));
  BOOST_CONCEPT_ASSERT((IsStream<Stream1>));
  BOOST_CONCEPT_ASSERT((IsStream<Stream2>));

  typedef typename Stream1::Iterator Iterator1;
  typedef typename Stream2::Iterator Iterator2;

  ZippedStream(const Stream1& s1, const Stream2& s2) : op_(), s1_(s1), s2_(s2) { }

  const Op op_;
  const Stream1& s1_;
  const Stream2& s2_;

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

  double operator[](const Iterator& it) const {
    return op_(s1_[it.it1_], s2_[it.it2_]);
  }
};

template<typename Op, class Stream1, class Stream2>
BOOST_CONCEPT_REQUIRES(
    ((IsStream<Stream1>))
    ((IsStream<Stream2>)),
(ZippedStream<Op, Stream1, Stream2>)) ZipWith(const Stream1& s1, const Stream2& s2) {
  return ZippedStream<Op, Stream1, Stream2>(s1, s2);
}

template<typename Op, class Stream>
BOOST_CONCEPT_REQUIRES(
    ((IsOpObject<Op>))
    ((IsStream<Stream>)),
(double)) Fold(const Stream& stream, double init) {
  Op op;
  double result = init;
  for (auto it = stream.begin(); !stream.end(it); ++it) {
    result = op(result, stream[it]);
  }
  return result;
}

//-----------------------

struct Plus : public std::binary_function<double, double, double> {
  double operator()(double d1, double d2) const { return d1 + d2; }
};

struct Mul : public std::binary_function<double, double, double> {
  double operator()(double d1, double d2) const { return d1 * d2; }
};

//-----------------------

class Vector {
 public:
  explicit Vector(size_t size) {
    data_.resize(size);
  }

  typedef int Iterator;
  int begin() const { return 0; }
  bool end(int i) const { return i >= size(); }

  size_t size() const { return data_.size(); }

  double operator[](size_t i) const {
    return data_[i];
  }

  double& operator[](size_t i) {
    return data_[i];
  }

  double length() {
    return Fold<Plus>(ZipWith<Mul>(*this, *this), 0);
  }

  std::vector<double> data_;
};

template<class S>
class VectorFromStream {
 public:
  BOOST_CONCEPT_ASSERT((IsStream<S>));

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

  typedef typename Stream::Iterator Iterator;
  Iterator begin() const { return stream_.begin(); }
  bool end(const Iterator& i) const { return stream_.end(i); }
  double operator[](const Iterator& i) const { return stream_[i]; }
  size_t size() const { return stream_.size(); }

  double length() {
    return Fold<Plus>(ZipWith<Mul>(*this, *this), 0);
  }

  const Stream& stream_;
};

template<class Stream>
struct FromStream {
  BOOST_CONCEPT_ASSERT((IsStream<Stream>));

  typedef VectorFromStream<Stream> Result;

  Result operator()(const Stream& s) {
    return VectorFromStream<Stream>(s);
  }
};

template<typename Vector1, typename Vector2>
typename FromStream<ZippedStream<Plus, Vector1, Vector2> >::Result
operator+(const Vector1& v1, const Vector2& v2) {
  return FromStream<ZippedStream<Plus, Vector1, Vector2> >()(ZipWith<Plus>(v1, v2));
}

#endif
