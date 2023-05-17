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

class CookPool {
public:
    /**
     * @brief Creates a cook pool.
     *
     * Launches the cook threads, each one waiting on a semaphore.
     * @param cooks The number of cooks.
     * @param multiplier The multiplier to apply to the cooking time.
     */
    CookPool(int cooks, float multiplier);

    /**
     * @brief Puts the requested pizza in a queue, then post on the semaphore.
     */
    void addPizza(Pizza pizza);

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

    /**
     * @brief Clear the list of finished pizzas.
     * @return A vector of cooked pizzas.
     */
    [[nodiscard]] std::vector<Pizza> clearFinishedPizzas();

    /**
     * @brief Waits for a pizza to be finished.
     *
     * It achieves this by waiting on the conditional variable.
     */
    void waitPizzaFinished();

private:
    std::queue<Pizza> _queue;
    std::counting_semaphore<INT32_MAX> _pizzasToCook;
    std::vector<Pizza> _finishedPizzas;
    std::vector<std::thread> _cookers;
    std::condition_variable _pizzaFinished;
    std::mutex _mutex;
    Semaphore _semaphore;

    std::mutex _pizzaInCookingMutex;

    int _pizzaInCooking;
    const int _cooks;
    const float _multiplier;
};

#endif //EPITECH_PLAZZA_COOKPOOL_HPP
