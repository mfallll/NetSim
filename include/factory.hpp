#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include "config.hpp"
#include "types.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"

class factory{
    factory();
    bool is_consistent(); // Sprawdzenie spójnosci
    void do_deliveries(); // Odwolanie do rampy
    void do_package_passing(); // odwolanie do przekazywania polproduktow
    void do_work(); //
};



#endif // FACTORY_HPP_
