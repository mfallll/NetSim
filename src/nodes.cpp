#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver *r){
    int size = preferences_.size();
    if (size == 0){
        preferences_[r] = 1;
    }
    else{
        for (auto &pref : preferences_){
            pref.second = 1/(size+1);
        }
    preferences_[r] = 1/(size+1);
    }

}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r){
    int size = preferences_.size();
    if (size >1){
        for (auto &pref : preferences_){
            pref.second = 1/(size-1);
        }
    }
    preferences_.erase(r);
}

IPackageReceiver* ReceiverPreferences::choose_receiver(){
    auto probability = probabilty_;
    if (probability>=0 && probability<=1){ // Tutaj trzeba poprawić: probability - std::functional, a 0 to int
        double distribution = 0.00;
        for (auto &pref : preferences_){
            distribution +=pref.second;
            if (probability <= distribution){
                return pref.first;
            }
        }
    }
    return nullptr;
}

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) :
id_(id),
pd_(pd),
q_(std::move(q))
{

}

void Worker::do_work(Time t) {

}

Time Worker::get_package_processing_start_time() {
    return 0;
}

void Worker::receive_package(Package &&p) {

}


void PackageSender::send_package() {
    IPackageReceiver * receiver;
    if (buffer_) {
        receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(*buffer_));
        buffer_.reset();
    }
}

void Ramp::deliver_goods(Time t) {
    if (!buffer_) {
        push_package(Package());
        buffer_.emplace(id_);
        t_ = t;
    }
    else if (t - di_ == t_) {
        push_package(Package());
    }
}

void Storehouse::receive_package(Package &&p) {

}
