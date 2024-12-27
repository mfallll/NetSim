#include "package.hpp"

std::set<ElementID> Package::assigned_IDs = {};
std::set<ElementID> Package::freed_IDs = {};

Package::Package() {
    if (assigned_IDs.empty() && freed_IDs.empty()) {
        id_ = 1;
    }
    else if (!freed_IDs.empty()) {
        id_ = *freed_IDs.begin();
        freed_IDs.erase(*freed_IDs.begin())
    }
    else if (!assigned_IDs.empty()) {
        id_ = assigned_IDs.end() + 1;
    }
    assigned_IDs.insert(id_)
}

Package &Package::operator=(Package &&package) noexcept {
    if (this == &package)
        return *this;
    assigned_IDs.erase(this->ID_);
    freed_IDs.insert(this->ID_);
    this->ID_ = package.ID_;
    assigned_IDs.insert(this->ID_);
    return *this;
}

Package::~Package() {
    assigned_IDs.erase(id_);
    freed_IDs.insert(id_);
}