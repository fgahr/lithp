#ifndef _LITHP_UTIL_STREAM_H_
#define _LITHP_UTIL_STREAM_H_

#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <vector>

namespace lithp::util {

namespace internal {
template <typename T> class __Stream {
public:
  virtual bool has_more() = 0;
  virtual T get() = 0;
  virtual ~__Stream<T>() = default;
  virtual T *peek() = 0;
};

template <typename T> class EmptyStream : public __Stream<T> {
public:
  EmptyStream<T>() {}
  virtual bool has_more() override { return false; }
  virtual T get() override {
    throw std::logic_error{"requested an element from an empty stream"};
  }
  virtual ~EmptyStream<T>() = default;
  virtual T *peek() override { return nullptr; }
};

template <typename T> class FixedStream : public __Stream<T> {
public:
  // FixedStream<T>(std::initializer_list<T> elems) : elems{elems} {}
  FixedStream<T>(std::vector<T> elems) : elems{std::move(elems)} {}
  virtual bool has_more() override { return pos < elems.size(); }
  virtual T get() override {
    if (!has_more()) {
      throw std::logic_error{"requested an element from drained stream"};
    }
    return elems.at(pos++);
  }
  virtual ~FixedStream<T>() = default;
  virtual T *peek() override {
    if (!has_more()) {
      return nullptr;
    }

    return &elems.at(pos);
  }

private:
  std::vector<T> elems;
  size_t pos = 0;
};

template <typename T> class CombinatorStream : public __Stream<T> {
public:
  CombinatorStream<T>(std::initializer_list<__Stream<T> *> sub_streams)
      : streams{sub_streams} {}
  CombinatorStream<T>(std::vector<__Stream<T> *> sub_streams)
      : streams{std::move(sub_streams)} {}
  virtual bool has_more() override {
    while (pos < streams.size() && !streams.at(pos)->has_more()) {
      ++pos;
    }
    return pos < streams.size();
  }
  virtual T get() override {
    // NOTE: has_more() has a side-effect we make use of here
    if (!has_more()) {
      throw std::logic_error{"requested an element from drained stream"};
    }
    return streams.at(pos)->get();
  }
  virtual ~CombinatorStream<T>() {
    for (auto s : streams) {
      delete s;
    }
  }
  virtual T *peek() override {
    if (!has_more()) {
      return nullptr;
    }
    return streams.at(pos)->peek();
  }

private:
  std::vector<__Stream<T> *> streams;
  size_t pos = 0;
};

template <typename T> class FilteredStream : public __Stream<T> {
public:
  FilteredStream<T>(std::function<bool(T)> predicate, __Stream<T> *stream)
      : pred{predicate}, s{stream} {}
  virtual bool has_more() override {
    return peek() != nullptr;
  }
  virtual T *peek() {
    T *next = s->peek();
    if (next && pred(*next)) {
      return next;
    }
    return nullptr;
  }

private:
  std::function<bool(T)> pred;
  __Stream<T> *s;
};
} // namespace internal

template <typename T> class Stream {
public:
  Stream<T>(internal::__Stream<T> *inner) : owned{inner} {}
  static Stream<T> of(std::initializer_list<T> elems) {
    return Stream<T>{new internal::FixedStream<T>(elems)};
  }
  static Stream<T> of(std::vector<T> elems) {
    return Stream<T>{new internal::FixedStream<T>{std::move(elems)}};
  }
  static Stream<T> concat(Stream<T> s1, Stream<T> s2) {
    return Stream<T>{new internal::CombinatorStream<T>{
        {s1.owned.release(), s2.owned.release()}}};
  }
  static Stream<T> empty() { return Stream<T>{new internal::EmptyStream<T>{}}; }
  bool has_more() { return owned->has_more(); }
  T get() { return owned->get(); }
  void append(Stream<T> &&other) {
    owned.reset(new internal::CombinatorStream<T>{
        {owned.release(), other.owned.release()}});
  }
  void filter(std::function<bool(T)> predicate) {
    owned.reset(new internal::FilteredStream<T>(predicate, owned.release()));
  }

private:
  std::unique_ptr<internal::__Stream<T>> owned;
};

} // namespace lithp::util

#endif // _LITHP_UTIL_STREAM_H_
