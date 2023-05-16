/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CookPool
*/

#include "CookPool.hpp"
#include <iostream>
#include <cstring>

CookPool::CookPool(int cooks, float multiplier) : _pizzasToCook(0), _pizzaInCooking(0),
    _cooks(cooks), _multiplier(multiplier), _semaphore(0)
{
    for (int i = 0; i < _cooks; i++)
        _cookers.emplace_back(&CookPool::cookThread, this);
}

void CookPool::addPizza(const Pizza &pizza)
{
    _pizzaInCookingMutex.lock();
    _queue.push(pizza);
    _semaphore.increment();
    _pizzaInCookingMutex.unlock();
}

void CookPool::cookThread()
{
    while (true) {
        _semaphore.decrement();

        _pizzaInCookingMutex.lock();
        auto pizza = _queue.front();
        _queue.pop();
        _pizzaInCooking++;
        _pizzaInCookingMutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds((unsigned)(pizza.getCookTime() * _multiplier * 1000)));

        _pizzaInCookingMutex.lock();
        _pizzaInCooking--;
        _finishedPizzas.push_back(pizza);
        _pizzaFinished.notify_one();
        _pizzaInCookingMutex.unlock();
    }
}

int CookPool::getPizzaInCooking() const
{
    return _queue.size() + _pizzaInCooking;
}

std::vector<Pizza> CookPool::clearFinishedPizzas()
{
    std::vector<Pizza> pizzas;
    pizzas.swap(_finishedPizzas);
    return pizzas;
}

void CookPool::waitPizzaFinished()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _pizzaFinished.wait(lock);
}
