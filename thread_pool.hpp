#ifndef _THREAD_POOL_HPP_
#define _THREAD_POOL_HPP_

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace jdi {

class thread_pool {
private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;

  std::mutex tasks_mutex_;
  std::condition_variable tasks_cv_;

  bool stop_it_ = false;

public:
  thread_pool(size_t num);
  ~thread_pool();

  void add_task(std::function<void()> &&task);
};

thread_pool::thread_pool(size_t num) {
  auto work = [this]() {
    while (true) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(this->tasks_mutex_);
        this->tasks_cv_.wait(
            lock, [this]() { return this->stop_it_ || !this->tasks_.empty(); });
        // 모든 작업을 완료하였을 경우 종료
        if (this->stop_it_ && this->tasks_.empty())
          return;

        task = std::move(this->tasks_.front());
        this->tasks_.pop();
      }
      task();
    }
  };

  workers_.reserve(num);
  for (size_t i = 0; i < num; i++)
    workers_.push_back(std::thread(work));
}

thread_pool::~thread_pool() {
  stop_it_ = true;
  tasks_cv_.notify_all();

  for (auto &w : workers_) {
    w.join();
  }
}

void thread_pool::add_task(std::function<void()> &&task) {
  {
    std::unique_lock<std::mutex> lock(tasks_mutex_);
    tasks_.push(std::move(task));
  }
  tasks_cv_.notify_one();
}

} // namespace jdi

#endif /* _THREAD_POOL_HPP_ */
