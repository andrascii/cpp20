#include "tuple.h"
#include <variant>
#include <iostream>
#include <type_traits>
#include <chrono>
#include <date/date.h>
#include <date/tz.h>

struct DataSet1 {
  int value = 1;
};

struct DataSet2 {
  int value = 2;
};

struct DataSet3 {
  int value = 3;
};

struct DataSet4 {
  int value = 4;
};

struct DataSet5 {
  int value = 5;
};

using DataSet = std::variant<DataSet1, DataSet2, DataSet3, DataSet4, DataSet5>;

enum class DataSetType {
  TypeDataSet1,
  TypeDataSet2,
  TypeDataSet3,
  TypeDataSet4,
  TypeDataSet5,
};

auto CreateDataSet(DataSetType type) -> DataSet {
  switch (type) {
    case DataSetType::TypeDataSet1: return DataSet1{};
    case DataSetType::TypeDataSet2: return DataSet2{};
    case DataSetType::TypeDataSet3: return DataSet3{};
    case DataSetType::TypeDataSet4: return DataSet4{};
    case DataSetType::TypeDataSet5: return DataSet5{};
  }

  throw std::runtime_error("Unknown data set");
}

template <typename... Args> struct TypeList {};

template <template <typename... Args> class C, typename... Args>
TypeList<Args...> FromType(C<Args...>&&) {
  return TypeList<Args...>();
}

template <typename... Callbacks>
struct CallbackPackage : Callbacks... {
  using Callbacks::operator()...;
};

template <typename ...Callbacks>
CallbackPackage(Callbacks...) -> CallbackPackage<Callbacks...>;

int main(int argc, char* argv[]) {
  using namespace std::chrono_literals;

  (void)argc;
  (void)argv;

  DataSet data_set = CreateDataSet(DataSetType::TypeDataSet4);
  //assert(TypeAtIndex<data_set.index(), >);

  std::visit([](auto&& data) {
    if constexpr (std::is_same_v<DataSet1, std::remove_reference_t<std::remove_cv_t<decltype(data)>>>) {
      std::cout << "Data set value: " << data.value << std::endl;
    } else if constexpr (std::is_same_v<DataSet2, std::remove_reference_t<std::remove_cv_t<decltype(data)>>>) {
      std::cout << "Data set value: " << data.value << std::endl;
    } else if constexpr  (std::is_same_v<DataSet3, std::remove_reference_t<std::remove_cv_t<decltype(data)>>>) {
      std::cout << "Data set value: " << data.value << std::endl;
    } else if constexpr  (std::is_same_v<DataSet4, std::remove_reference_t<std::remove_cv_t<decltype(data)>>>) {
      std::cout << "Data set value: " << data.value << std::endl;
    } else if constexpr (std::is_same_v<DataSet5, std::remove_reference_t<std::remove_cv_t<decltype(data)>>>) {
      std::cout << "Data set value: " << data.value << std::endl;
    }
  }, data_set);


  std::cout << "Handle variant value by CallbacksPackage:" << std::endl;

  std::visit(CallbackPackage{
    [](const DataSet1& data) { std::cout << data.value << std::endl; },
    [](const DataSet2& data) { std::cout << data.value << std::endl; },
    [](const DataSet3& data) { std::cout << data.value << std::endl; },
    [](const DataSet4& data) { std::cout << data.value << std::endl; },
    [](const DataSet5& data) { std::cout << data.value << std::endl; }
  }, data_set);

  //=============================== Tuple ===============================

  Tuple<int, double, float> tuple;

  get<0>(tuple) = 9;
  get<1>(tuple) = 10.0006;
  get<2>(tuple) = 11.003f;

  // checking TypeAtIndex metafunction vagueness implementation
  static_assert(std::is_same<TypeAtSpecifiedIndex<0, int, double, float>, int>::value, "Invalid type");
  static_assert(std::is_same<TypeAtSpecifiedIndex<1, int, double, float>, double>::value, "Invalid type");
  static_assert(std::is_same<TypeAtSpecifiedIndex<2, int, double, float>, float>::value, "Invalid type");

  std::cout << get<0>(tuple) << std::endl;
  std::cout << get<1>(tuple) << std::endl;
  std::cout << get<2>(tuple) << std::endl;

  using namespace date;
  using namespace std::chrono;

  auto const current_time = system_clock::now();
  auto const tz = make_zoned(locate_zone("Europe/Moscow"), current_time);
  std::cout << "offset: " << tz.get_info().offset << std::endl;
  std::cout << "msk: " << tz << std::endl;
  std::cout << "utc: " << current_time << std::endl;
  std::cout << "msk: " << tz.get_local_time().time_since_epoch().count() << std::endl;
  std::cout << "utc: " << tz.get_sys_time().time_since_epoch().count() << std::endl;
  std::cout << "utc: " << current_time.time_since_epoch().count() << std::endl;

  return 0;
}
