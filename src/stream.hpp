#ifndef LITHP_UTIL_STREAM_HPP
#define LITHP_UTIL_STREAM_HPP

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
};

template <typename T> class EmptyStream : public __Stream<T> {
public:
  EmptyStream<T>() {}
  virtual bool has_more() override { return false; }
  virtual T get() override {
    throw std::logic_error{"requested an element from an empty stream"};
  }
  virtual ~EmptyStream<T>() = default;
};

template <typename T> class FixedStream : public __Stream<T> {
public:
  FixedStream<T>(std::initializer_list<T> elems) : elems{elems} {}
  FixedStream<T>(std::vector<T> elems) : elems{std::move(elems)} {}
  virtual bool has_more() override { return pos < elems.size(); }
  virtual T get() override {
    if (!has_more()) {
      throw std::logic_error{"requested an element from drained stream"};
    }
    return elems.at(pos++);
  }
  virtual ~FixedStream<T>() = default;

private:
  std::vector<T> elems;
  size_t pos = 0;
};

template <typename T> class AggregateStream : public __Stream<T> {
public:
  AggregateStream<T>(std::initializer_list<__Stream<T> *> sub_streams)
      : streams{sub_streams} {}
  AggregateStream<T>(std::vector<__Stream<T> *> sub_streams)
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
  virtual ~AggregateStream<T>() {
    for (auto s : streams) {
      delete s;
    }
  }

private:
  std::vector<__Stream<T> *> streams;
  size_t pos = 0;
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
    return Stream<T>{new internal::AggregateStream<T>{
        {s1.owned.release(), s2.owned.release()}}};
  }
  static Stream<T> empty() { return Stream<T>{new internal::EmptyStream<T>{}}; }
  bool has_more() { return owned->has_more(); }
  T get() { return owned->get(); }
  void append(Stream<T> &&other) {
    owned.reset(new internal::AggregateStream<T>{
        {owned.release(), other.owned.release()}});
  }

private:
  std::unique_ptr<internal::__Stream<T>> owned;
};

} // namespace lithp::util

#endif /* LITHP_UTIL_STREAM_HPP */
