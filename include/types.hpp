#ifndef TYPES_HPP
#define TYPES_HPP

#include <functional>

using ElementID = int;

/*idk jakiego typu ma być czas tutaj, można poprawić*/
using Time = int;
using TimeOffset = int;

using ProbabilityGenerator = std::function<double()>;

#endif //TYPES_HPP