#ifndef TYPES_HPP
#define TYPES_HPP

#include <functional>

using ElementID = int;
using Time = double;
using TimeOffset = double;

using ProbabilityGenerator = std::function<double()>;

#endif //TYPES_HPP