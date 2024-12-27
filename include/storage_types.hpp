#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP

#include "package.hpp"
#include <list>


enum class PackageQueueType {
    FIFO,
    LIFO
};

class IPackageStockpile {
    public:

    virtual void push(Package&& package) = 0;
    virtual bool empty() const = 0;
    virtual std::size_t size() const = 0;

    //iteratory
    using const_iterator = std::list<Package>::const_iterator;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile {
    public:

    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;

    ~IPackageQueue() override = default;
};

class PackageQueue : public IPackageQueue {
    public:

    PackageQueue(PackageQueueType queuetype) : queue_(), queuetype_(queuetype) {}

    void push(Package&& package) override { queue_.emplace_back(std::move(package)); }
    std::size_t size() const override { return queue_.size(); }

    //iteratory
    const_iterator begin() const override { return queue_.cbegin(); }
    const_iterator end() const override { return queue_.cend(); }
    const_iterator cbegin() const override { return queue_.cbegin(); }
    const_iterator cend() const override {return queue_.cend(); }

    bool empty() const override { return queue_.empty(); }

    Package pop() override;

    PackageQueueType get_queue_type() const override { return queuetype_; }

    ~PackageQueue() override = default;

    private:

    std::list<Package> queue_;
    PackageQueueType queuetype_;
};

#endif //STORAGE_TYPES_HPP