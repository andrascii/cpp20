#pragma once

// metafunction applies template class predicate to all types in chain

template <template <typename> class>
constexpr bool isAnyOf() {
  return false;
}

template <template <typename> class Pred, typename Head, typename... Tail>
constexpr bool isAnyOf() {
  return Pred<Head>::value || isAnyOf<Pred, Tail...>();
}
