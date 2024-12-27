#include "storage_types.hpp"

Package PackageQueue::pop() {
        Package package;
        PackageQueueType t = queuetype_;
        switch(t) {
            case PackageQueueType::FIFO:
                package = std::move(queue_.front());
                queue_.pop_front();
                break;
            case PackageQueueType::LIFO:
                package = std::move(queue_.back());
                queue_.pop_back();
                break;
        }
        return package;
}