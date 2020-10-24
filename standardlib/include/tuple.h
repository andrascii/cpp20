#include "type_in_pack.h"
#include "type_at_index.h"
#include <cstdio>
#include <utility>
#include <type_traits>

template <std::size_t I, typename T>
struct TupleElement {
  TupleElement() = default;

  template <typename U>
  TupleElement(U&& arg)
    : value(std::forward<U>(arg))
  {}

  T value;
};

template <typename, typename...>
struct TupleImpl;

template <typename T>
struct IsTupleImpl : std::false_type {};

template <size_t... Idx, typename... Args>
struct IsTupleImpl<TupleImpl<std::index_sequence<Idx...>, Args...>> : std::true_type {};

template <std::size_t... Indices, typename... Args>
struct TupleImpl<std::index_sequence<Indices...>, Args...> : TupleElement<Indices, Args>... {
  TupleImpl() = default;

  template <typename... CompatibleTypes,
    typename = typename std::enable_if<!isAnyOf<IsTupleImpl, typename std::decay<CompatibleTypes>::type...>()>::type
  >
  TupleImpl(CompatibleTypes&&... pack)
    : TupleElement<Indices, Args>(std::forward<CompatibleTypes>(pack))...
  {}

  template <typename... CompatibleTypes>
  TupleImpl(TupleImpl<std::index_sequence<Indices...>, CompatibleTypes...> const& tupleImpl)
    : TupleElement<Indices, Args>(static_cast<TupleElement<Indices, CompatibleTypes> const&>(tupleImpl).value)...
  {}
};

template <typename... Args>
struct Tuple;

template <typename T>
struct IsTuple : std::false_type {};

template <typename... Args>
struct IsTuple<Tuple<Args...>> : std::true_type {};

template <typename... Args>
struct Tuple : TupleImpl<std::index_sequence_for<Args...>, Args...> {
  using DirectBaseType = TupleImpl<std::index_sequence_for<Args...>, Args...>;

  Tuple() = default;
  //Tuple(Tuple const&) = delete;
  ~Tuple() = default;
  Tuple& operator=(Tuple const&) = default;
  Tuple& operator=(Tuple&&) = default;

  template <typename... CompatibleTypes,
    typename = typename std::enable_if<!isAnyOf<IsTuple, typename std::decay<CompatibleTypes>::type...>()>::type
  >
  Tuple(CompatibleTypes&&... pack)
    : DirectBaseType(std::forward<CompatibleTypes>(pack)...)
  {}

  template <typename... OtherTypes>
  Tuple(Tuple<OtherTypes...> const& tuple)
    : DirectBaseType(tuple)
  {}

  template <typename... OtherTypes>
  Tuple(Tuple<OtherTypes...>&& tuple)
    : DirectBaseType(std::move(tuple))
  {}
};

// get by index implementation

template <size_t Idx, typename... Args>
TypeAtSpecifiedIndex<Idx, Args...> const& get(Tuple<Args...> const& tuple) {
  static_assert(Idx < sizeof...(Args), "Invalid index");
  return static_cast<TupleElement<Idx, TypeAtSpecifiedIndex<Idx, Args...>> const&>(tuple).value;
}

template <size_t Idx, typename... Args>
TypeAtSpecifiedIndex<Idx, Args...>& get(Tuple<Args...>& tuple) {
  static_assert(Idx < sizeof...(Args), "Invalid index");
  return static_cast<TupleElement<Idx, TypeAtSpecifiedIndex<Idx, Args...>>&>(tuple).value;
}

template <size_t Idx, typename... Args>
TypeAtSpecifiedIndex<Idx, Args...> get(Tuple<Args...>&& tuple) {
  static_assert(Idx < sizeof...(Args), "Invalid index");
  return static_cast<TupleElement<Idx, TypeAtSpecifiedIndex<Idx, Args...>>&&>(tuple).value;
}

// get by type implementation

template <typename Searching, typename... Args>
typename TypeInPackHelper<0, Searching, Args...>::Type const& get(Tuple<Args...> const& tuple) {
  constexpr size_t index = TypeInPackHelper<0, Searching, Args...>::index;
  return static_cast<TupleElement<index, TypeAtSpecifiedIndex<index, Args...>> const&>(tuple).value;
}

template <typename Searching, typename... Args>
typename TypeInPackHelper<0, Searching, Args...>::Type& get(Tuple<Args...>& tuple) {}

template <typename Searching, typename... Args>
typename TypeInPackHelper<0, Searching, Args...>::Type&& get(Tuple<Args...>&& tuple) {}
