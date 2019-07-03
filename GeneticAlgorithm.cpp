//
// Created by spandan on 7/3/19.
//

#include "GeneticAlgorithm.h"

std::vector<std::vector<double>> generate_uniform_random_vector(std::pair<int, int> shape){
    std::vector<std::vector<double>> random_vector;
    std::random_device rnd_device;
    std::mt19937 engine{rnd_device()};
    std::uniform_real_distribution<double> dist_0_1(-4,4);
    for(auto i=0; i < shape.first ; ++i ){
        std::vector<double> temp(shape.second);
        for(auto j =0; j < shape.second; ++j){
            temp[j] = dist_0_1(engine);
        }
        random_vector.emplace_back(temp);
    }
    return random_vector;
}




std::vector<double> GeneticAlgorithm::calc_pop_fitness(const std::vector<std::vector<double>>& population) {
    std::vector<double> fitness_vector;
    fitness_vector.reserve(population.size());
    for(auto& temp_vector : population){
        auto fit = std::inner_product(m_equation_param.begin(), m_equation_param.end(), temp_vector.begin(), 0.0);
        fitness_vector.emplace_back(fit);
    }
    return fitness_vector;
}

std::vector<std::vector<double>> GeneticAlgorithm::select_mating_pool(const std::vector<std::vector<double>>& population,
                                                                      std::vector<double> &fitness) {
    /* Arguments
     * population: population vector( initial is randomly generated)
     * fitness : the result of the calc_pop_fitness function
     */
    std::vector<std::vector<double>> parent_vector;
    parent_vector.reserve(num_parents);
    for(int i=0; i < num_parents;++i){
        auto max_index = GeneticAlgorithm::find_max(fitness);         // find the index of the max fitness values
        auto parent = population.at(max_index);                       // uses the index to find the best population vector
        parent_vector.emplace_back(parent);
        fitness[max_index] = std::numeric_limits<int>::min();         // next iteration will find next highest,
        // assign smallest value possible to the detected best population
    }

    return parent_vector;
}

std::vector<std::vector<double>> GeneticAlgorithm::cross_over(std::vector<std::vector<double>>& parents, std::pair<int, int>& offspring_size){
    /*
     * parents: vector consisting of the parent array
     * offspring_size (number of offspring, size of each offspring)
     * This function does does the evolutionary changes using the parent genes.
     */
    std::vector<std::vector<double>> offsprings;
    int crossover_point = offspring_size.second / 2;
    for(int i=0; i < offspring_size.first; ++i){
        int parent_ind1 = i % (int) parents.size();
        int parent_ind2 = (i+1) % (int) parents.size();
        // copying from 0 to cross-over point of parent into first half of off-spring
        std::vector<double> offspring1(parents[parent_ind1].begin(), parents[parent_ind1].begin()+crossover_point);
        //copying from cross-over point to end into second half of offspring
        std::vector<double> offspring2(parents[parent_ind2].begin()+crossover_point, parents[parent_ind2].end());

        offspring1.insert(offspring1.end(), std::make_move_iterator(offspring2.begin()), std::make_move_iterator(offspring2.end()));
        offspring2.clear();
        offsprings.emplace_back(offspring1);
    }
    return offsprings;
}

void GeneticAlgorithm::mutation(std::vector<std::vector<double>>& children, int num_mutation) {
    /*
     * Randomly change the genes of the offspring
     */
    int n = children[0].size() -1;
    std::random_device rnd_dvc;
    std::mt19937 engine{rnd_dvc()};
    std::uniform_real_distribution dist(1.0);
    std::uniform_int_distribution idxDist(0,n);
    for(size_t idx=0; idx < children.size(); ++idx){
        for(size_t jMutation=0; jMutation < num_mutation; ++jMutation){
            int index_2mutate = idxDist(engine);
            children[idx][index_2mutate] += dist(engine);
        }
    }
}

int GeneticAlgorithm::find_max(const std::vector<double>& input_vector){
    auto max_itr = std::max_element(input_vector.begin(), input_vector.end());
    int max_index = std::distance(input_vector.begin(), max_itr);
    return max_index;
}


void GeneticAlgorithm::run(){
    // equation = w1 x1 + w2 x2 + w3 x3;
    auto new_population = generate_uniform_random_vector(m_pop_size);
    int m_number_of_offspring = m_pop_size.first - num_parents;
    m_offspring_size = std::make_pair(m_number_of_offspring, m_equation_param.size());
    for(int i=0; i < m_num_generations;++i){
        auto fitness = calc_pop_fitness(new_population);
        // print the fitness
        auto idx_max_fitness = find_max(fitness);
        auto max_fitness_value =  std::max_element(fitness.begin(), fitness.end());
        std::cout << "the value of fitness function is: " << *max_fitness_value << "\n";
        std::for_each(new_population.at(idx_max_fitness).begin(), new_population.at(idx_max_fitness).end(), [](auto& val){std::cout << val <<" ";});
        std::cout << "\n";
        auto new_parents = select_mating_pool(new_population, fitness);
        // do crossover
        auto crossover_mutated_offspring = cross_over(new_parents, m_offspring_size);
        // do mutation
        mutation(crossover_mutated_offspring,3);
        auto len_new_parents = new_parents.size();
        // create a new population by combing new_parents and offsprings
        auto position1 = new_population.begin();
        auto position2 = new_population.begin() + len_new_parents;
        new_population.clear();                                                      // clear the population vector before adding values
        new_population.insert(position1, std::make_move_iterator(new_parents.begin()), std::make_move_iterator(new_parents.end()));
        new_population.insert(position2, std::make_move_iterator(crossover_mutated_offspring.begin()), std::make_move_iterator(crossover_mutated_offspring.end()));

    }

}