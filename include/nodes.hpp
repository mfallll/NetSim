#ifndef NODES_HPP_
#define NODES_HPP_

#include "config.hpp"

#include "types.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"
#include <memory>
#include <map>
#include <optional>

class IPackageReceiver {
    public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ~IPackageReceiver() = default;
    //dodana część z 'poprawy' zadania, ale idk kiedy mamy to faktycznie zrobić? nie podoba mi się to, że to zadanie ma fabułę XDDDDDDD

    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        virtual ReceiverType get_receiver_type()  const = 0;
    #endif
};

class ReceiverPreferences {
    public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : probabilty_(pg) {};
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    const preferences_t& get_preferences() const { return preferences_; }

    //iteratory
    const_iterator begin() const { return preferences_.cbegin(); }
    const_iterator end() const { return preferences_.cend(); }
    const_iterator cbegin() const { return preferences_.cbegin(); }
    const_iterator cend() const { return preferences_.cend(); }

    private:
    preferences_t preferences_;
    ProbabilityGenerator probabilty_;
};

class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;
    PackageSender() = default;
    PackageSender(PackageSender &&package_sender) = default;
    void send_package();
    const std::optional<Package>& get_sending_buffer() const { return buffer_; }
protected:
    void push_package(Package &&package) { buffer_.emplace(package.get_id()); }
private:
    std::optional<Package> buffer_ = std::nullopt;
};

class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset di) : PackageSender(), id_(id), di_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const { return di_; }
    ElementID get_id() const { return id_; }
private:
    ElementID id_;
    TimeOffset di_;
    std::optional<Package> buffer_ = std::nullopt;
    Time t_;
};

class Worker : public IPackageReceiver, public PackageSender{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);

    void receive_package(Package&& p);
    ElementID get_id() const override {return id_; };
    void do_work(Time t);
    TimeOffset get_processing_duration() const {return pd_;}
    Time get_package_processing_start_time();

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
};

class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) : id_(id), d_(std::move(d)) {}
    void receive_package(Package&& p) override;
    ElementID get_id() const override {return id_; }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};
#endif //NODES_HPP_