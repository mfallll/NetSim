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
#include <deque>
#include <map>
#include <stack>
#include <vector>

template <typename Node>
class NodeCollection {
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    const_iterator cbegin() const { return nodes.cbegin(); }
    const_iterator cend() const { return nodes.cend(); }
    iterator begin() { return nodes.begin(); }
    iterator end() { return nodes.end(); }
    const_iterator begin() const { return nodes.cbegin(); }
    const_iterator end() const { return nodes.cend(); }

    void add(Node&& node) { nodes.push_back(std::move(node)); }
    std::size_t size() const { return nodes.size(); }
    iterator find_by_id(ElementID id) {return std::find_if(nodes.begin(), nodes.end(), [id](Node& node){ return id == node.get_id(); });}
    const_iterator find_by_id(ElementID id) const { return std::find_if(nodes.cbegin(), nodes.cend(), [id](const Node& node){ return id == node.get_id(); });};
    void remove_by_id(ElementID id);
    //void remove_by_id(ElementID id) { nodes.remove_if([id](const Node& elem) { return elem.get_id() == id; }); }

private:
    container_t nodes;

};

template<typename Node>
void NodeCollection<Node>::remove_by_id(ElementID id)  {
    auto it = find_by_id(id);
    if (it != nodes.end()) {
        this->nodes.erase(it);
    }
}


enum class ElementType{
    RAMP, WORKER, STOREHOUSE, LINK
};

struct ParsedLineData{
    ElementType element_type;
    std::map<std::string, std::string> parameters;
};

enum class NodeColor { UNVISITED, VISITED, VERIFIED };
class Factory{
public:
    Factory() = default;

    bool is_consistent();       // Sprawdzenie spójnosci
    void do_deliveries(Time t);       // Odwolanie do rampy
    void do_package_passing();  // odwolanie do przekazywania polproduktow
    void do_work(Time t);             // odwolanie do robotnikow

    void add_ramp(Ramp&& rmp){RampCont.add(std::move(rmp));}
    void remove_ramp(ElementID id){RampCont.remove_by_id(id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id){return RampCont.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return RampCont.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return RampCont.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() const {return RampCont.cend();}

    void add_worker(Worker&& wrk){WorkerCont.add(std::move(wrk));}
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){return WorkerCont.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return WorkerCont.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return WorkerCont.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() const {return WorkerCont.cend();}

    void add_storehouse(Storehouse&& str){StorehouseCont.add(std::move(str));}
    void remove_storehouse(ElementID id){StorehouseCont.remove_by_id(id);}
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return StorehouseCont.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return StorehouseCont.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return StorehouseCont.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return StorehouseCont.cend();}
private:
    NodeCollection<Ramp> RampCont;
    NodeCollection<Worker> WorkerCont;
    NodeCollection<Storehouse> StorehouseCont;


    bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);
};

ParsedLineData parse_line(std::string& line);

Factory load_factory_structure(std::istream& is);

void save_factory_structure(Factory& factory, std::ostream& os);

#endif // FACTORY_HPP_
