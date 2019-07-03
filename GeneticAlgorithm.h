//
// Created by spandan on 7/3/19.
//

#ifndef GENETICALGORITHM_GENETICALGORITHM_H
#define GENETICALGORITHM_GENETICALGORITHM_H
#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <limits>
#include <iterator>
#include <tuple>

std::vector<std::vector<double>> generate_uniform_random_vector(std::pair<int, int> shape);

class GeneticAlgorithm{
private:
    int m_num_generations{0};
    int num_parents;
    std::pair<int, int> m_pop_size, m_offspring_size;
    std::vector<double> m_equation_param;
    std::vector<double>  calc_pop_fitness(const std::vector<std::vector<double>>& population);
    std::vector<std::vector<double>> select_mating_pool(const std::vector<std::vector<double>>& population, std::vector<double>& fitness);
    std::vector<std::vector<double>> cross_over(std::vector<std::vector<double>>& parents, std::pair<int, int>& offspring_size);
    void mutation(std::vector<std::vector<double>>& children, int num_mutation);


public:
    GeneticAlgorithm(std::pair<int, int>& t_pop_size, std::vector<double>& t_equation_param, int t_num_generations ): m_pop_size(t_pop_size),m_equation_param(t_equation_param), m_num_generations(t_num_generations){
        num_parents = 2;
    }
    std::tuple<double, std::vector<double>> run();
    static int find_max(const std::vector<double>& input_vector);
};



#endif //GENETICALGORITHM_GENETICALGORITHM_H
