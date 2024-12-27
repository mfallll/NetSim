#include "types.hpp"
#include <set>

class Package {
public:
    Package();
    Package(ElementID id) : id_(id) {};
    Package(Package &&package) : id_(package.id_) {};
    Package &operator=(Package &&package);
    ElementID get_id() { return id_; }
    ~Package();
private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs_;
    static std::set<ElementID> freed_IDs_;
};
