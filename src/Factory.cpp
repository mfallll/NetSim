#include "Factory.hpp"
//#include <unique>
#include <vector>

std::string queue_type_to_str(PackageQueueType package_queue_type){
    switch(package_queue_type){
        case PackageQueueType::LIFO: {
            return "LIFO";
        }
        case PackageQueueType::FIFO: {
            return "FIFO";
        }
    }
    return "";
}

std::string receiver_type_to_str(ReceiverType receiverType){
    switch(receiverType){
        case ReceiverType::WORKER:
            return "worker";
        case ReceiverType::STOREHOUSE:
            return "store";
    }
    return "";
}

PackageQueueType str_to_queue_type(std::string& str) {
    if (str == "LIFO"){
        return PackageQueueType::LIFO;
    }
    if (str == "FIFO"){
        return PackageQueueType::FIFO;
    }
    return {};
}

bool Factory::is_consistent() {
    /* Użyj algorytmu DFS do przejścia grafu, dla każdej rampy. */

//    kolor := mapowanie {PackageSender* -> kolor_węzła_enum}
//    dla wszystkich ramp i dla wszystkich robotników jako `węzeł`:
//    kolor[węzeł] = NIEODWIEDZONY

    std::map<const PackageSender *, NodeColor> node_colors;

    for(const auto& i : RampCont){
        node_colors[&i] = NodeColor::UNVISITED;
    }
    for(const auto& i : WorkerCont){
        node_colors[&i] = NodeColor::UNVISITED;
    }

    //
//    jeśli poniższa instrukcja rzuci wyjątek, zwróć FAŁSZ:  /* tj. sieć nie jest spójna */
//    dla każdej rampy `rampa` w kolekcji wszystkich ramp w sieci:
//    czy_nadawca_posiada_osiągalny_magazyn(rampa, kolor)
//    w przeciwnym razie:
//    zwróć PRAWDA  /* tj. sieć jest spójna */ xd
    // TODO catch wyjatki
    for(const auto& i : RampCont){
        if(!this->has_reachable_storehouse(&i, node_colors)) {
            return false;
        }
    }

    return true;
}

void Factory::do_deliveries(Time t) {
    for (auto &ramp : RampCont) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_package_passing() {
    for (auto &ramp : RampCont) {
        ramp.send_package();
    }
    for (auto &worker : WorkerCont) {
        worker.send_package();
    }
}

void Factory::do_work(Time t) {
    for (auto &worker : WorkerCont) {
        worker.do_work(t);
    }
}

bool Factory::has_reachable_storehouse(const PackageSender *sender,
                                       std::map<const PackageSender *, NodeColor> &node_colors) {

    bool has_not_self_receiver = false;

    switch(node_colors[sender]) {
        case NodeColor::VERIFIED:
            return true;
        default:
            node_colors[sender] = NodeColor::VISITED;
            if (sender->receiver_preferences_.get_preferences().empty()) {
                return false;
            }

            for (const auto &i: sender->receiver_preferences_.get_preferences()) {
                if (i.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
                    has_not_self_receiver = true;
                } else if (i.first->get_receiver_type() == ReceiverType::WORKER) {
                    IPackageReceiver *receiver_ptr = i.first;
                    auto worker_ptr = dynamic_cast<Worker *>(receiver_ptr);
                    auto sendrecv_ptr = dynamic_cast<PackageSender *>(worker_ptr);

                    if (sendrecv_ptr == sender) {
                        has_not_self_receiver = true;
                    }
                    bool ret;
                    if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED) {
                        ret = has_reachable_storehouse(sendrecv_ptr, node_colors);
                    }
                    node_colors[sender] = NodeColor::VERIFIED;
                    if (ret) {
                        return true;
                    } else {
                        return false;
                    }
                }
            }

    }
    return has_not_self_receiver;
}

void Factory::remove_worker(ElementID id) {
    for(auto& ramp : RampCont){
        ramp.receiver_preferences_.remove_receiver(&(*find_worker_by_id(id)));
    }
    for(auto& worker : WorkerCont){
        worker.receiver_preferences_.remove_receiver(&(*find_worker_by_id(id)));
    }

    WorkerCont.remove_by_id(id);
}

std::vector<std::string> split(const std::string& str, char delimiter) {
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
        std::for_each(std::next(tokens.begin()), tokens.end(), [&](const std::string& str){
            std::vector<std::string> value = split(str, '=');
            parsed_line_data.parameters[value[0]] = value[1];
        });
    } catch( std::out_of_range& e) { throw std::exception(); }

    return parsed_line_data;
}

