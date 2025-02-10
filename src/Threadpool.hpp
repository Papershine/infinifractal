#include <thread>
#include <mutex>
#include <condition_variable>

struct task {
  std::function<void(void*)> func;
  void* arg;
};

class Threadpool {
  public:
    unsigned max_threads;
    bool running;

    Threadpool();
    void schedule(std::function<void(void*)> func, void* arg);
    void join();

  private:
    std::vector<std::thread> threads;
    std::queue<task> queue;
    std::mutex queue_mutex;
    std::condition_variable has_tasks;

    void worker();
};
