#include <iostream>
#include <chrono>
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <boost/core/demangle.hpp>

class RadioButton;
class CheckBox;
class ComboBox;

class IWidgetVisitor {
 public:
  // add here overload for your new type if you want to cast to it
  virtual void Visit(RadioButton* widget) = 0;
  virtual void Visit(CheckBox* widget) = 0;
  virtual void Visit(ComboBox* widget) = 0;
};

class WidgetVisitor : public IWidgetVisitor {
 public:
  // by default we do nothing
  virtual void Visit(RadioButton* widget) override {}
  virtual void Visit(CheckBox* widget) override {}
  virtual void Visit(ComboBox* widget) override {}
};

template <typename T>
class WidgetCastVisitor;

template <typename T>
class WidgetCastVisitor<T*> : public WidgetVisitor {
 public:
  WidgetCastVisitor()
    : result_{nullptr} {}

  //
  // This overload is used to catch actual type but we fall into this function only if
  // the expected type (see widget_cast<T>) and actual type are the same.
  //
  // Here we just saving it.
  //
  virtual void Visit(T* widget) override {
    result_ = widget;
  }

  [[nodiscard]] T* Result() const {
    return result_;
  }

 private:
  T* result_;
};




// this is the hierarchy of IWidget (RadioButton, CheckBox, ComboBox)
class IWidget {
 public:
  virtual ~IWidget() = default;
  virtual void Accept(IWidgetVisitor* visitor) const = 0;
};

class RadioButton : public IWidget {
 public:
  virtual void Accept(IWidgetVisitor* visitor) const override {
    visitor->Visit(const_cast<RadioButton*>(this));
  }

  int Radius() {
    return 42;
  }
};

class CheckBox : public IWidget {
 public:
  virtual void Accept(IWidgetVisitor* visitor) const override {
    visitor->Visit(const_cast<CheckBox*>(this));
  }

  int Width() {
    return 142;
  }

  int Height() {
    return 142;
  }
};

class ComboBox : public IWidget {
 public:
  virtual void Accept(IWidgetVisitor* visitor) const override {
    visitor->Visit(const_cast<ComboBox*>(this));
  }

  const char* BackgroundColor() {
    return "Green";
  }

  int MaxNumberOfElement() {
    return 15;
  }
};

template <typename T>
T widget_cast(const IWidget* widget) {
  static_assert(std::is_pointer<T>::value, "T must be a pointer type");

  if (!widget) {
    return nullptr;
  }

  WidgetCastVisitor<T> visitor;
  widget->Accept(&visitor);

  return visitor.Result();
}

//
// Usage example and benchmarks
//
void PrintWidgetDetails(RadioButton* radio_button) {
  std::cout
    << "Radio Button radius is "
    << radio_button->Radius()
    << std::endl;
}

void PrintWidgetDetails(CheckBox* check_box) {
  std::cout
    << "Check Box width is "
    << check_box->Width()
    << ", height is "
    << check_box->Height()
    << std::endl;
}

void PrintWidgetDetails(ComboBox* combo_box) {
  std::cout
    << "Combo Box background color is "
    << combo_box->BackgroundColor()
    << ", max number of elements in is "
    << combo_box->MaxNumberOfElement()
    << std::endl;
}

template <typename T>
void TryCast(IWidget* widget) {
  static_assert(std::is_pointer<T>::value, "T must be a pointer type");

  const auto desired_type = boost::core::demangle(typeid(T).name());
  const auto actual = widget_cast<T>(widget);

  if (actual) {
    std::cout
      << widget
      << " successfully casted to "
      << desired_type
      << std::endl;

    PrintWidgetDetails(actual);
  } else {
    std::cout
      << widget
      << " wasn't casted to "
      << desired_type
      << " because this object is of type "
      << boost::core::demangle(typeid(*widget).name()) << std::endl;
  }

  std::cout << std::endl;
}

void BenchmarkDynamicCast(size_t iteration_count) {
  IWidget* radio_button = new RadioButton;

  const auto start = std::chrono::system_clock::now();

  for (auto i{0u}; i < iteration_count; ++i) {
    [[maybe_unused]] volatile const auto result_1 =
      dynamic_cast<RadioButton*>(radio_button);

    [[maybe_unused]] volatile const auto result_2 =
      dynamic_cast<CheckBox*>(radio_button);
  }

  const auto end = std::chrono::system_clock::now();
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout
    << "dynamic_cast's elapsed time: "
    << elapsed.count()
    << "ms"
    << std::endl;
}

void BenchmarkWidgetCast(size_t iteration_count) {
  IWidget* radio_button = new RadioButton;

  const auto start = std::chrono::system_clock::now();

  for (auto i{0u}; i < iteration_count; ++i) {
    [[maybe_unused]] volatile const auto result_1 =
      widget_cast<RadioButton*>(radio_button);

    [[maybe_unused]] volatile const auto result_2 =
      widget_cast<CheckBox*>(radio_button);
  }

  const auto end = std::chrono::system_clock::now();
  const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

  std::cout
    << "widget_cast's elapsed time: "
    << elapsed.count()
    << "ms"
    << std::endl;
}

int main() {
  const std::shared_ptr<IWidget> radio_button = std::make_shared<RadioButton>();
  const std::shared_ptr<IWidget> check_box = std::make_shared<CheckBox>();
  const std::shared_ptr<IWidget> combo_box = std::make_shared<ComboBox>();

  // normal casts
  TryCast<RadioButton*>(radio_button.get());
  TryCast<CheckBox*>(check_box.get());
  TryCast<ComboBox*>(combo_box.get());

  // invalid casts
  TryCast<CheckBox*>(radio_button.get());
  TryCast<RadioButton*>(check_box.get());
  TryCast<CheckBox*>(combo_box.get());

  constexpr auto kIterationCount = 100'000'000u;
  BenchmarkDynamicCast(kIterationCount);
  BenchmarkWidgetCast(kIterationCount);
}