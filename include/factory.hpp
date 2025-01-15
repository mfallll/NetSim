#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include <stack>
#include "config.hpp"
#include "types.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"

template <typename Node> class NodeCollection {
public:
    void add(Node &&node);
    void remove_by_id(ElementID id);

    using stack = typename std::stack<Node>;
    using iterator = typename stack::iterator;
    using const_iterator = typename stack::const_iterator;



    NodeCollection<Node>::iterator find_by_id(ElementID id);
    NodeCollection<Node>::const_iterator find_by_id(ElementID id);
};


class factory{
    factory();
    bool is_consistent(); // Sprawdzenie spójnosci
    void do_deliveries(); // Odwolanie do rampy
    void do_package_passing(); // odwolanie do przekazywania polproduktow
    void do_work(); //
};



#endif // FACTORY_HPP_
