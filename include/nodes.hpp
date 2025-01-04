#ifndef NODES_HPP_
#define NODES_HPP_

#include "config.hpp"

#include "types.hpp"
#include "package.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"

class IPackageReceiver {
    public:
    virtual void receive_package(Package&& p) = 0;
    virtual void get_id() const = 0;
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

#endif //NODES_HPP_