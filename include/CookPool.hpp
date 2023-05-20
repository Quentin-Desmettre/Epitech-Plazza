/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CookPool
*/

#ifndef EPITECH_PLAZZA_COOKPOOL_HPP
#define EPITECH_PLAZZA_COOKPOOL_HPP
#include <queue>
#include <semaphore>
#include <condition_variable>
#include "pizza/Pizza.hpp"
#include <thread>
#include "Semaphore.hpp"
#include "pizza/PizzaQueue.hpp"

class Kitchen;

class CookPool {
public:
    /**
     * @brief Creates a cook pool.
     *
     * Launches the cook threads, each one waiting on a semaphore.
     * @param cooks The number of cooks.
     * @param multiplier The multiplier to apply to the cooking time.
     */
    CookPool(int cooks, float multiplier, const Kitchen &kitchen);

    /**
     * @brief Puts the requested pizza in a queue, then post on the semaphore.
     */
    void addPizza(const Pizza& pizza);

    /**
     * @brief The cook thread.
     *
     * Each cook is represented by a thread, that runs this method.
     * This method execute the following steps:
     *  - Wait on the semaphore
     *  - pop a pizza from the queue (because if it could wait on the semaphore, it means that there is a pizza to cook)
     *  - wait the cooking time
     *  - push the pizza in the finished pizzas list
     *  - signals the conditional variable
     */
    void cookThread();

    /**
     * @brief Returns the number of pizzas in cooking.
     * @return The number of pizzas in cooking.
     */
    int getPizzaInCooking() const;

    using Ingredients = std::map<Pizza::Ingredient, Semaphore>;

    Ingredients *getIngredients();

    void consumeIngredients(const Pizza &pizza);

    PizzaQueue &getFinishedPizzas();

private:
    PizzaQueue _queue;
    PizzaQueue _finishedPizzas;
    std::vector<std::thread> _cookers;

    mutable ConditionalVariable<int> _pizzaInCooking;
    const int _cooks;
    const float _multiplier;
    Ingredients _ingredients;
    const Kitchen &_kitchen;
};

#endif //EPITECH_PLAZZA_COOKPOOL_HPP
