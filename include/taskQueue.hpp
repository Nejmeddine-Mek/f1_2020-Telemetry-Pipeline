#ifndef TASKQUEUE_HPP
#define TASKQUEUE_HPP
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class TaskQueue {
public:
    using Task = std::function<void()>;

private:
    std::queue<Task> queue;
    std::mutex mtx;
    std::condition_variable cv;
    bool stopRequested = false;

public:
    TaskQueue() = default;

    void push(Task task);
    bool pop(Task& outTask);

    // Unblocks the Writer thread during application shutdown
    void stop();
};
#endif