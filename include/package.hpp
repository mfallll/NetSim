#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "types.hpp"
#include <set>

//bombelki 3

class Package {
public:
    Package();
    explicit Package(ElementID id) : id_(id) { assigned_IDs.insert(id_); };
    Package(Package &&package) noexcept : id_(package.id_) {};
    Package &operator=(Package &&package) noexcept;
    ElementID get_id() const { return id_; }
    ~Package();
private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
};

#endif //PACKAGE_HPP