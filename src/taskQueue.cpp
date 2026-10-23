#include "../include/taskQueue.hpp"
    // Parser pushes executable lambda tasks here
    void TaskQueue::push(Task task) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(std::move(task));
        }
        cv.notify_one(); // Wake up Writer thread
    }
    
    // Writer thread pops executable tasks here
    bool TaskQueue::pop(Task& outTask) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty() || stopRequested; });

        if (stopRequested && queue.empty()) {
            return false;
        }

        outTask = std::move(queue.front());
        queue.pop();
        return true;
    }
        void TaskQueue::stop() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stopRequested = true;
        }
        cv.notify_all();
    }