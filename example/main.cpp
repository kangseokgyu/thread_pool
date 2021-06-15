#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "thread_pool.hpp"

class task {
private:
  std::string name_ = "";

public:
  task(std::string &&task_name) : name_(task_name) {
    std::cout << "[" << this << "] construct task.[" << name_ << ": " << &name_
              << "]\n";
  }
  task(task &&t) : name_(t.name_) {
    std::cout << "[" << this << "] move task.[" << name_ << ": " << &name_
              << "]\n";
  }
  ~task() { std::cout << "[" << this << "] destruct task.[" << name_ << "]\n"; }

  std::function<void()> run_task(int i) {
    return [i, name = this->name_]() {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      std::cout << name << ": " << &name << '\n';
    };
  }
};

int main() {
  thread_pool pool(20);
  std::vector<task> tasks;
  for (int i = 0; i < 10; i++) {
    tasks.emplace_back("task " + std::to_string(i));
  }

  for (auto &t : tasks) {
    pool.add_task(t.run_task(0));
    // std::cout << "add task [" << i << "]\n";
  }

  return 0;
}
