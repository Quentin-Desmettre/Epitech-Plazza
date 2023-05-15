/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CookPool
*/

#include "CookPool.hpp"

CookPool::CookPool(int cooks, float multiplier) : _multiplier(multiplier), _cooks(cooks),
    _mutex(), _pizzaFinished(), _queue(), _finishedPizzas(), _pizzasToCook(0)
{
    for (int i = 0; i < cooks; i++)
        _cookers.emplace_back(&CookPool::cookThread, this);
}

void CookPool::addPizza(const Pizza &pizza)
{
    _queue.push(pizza);
    _pizzasToCook.post();
}

void CookPool::cookThread()
{
    while (true) {
        _pizzasToCook.wait(_multiplier);
        auto pizza = _queue.front();
        _queue.pop();
        std::this_thread::sleep_for(std::chrono::milliseconds(pizza.getCookingTime() * _multiplier));
        _finishedPizzas.push_back(pizza);
        _pizzaFinished.notify_one();
    }
}

int CookPool::getPizzaInCooking() const
{
    return _queue.size();
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
