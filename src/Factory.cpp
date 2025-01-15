#include "Factory.hpp"
#include <unique>


Factory::Factory() {
    //potrrzebuje zmiany
}

bool Factory::is_consistent() {
    /* Użyj algorytmu DFS do przejścia grafu, dla każdej rampy. */

//    kolor := mapowanie {PackageSender* -> kolor_węzła_enum}
//    dla wszystkich ramp i dla wszystkich robotników jako `węzeł`:
//    kolor[węzeł] = NIEODWIEDZONY


//
//    jeśli poniższa instrukcja rzuci wyjątek, zwróć FAŁSZ:  /* tj. sieć nie jest spójna */
//    dla każdej rampy `rampa` w kolekcji wszystkich ramp w sieci:
//    czy_nadawca_posiada_osiągalny_magazyn(rampa, kolor)
//    w przeciwnym razie:
//    zwróć PRAWDA  /* tj. sieć jest spójna */


    return false;
}

void Factory::do_deliveries() {

}

void Factory::do_package_passing() {

}

void Factory::do_work() {

}

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::stringstream test(str);
    std::string segment;
    std::vector<std::string> seglist;

    while(std::getline(test, segment, delimiter))
    {
        seglist.push_back(segment);
    }
    return seglist;
}

ParsedLineData parse_line(std::string& line){
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    std::map<std::string, ElementType> map_element_types{
            {"LOADING_RAMP", ElementType::RAMP},
            {"WORKER", ElementType::WORKER},
            {"STOREHOUSE", ElementType::STOREHOUSE},
            {"LINK", ElementType::LINK}
    };

    ParsedLineData parsed_line_data;

    try {
        parsed_line_data.element_type = map_element_types.at(tokens[0]);
        std::for_each(tokens.begin(), tokens.end(), [&](const std::string & please){
            auto value = split(please, '=');
            parsed_line_data.parameters[value[0]] = value[1];
        });
    }
    catch( std::out_of_range& e) { throw std::exception(); }

    return parsed_line_data;
}

void Factory::add_ramp(Ramp &&rmp) {

}

void Factory::remove_ramp(ElementID id) {

}

NodeCollection<Ramp>::iterator Factory::find_ramp_by_id(ElementID id) {
    return nullptr;
}

NodeCollection<Ramp>::const_iterator Factory::find_ramp_by_id(ElementID id) const {
    return nullptr;
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cbegin() {
    return nullptr;
}

NodeCollection<Ramp>::const_iterator Factory::ramp_cend() {
    return nullptr;
}

Factory load_factory_structure(std::istream& is){
    Factory factory;
    std::string line;
    while (std::getline(is, line)) {
        if (line.empty() or line[0] == ';'){
            continue;
        }
        ParsedLineData parsed_line = parse_line(line);
        switch(parsed_line.element_type){
            case ElementType::RAMP:{
                ElementID element_id = std::stoi(parsed_line.parameters.at("id"));
                TimeOffset time_offset = std::stoi(parsed_line.parameters.at("processing-time"));
                Ramp ramp(element_id, time_offset);
                factory.add_ramp(std::move(ramp));
                break;
            }
            //jakas kraksa chyba czekam az skonczycie moze sie naprawi
            case ElementType::WORKER:{
                ElementID element_id = std::stoi(parsed_line.parameters.at("id"));
                TimeOffset time_offset = std::stoi(parsed_line.parameters.at("processing-time"));
                PackageQueueType package_queue_type = std::stoi(parsed_line.parameters.at("queue-type"));
                Worker worker(element_id, time_offset, std::make_unique<PackageQueue>(package_queue_type));
                factory.add_worker(std::move(worker));
                break;
            }
            //jakas kraksa again, ta sama wlasciwie
            case ElementType::STOREHOUSE:{
                ElementID element_id = std::stoi(parsed_line.parameters.at("id"));
                Storehouse storehouse(element_id)
                factory.add_storehouse(std::move(storehouse));
                break;
            }
            //a tego nie ma lols
            case ElementType::LINK:{
                link(factory, parsed_line.parameters);
                break;
            }
        }
    }
    return factory;
}
