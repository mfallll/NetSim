#ifndef FACTORY_HPP_
#define FACTORY_HPP_

#include "config.hpp"
#include "types.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"
#include "nodes.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <stack>


#include <vector> // Przykład z std::vector jako std_container_t xd

template <typename Node, template <typename> class std_container_t = std::vector>
class NodeCollection {
public:
    using container_t = class std_container_t<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    const_iterator cbegin() const { return nodes.cbegin(); }
    const_iterator cend() const { return nodes.cend(); }
    iterator begin() { return nodes.begin(); }
    iterator end() { return nodes.end(); }
    const_iterator begin() const { return nodes.cbegin(); }
    const_iterator end() const { return nodes.cend(); }

    void add(const Node&& node) { nodes.push_back(std::move(node)); }
    std::size_t size() const { return nodes.size(); }
    iterator find_by_id(ElementID id);
    const_iterator find_by_id(ElementID id) const;
    void remove_by_id(ElementID id);

private:
    container_t nodes;
};

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
    bool is_consistent();       // Sprawdzenie spójnosci
    void do_deliveries();       // Odwolanie do rampy
    void do_package_passing();  // odwolanie do przekazywania polproduktow
    void do_work();             // odwolanie do robotnikow

    void add_ramp(Ramp&& rmp){RampCont.add(std::move(rmp));}
    void remove_ramp(ElementID id){RampCont.remove_by_id(id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id){return RampCont.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return RampCont.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin(){RampCont.begin();}
    NodeCollection<Ramp>::const_iterator ramp_cend(){RampCont.end();}

    void add_worker(Ramp&& rmp);
    void remove_worker(ElementID id);
    NodeCollection<Ramp>::iterator find_worker_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator find_worker_by_id(ElementID id) const;
    NodeCollection<Ramp>::const_iterator worker_cbegin();
    NodeCollection<Ramp>::const_iterator worker_cend();

    void add_storehouse(Ramp&& rmp);
    void remove_storehouse(ElementID id);
    NodeCollection<Ramp>::iterator find_storehouse_by_id(ElementID id);
    NodeCollection<Ramp>::const_iterator find_storehouse_by_id(ElementID id) const;
    NodeCollection<Ramp>::const_iterator storehouse_cbegin();
    NodeCollection<Ramp>::const_iterator storehouse_cend();
private:
    NodeCollection<Ramp> RampCont;
    NodeCollection<Worker> WorkerCont;
    NodeCollection<Storehouse> StorehouseCont;
};

ParsedLineData parse_line(std::string& line);

Factory load_factory_structure(std::istream& is);

void save_factory_structure(Factory& factory, std::ostream& os);

#endif // FACTORY_HPP_
