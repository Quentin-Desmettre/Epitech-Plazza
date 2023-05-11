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
#include "pizza/Pizza.hpp"

class CookPool {
public:
    /**
     * @brief Creates a cook pool.
     *
     * Launches a thread that will watch the queue for pizzas to cook.
     * @param cooks
     * @param multiplier
     */
    CookPool(int cooks, float multiplier);

    /**
     * @brief Puts the requested pizza in a queue.
     */
    void addPizza(const Pizza &pizza);

    /**
     * @brief Cooks a pizza.
     *
     * Waits the time needed to cook the pizza, depending on the multiplier.
     * When this method has finished:
     *  - sem_post
     *  - add the pizza in the list of finished pizzas
     *  - signal the conditional variable in the Kitchen.
     */
    void cookPizza(const Pizza &pizza);

    /**
     * @brief Returns the number of pizzas in cooking.
     * @return _cooks - _cookSemaphore.size() + _queue.size()
     */
    int getPizzaInCooking() const;

    /**
     * @brief Clear the list of finished pizzas.
     * @return A vector of cooked pizzas.
     */
    [[nodiscard]] std::vector<Pizza> clearFinishedPizzas();

private:
    /**
     * @brief Watches the queue for pizzas to cook.
     *
     * If a pizza has to be cooked, tries to sem_wait on the cook semaphore,
     * then launch a thread that will cook the pizza.
     */
    void watchQueue();

    std::queue<Pizza> _queue;
    std::counting_semaphore<INT32_MAX> _cookSemaphore;
    std::vector<Pizza> _finishedPizzas;
    const int _cooks;
    const float _multiplier;
};

#endif //EPITECH_PLAZZA_COOKPOOL_HPP
