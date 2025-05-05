#include <thread>
#include <mutex>
#include <condition_variable>

class Threadpool {
  public:
    unsigned max_threads;
    bool running;

    Threadpool() {
      this->max_threads = std::thread::hardware_concurrency();
      this->running = true;
      for (unsigned i=0; i<this->max_threads; i++) {
        this->threads.push_back(std::thread(&Threadpool::worker, this));
      }
    };


    template <typename F, typename... Args>
    void schedule(F&& f, Args&&... args) {
      auto task = [func = std::forward<F>(f),
                   tup = std::make_tuple(std::forward<Args>(args)...)]() mutable {
        std::apply(func, std::move(tup));
      };

      {
        std::unique_lock lock(this->queue_mutex);
        this->queue.push(std::move(task));
      }
      this->has_tasks.notify_one();
    };

    void join() {
      {
        std::unique_lock lock(this->queue_mutex);
        this->running = false;
      }
      this->has_tasks.notify_all();

      for (auto& thread : this->threads) {
        thread.join();
      }
    };

  private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> queue;
    std::mutex queue_mutex;
    std::condition_variable has_tasks;

    void worker() {
      while (true) {
        std::function<void()> task;
        {
          std::unique_lock lock(this->queue_mutex);
          this->has_tasks.wait(lock, [this]{ return !this->queue.empty() || !this->running; });

          if (this->queue.empty() && !this->running) {
            return;
          }

          task = std::move(this->queue.front());
          this->queue.pop();
        }

        task();
      }
    }
};
