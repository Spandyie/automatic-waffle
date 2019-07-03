#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <limits>
#include <iterator>
#include "GeneticAlgorithm.h"


int main() {
    double max_value;
    std::vector<double> arg_max_values;
    int t_num_generations = 10000;
    auto t_pop_size = std::make_pair(8, 6);
    std::vector<double> t_equation_param{4,-2,3.5,5,-11,-4.7};
    GeneticAlgorithm geneticAlgoObject(t_pop_size, t_equation_param, t_num_generations);
    std::tie(max_value, arg_max_values) = geneticAlgoObject.run();
}