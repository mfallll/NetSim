#include "nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver *r){
    unsigned long long size = preferences_.size();
    if (size == 0){
        preferences_[r] = 1;
    }
    else{
        for (auto &pref : preferences_){
            pref.second = (double) 1/(size+1) ;
        }
        preferences_[r] = (double) 1/(size+1);
    }

}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r){
    std::size_t size = preferences_.size();
    if (size >1){
        for (auto &pref : preferences_){
            pref.second = (double)1/(size-1);
        }
    }
    preferences_.erase(r);
}

IPackageReceiver* ReceiverPreferences::choose_receiver(){
    auto probability = probabilty_();
    if (probability >= 0 && probability <= 1){
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
{}

void Worker::do_work(Time t) {
    if(!get_sending_buffer().has_value()){
        if(!q_->empty()){
            push_package(std::move(q_->pop()));
            t_ = t;

        }
    }else{
        if(t >= t_+ pd_ ) {
            send_package();
            if(!q_->empty()){
                push_package(std::move(q_->pop()));
                t_ = t;
            }
        }
    }
}

Time Worker::get_package_processing_start_time() {
    return t_;
}



void PackageSender::send_package() {
    IPackageReceiver * receiver;
    if (buffer_.has_value()) {
        receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(buffer_.value()));
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
    d_->push(std::move(p));
}
