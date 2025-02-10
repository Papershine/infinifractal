#include <Threadpool.hpp>
#include <iostream>

Threadpool::Threadpool() {
  this->max_threads = std::thread::hardware_concurrency();
  this->running = true;
  for (unsigned i=0; i<this->max_threads; i++) {
    this->threads.push_back(std::thread(&Threadpool::worker, this));
  }
}

void Threadpool::schedule(std::function<void(void*)> func, void *arg) {
  std::unique_lock lock(this->queue_mutex);
  this->queue.push({func, arg});
  this->has_tasks.notify_one();
}

void Threadpool::join() {
  {
    std::unique_lock lock(this->queue_mutex);
    this->running = false;
  }
  this->has_tasks.notify_all();

  for (auto& thread : this->threads) {
    thread.join();
  }
}

void Threadpool::worker() {
  while (true) {
    task task;
    {
      std::unique_lock lock(this->queue_mutex);
      this->has_tasks.wait(lock, [this]{ return !queue.empty() || !running; });

      if (this->queue.empty() && !this->running) {
        return;
      }

      task = this->queue.front();
      this->queue.pop();
    }

    std::function<void(void*)> func = task.func;
    void* arg = task.arg;
    func(arg);
  }
}