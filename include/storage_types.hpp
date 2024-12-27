#ifndef STORAGE_TYPES_HPP
#define STORAGE_TYPES_HPP

#include "package.hpp"
#include <list>
#include <cstddef>

// Biblioteka <vector> zawiera definicjÄ™ szablonu klasy `std::vector`.
#include <vector>

#include <string>


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
}

#endif //STORAGE_TYPES_HPP