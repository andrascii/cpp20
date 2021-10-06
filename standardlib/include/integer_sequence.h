#pragma once

template <typename T, T... S>
struct IntegerSequence {
  using SequenceType = IntegerSequence<T, S - 1, S>::SequenceType;
};

template <typename T, T Head, T... Pack>
struct IntegerSequence {};

template <typename T, T Head>
struct IntegerSequence {};

