/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CookPool
*/

#include "CookPool.hpp"
#include "logging/ILogger.hpp"
#include <iostream>
#include "Kitchen.hpp"

CookPool::CookPool(int cooks, float multiplier, const Kitchen &kitchen) : _pizzaInCooking(0),
                                                  _cooks(cooks), _multiplier(multiplier), _queuedPizzaSemaphore(0),
                                                  _kitchen(kitchen)
{
    for (int i = 0; i < _cooks; i++)
        _cookers.emplace_back(&CookPool::cookThread, this);
    for (int i = 0; i < Pizza::Ingredient::IngredientCount; i++)
        _ingredients[Pizza::Ingredient(i)] = Semaphore(5);
}

void CookPool::addPizza(Pizza pizza)
{
    _pizzaInCookingMutex.lock();
    _queue.push(pizza);
    _queuedPizzaSemaphore.increment();
    _pizzaInCookingMutex.unlock();
}

void CookPool::cookThread()
{
    while (true) {
        _queuedPizzaSemaphore.decrement();

        _pizzaInCookingMutex.lock();
        auto pizza = _queue.front();
        _queue.pop();
        _pizzaInCooking++;
        _pizzaInCookingMutex.unlock();
        waitForIngredients(pizza);

        ILogger::getLogger().logPizzaCookingStarted(_kitchen.getId(), pizza);
        std::this_thread::sleep_for(std::chrono::milliseconds((unsigned)(pizza.getCookTime() * _multiplier * 1000)));
        ILogger::getLogger().logPizzaCooked(_kitchen.getId(), pizza);

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
    if (_finishedPizzas.empty())
        return {};

    std::vector<Pizza> pizzas;
    pizzas.swap(_finishedPizzas);
    return pizzas;
}

void CookPool::waitPizzaFinished()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _pizzaFinished.wait(lock);
}

CookPool::Ingredients *CookPool::getIngredients()
{
    return &_ingredients;
}

void CookPool::waitForIngredients(const Pizza &pizza)
{
    for (auto &ingredient : pizza.getIngredients())
        _ingredients[ingredient].decrement();
}
