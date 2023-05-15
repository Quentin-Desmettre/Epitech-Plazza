/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CookPool
*/

#include "CookPool.hpp"

CookPool::CookPool(int cooks, float multiplier) : _multiplier(multiplier), _cooks(cooks),
    _mutex(), _pizzaFinished(), _queue(), _finishedPizzas(), _pizzasToCook(0), _pizzaInCooking(0), _pizzaInCookingMutex()
{
    for (int i = 0; i < cooks; i++)
        _cookers.emplace_back(&CookPool::cookThread, this);
}

void CookPool::addPizza(const Pizza &pizza)
{
    _queue.push(pizza);
    _pizzasToCook.release();
}

void CookPool::cookThread()
{
    while (true) {
        _pizzasToCook.acquire();


        _pizzaInCookingMutex.lock();
        auto pizza = _queue.front();
        _queue.pop();
        _pizzaInCooking++;
        _pizzaInCookingMutex.unlock();


        std::this_thread::sleep_for(std::chrono::milliseconds(pizza.getCookingTime() * _multiplier));
        //TODO: Quentin tu suces ?


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
