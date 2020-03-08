#include "thread_pool.h"
#include <iostream>
#include <future>
#include <experimental/coroutine>

using namespace std::chrono_literals;

struct LongTask {
  static size_t kId;

  size_t id;

  LongTask() {
    id = kId++;
  }

  void operator()() {
    std::this_thread::sleep_for(5s);
    std::cout << "LongTask #" << id << " completed" << std::endl;
  }
};

size_t LongTask::kId;

struct ShortTask {
  static size_t kId;

  size_t id;

  ShortTask() {
    id = kId++;
  }

  void operator()() {
    std::this_thread::sleep_for(2s);
    std::cout << "ShortTask #" << id << " completed" << std::endl;
  }
};

size_t ShortTask::kId;

template <typename Task>
struct Awaiter {
  struct TaskWrapper {
    Task task;
    std::experimental::coroutine_handle<> coro_handle;

    void operator()() {
      task();
      coro_handle();
    }
  };

  Task task;

  Awaiter(Task task)
    : task(task) {
  }

  bool await_ready() const {
    return false;
  }

  void await_suspend(std::experimental::coroutine_handle<> coro_handle) {
    TaskWrapper task_wrapper{ task, coro_handle };
    ThreadPool::instance().schedule(task_wrapper);
  }

  void await_resume() { }
};

template <typename Task>
auto operator co_await(Task task) {
  return Awaiter{ task };
}

std::future<void> coro() {
  std::cout << "awaiting task from thread " << std::this_thread::get_id() << std::endl;
  co_await LongTask{};

  std::cout << "awaiting task from thread " << std::this_thread::get_id() << std::endl;
  co_await ShortTask{};

  std::cout << "awaiting task from thread " << std::this_thread::get_id() << std::endl;
  co_await LongTask{};

  std::cout << "awaiting task from thread " << std::this_thread::get_id() << std::endl;
  co_await ShortTask{};

  std::cout << "all tasks done and we are in the " << std::this_thread::get_id() << " thread" << std::endl;

  co_return;
}

int main() {
  auto f = coro();
  f.get();

  std::cout << "Done" << std::endl;
}
