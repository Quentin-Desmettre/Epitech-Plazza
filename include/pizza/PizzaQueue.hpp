/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaQueue
*/

#ifndef EPITECH_PLAZZA_PIZZAQUEUE_HPP
#define EPITECH_PLAZZA_PIZZAQUEUE_HPP
#include "pizza/Pizza.hpp"
#include "Semaphore.hpp"
#include "ConditionalVariable.hpp"
#include <mutex>
#include <queue>

class PizzaQueue {
public:
    PizzaQueue();
    ~PizzaQueue() = default;

    /**
     * @brief Atomically pushes a pizza to the queue
     * @param pizza
     */
    void push(const Pizza &pizza);

    /**
     * @brief Tries to pop a pizza, and block until one is available
     * @return The pizza
     */
    Pizza pop();

    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;

private:
    ConditionalVariable<std::queue<Pizza>> _queue;
    Semaphore _queueSize;
};

#endif //EPITECH_PLAZZA_PIZZAQUEUE_HPP
