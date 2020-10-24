#pragma once

#include <cstdio>

template <size_t Idx, typename Head, typename... Tail>
struct TypeAtIndex {
  using Type = typename TypeAtIndex<Idx - 1, Tail...>::Type;
};

template <typename Head, typename... Tail>
struct TypeAtIndex<0, Head, Tail...> {
  using Type = Head;
};

template <size_t Idx, typename... Args>
using TypeAtSpecifiedIndex = typename TypeAtIndex<Idx, Args...>::Type;
