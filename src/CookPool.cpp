/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CookPool
*/

#include "CookPool.hpp"

#include <iostream>
CookPool::CookPool(int cooks, float multiplier) : _pizzasToCook(0), _pizzaInCooking(0),
    _cooks(cooks), _multiplier(multiplier)
{
    for (int i = 0; i < _cooks; i++)
        _cookers.emplace_back(&CookPool::cookThread, this);
}

void CookPool::addPizza(const Pizza &pizza)
{
    std::cout << "Adding pizza to cook" << std::endl;
    _queue.push(pizza);
    std::cout << "Adding pizza to cook 1" << std::endl;
    _pizzasToCook.release();
    std::cout << "Adding pizza to cook 2" << std::endl;
}

void CookPool::cookThread()
{
    while (true) {
        std::cout << "Waiting for pizza" << std::endl;
        _pizzasToCook.acquire();
        std::cout << "Waiting for pizza 0.5" << std::endl;
        _pizzaInCookingMutex.lock();
        std::cout << "Waiting for pizza 1" << std::endl;
        auto pizza = _queue.front();
        _queue.pop();
        _pizzaInCooking++;
        std::cout << "Waiting for pizza 2" << std::endl;
        _pizzaInCookingMutex.unlock();
        std::cout << "Cooking pizza " << pizza.getType() << std::endl;

        unsigned long test = pizza.getCookTime() * _multiplier * 1000;
        std::this_thread::sleep_for(std::chrono::nanoseconds(test));
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
