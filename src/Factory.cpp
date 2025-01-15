#include "Factory.hpp"
#include <exception>
#include <vector>
#include <string>
#include <sstream>

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

Factory load_factory_structure(std::istream& is){
    Factory factory;
    std::string line;
    while (std::getline(is, line)) {
        if (line.empty() or line[0] == ';'){
            continue;
        }
    }

    //TO DO
}
