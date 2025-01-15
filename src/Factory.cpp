#include "Factory.hpp"


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
    }

}
