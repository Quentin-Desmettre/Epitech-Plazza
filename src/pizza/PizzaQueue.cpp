/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaQueue
*/

#include "pizza/PizzaQueue.hpp"

PizzaQueue::PizzaQueue():
    _queueSize(0)
{
}

void PizzaQueue::push(const Pizza &pizza)
{
    auto queue = _queue.wait();
    _queueSize.increment();
    queue->push(pizza);
    _queue.signal();
}

Pizza PizzaQueue::pop()
{
    // Wait until there is a pizza to pop
    _queueSize.decrement();

    // Get the lock and to the operations
    auto queue = _queue.wait();
    auto pizza = queue->front();
    queue->pop();
    _queue.signal();
    return pizza;
}

bool PizzaQueue::empty() const
{
    return _queueSize.getValue() == 0;
}

size_t PizzaQueue::size() const
{
    return _queueSize.getValue();
}
