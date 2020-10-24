#pragma once

#include <cstdio>
#include <type_traits>

template <size_t Idx, typename Searching, typename Head, typename... Tail>
struct TypeInPackHelper {
  static_assert(!(!std::is_same<Searching, Head>::value && sizeof...(Tail) == 0),
    "Tuple doesn't contain specified type");

  using Type = typename TypeInPackHelper<Idx + 1, Searching, Tail...>::Type;
  static constexpr size_t index = Idx;
};

template <size_t Idx, typename Searching, typename... Tail>
struct TypeInPackHelper<Idx, Searching, Searching, Tail...> {
  using Type = Searching;
  static constexpr size_t index = Idx;
};

template <typename Searching, typename Head, typename... Tail>
using TypeInPack = TypeInPackHelper<0, Searching, Head, Tail...>;