void link(Factory& factory, const std::map<std::string, std::string>& parameters) {
    enum class NodeType{
        RAMP, WORKER, STORE
    };

    std::map<std::string, NodeType> str_to_node_type{
            {"ramp", NodeType::RAMP},
            {"worker", NodeType::WORKER},
            {"store", NodeType::STORE}
    };

    std::string src = parameters.at("src");
    std::string dest = parameters.at("dest");

    auto src_parameters = split(src, '-');
    NodeType src_node_type = str_to_node_type.at(src_parameters[0]);
    ElementID src_element_id = std::stoi(src_parameters[1]);

    auto dest_parameters = split(dest, '-');
    NodeType dest_node_type = str_to_node_type.at(dest_parameters[0]);
    ElementID dest_element_id = std::stoi(dest_parameters[1]);

    IPackageReceiver* package_receiver = nullptr;

    switch (dest_node_type) {
        case NodeType::RAMP:
            break;
        case NodeType::WORKER:
            package_receiver = &*factory.find_worker_by_id(dest_element_id);
            break;
        case NodeType::STORE:
            package_receiver = &*factory.find_storehouse_by_id(dest_element_id);
            break;
    }

    switch (src_node_type) {
        case NodeType::RAMP:
            factory.find_ramp_by_id(src_element_id)->receiver_preferences_.add_receiver(package_receiver);
            break;
        case NodeType::WORKER:
            factory.find_worker_by_id(src_element_id)->receiver_preferences_.add_receiver(package_receiver);
            break;
        case NodeType::STORE:
            break;
    }
}

Factory load_factory_structure(std::istream& is){
    Factory factory;
    std::string line;
    while (std::getline(is, line)) {
        if (line.empty() or line[0] == ';'){
            continue;
        }
        ParsedLineData parsed_line = parse_line(line);
        int element_id;
        TimeOffset time_offset;
        PackageQueueType type;

        switch(parsed_line.element_type){
            case ElementType::RAMP:
                element_id = std::stoi(parsed_line.parameters.at("id"));
                time_offset = std::stoi(parsed_line.parameters.at("delivery-interval"));
                factory.add_ramp(std::move(Ramp(element_id, time_offset)));
                break;

            case ElementType::WORKER:
                element_id = std::stoi(parsed_line.parameters.at("id"));
                time_offset = std::stoi(parsed_line.parameters.at("processing-time"));
                type = str_to_queue_type(parsed_line.parameters.at("queue-type"));
                factory.add_worker(std::move(Worker(element_id, time_offset, std::make_unique<PackageQueue>(type))));
                break;

            case ElementType::STOREHOUSE:
                element_id = std::stoi(parsed_line.parameters.at("id"));
                factory.add_storehouse(std::move(Storehouse(element_id)));
                break;

            case ElementType::LINK:
                link(factory, parsed_line.parameters);
                break;
        }
    }
    return factory;
}

void link_to_str(std::stringstream& str, const PackageSender& sender, ElementID id, std::string sender_type){
    auto preferences = sender.receiver_preferences_.get_preferences();
    std::for_each(preferences.cbegin(), preferences.cend(), [&](const std::pair<IPackageReceiver*, double>& key_value) {
        str << "LINK src=" << sender_type << "-" << id << " ";
        const IPackageReceiver* receiver = key_value.first;
        ReceiverType receiver_type = receiver->get_receiver_type();

        std::string receiver_type_str = receiver_type_to_str(receiver_type);

        str << "dest=" << receiver_type_str << "-" << receiver->get_id() << '\n';
        std::cout << str.str();
    });

}

void save_factory_structure(Factory& factory, std::ostream& os){
    std::stringstream str;
    //RAMP
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), [&](const Ramp& ramp) {
        ElementID id_ramp = ramp.get_id();
        os << "LOADING_RAMP id=" << id_ramp << ' '
           << "delivery-interval=" << ramp.get_delivery_interval() << '\n';

        link_to_str(str, ramp, id_ramp, "ramp");
    });

    //WORKER
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), [&](const Worker& worker){
        ElementID id_worker = worker.get_id();
        TimeOffset processing_duration = worker.get_processing_duration();
        PackageQueueType package_queue_type = worker.get_queue()->get_queue_type();

        os <<"WORKER id="<<id_worker<<' '<<
           "processing-time="<<processing_duration<<' '
           <<"queue-type="<<queue_type_to_str(package_queue_type)<<'\n';

        link_to_str(str, worker, id_worker, "worker");

    });

    //STOREHOUSE
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), [&](const Storehouse& storehouse){
        ElementID  id_storehouse = storehouse.get_id();
        os <<"STOREHOUSE id="<<id_storehouse<<'\n';

    });

    //LINK

    os << str.str();
    os.flush();


}