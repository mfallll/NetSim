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
#include <functional>

enum class ReceiverType {
    WORKER,
    STOREHOUSE
};

class IPackageReceiver {
    public:
    //IPackageReceiver(IPackageReceiver& p) = default;
//    IPackageReceiver(IPackageReceiver&& p) = default;
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;


    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;

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

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : probabilty_(std::move(pg)) {};
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
    Ramp(Ramp &&ramp) : id_(ramp.id_), di_(ramp.di_), buffer_(std::move(ramp.buffer_)), t_(ramp.t_) {}
    Ramp(Ramp&) = delete; // Konstruktor kopiujący usunięty
    Ramp& operator=(Ramp&) = delete; // Operator przypisania również usunięty
    ~Ramp() = default;
    //Ramp(const Ramp& ramp) : id_(ramp.id_), di_(ramp.di_), buffer_(ramp.buffer_), t_(ramp.t_) {}

//Ramp(Ramp &ramp) = default;
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
    Worker(Worker& wrk) = delete;
    Worker(Worker&& wrk) : id_(std::move(wrk.id_)), pd_(std::move(wrk.pd_)), q_(std::move(wrk.q_)), t_(std::move(wrk.t_)) {}


    void receive_package(Package &&p) override {q_->push(std::move(p));}
    [[nodiscard]] ElementID get_id() const override {return id_; };
    void do_work(Time t);
    [[nodiscard]] TimeOffset get_processing_duration() const {return pd_;}
    Time get_package_processing_start_time();

    IPackageQueue* get_queue(void) const {return q_.get(); };

    IPackageStockpile::const_iterator cbegin() const override {return q_->cbegin(); }
    IPackageStockpile::const_iterator cend() const override {return q_->cend(); }
    IPackageStockpile::const_iterator begin() const override {return q_->begin(); }
    IPackageStockpile::const_iterator end() const override {return q_->end(); }
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        ReceiverType get_receiver_type() const { return ReceiverType::WORKER; };
    #endif

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    Time t_ = 0;
};

class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id)  : id_(id), d_(new PackageQueue(PackageQueueType::FIFO)) {}
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) : id_(id), d_(std::move(d)) {}
    void receive_package(Package&& p) override;
    [[nodiscard]] ElementID get_id() const override {return id_; }

    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin(); }
    IPackageStockpile::const_iterator cend() const override {return d_->cend(); }
    IPackageStockpile::const_iterator begin() const override {return d_->begin(); }
    IPackageStockpile::const_iterator end() const override {return d_->end(); }
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
        ReceiverType get_receiver_type() const { return ReceiverType::STOREHOUSE; };
    #endif
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};
#endif //NODES_HPP_