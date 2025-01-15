#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include "config.hpp"
#include "types.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"

#include <sstream>
#include <iostream>
#include <vector>
#include <map>

enum class ElementType{
    RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLineData{
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

class Factory{
public:
    Factory();
    bool is_consistent(); // Sprawdzenie spójnosci
    void do_deliveries(); // Odwolanie do rampy
    void do_package_passing(); // odwolanie do przekazywania polproduktow
    void do_work(); // odwolanie do robotnikow
private:

};

ParsedLineData parse_line(string line);

Factory load_factory_structure(std::istream& is);

void save_factory_structure(Factory& factory, std::ostream& os);

#endif // FACTORY_HPP_
