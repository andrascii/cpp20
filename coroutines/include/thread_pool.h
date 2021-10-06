#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>
#include <thread>
#include <atomic>

class ThreadPool {
public:
  static ThreadPool& instance() {
    static std::unique_ptr<ThreadPool> kInstance = nullptr;

    if (!kInstance) {
      kInstance.reset(new ThreadPool);
    }

    return *kInstance;
  }

  ~ThreadPool() {
    break_.store(true, std::memory_order_relaxed);
    signal_.notify_all();

    for (auto& thread : threads_) {
      if (!thread.joinable()) {
        continue;
      }

      thread.join();
    }
  }

  void join_all() {
    for (auto& thread : threads_) {
      if (!thread.joinable()) {
        continue;
      }

      thread.join();
    }
  }

  void schedule(const std::function<void()>& job) {
    std::lock_guard lock(mutex_);
    queue_.push(job);
    signal_.notify_all();
  }

private:
  struct BreakException {};

  ThreadPool()
    : break_(false) {
    for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
      threads_.push_back(std::thread(&ThreadPool::loop, this));
    }
  }

  void loop() {
    while (true) {
      std::function<void()> job;

      try {
        job = extract_job();
      }
      catch (BreakException) {
        break;
      }

      job();
    }
  }

  std::function<void()> extract_job() {
    std::unique_lock locker(mutex_);
    signal_.wait(locker, [this] {
      return !queue_.empty() || break_;
    });

    if (break_) {
      throw BreakException();
    }

    std::function<void()> job = std::move(queue_.front());
    queue_.pop();

    return job;
  }

private:
  mutable std::mutex mutex_;
  std::queue<std::function<void()>> queue_;
  std::condition_variable signal_;
  std::vector<std::thread> threads_;
  std::atomic_bool break_;
};