/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CookPool
*/

#include "CookPool.hpp"
#include "logging/ILogger.hpp"
#include "Kitchen.hpp"

CookPool::CookPool(int cooks, float multiplier, const Kitchen &kitchen):
    _cooks(cooks), _multiplier(multiplier), _kitchen(kitchen)
{
    *_pizzaInCooking.wait() = 0;
    _pizzaInCooking.signal();
    for (int i = 0; i < _cooks; i++)
        _cookers.emplace_back(&CookPool::cookThread, this);
    for (int i = 0; i < Pizza::Ingredient::IngredientCount; i++)
        _ingredients[Pizza::Ingredient(i)] = Semaphore(5);
}

void CookPool::addPizza(Pizza pizza)
{
    _queue.push(pizza);
}

void CookPool::cookThread()
{
    while (true) {

        // Fetch a pizza
        auto pizza = _queue.pop();
        (*_pizzaInCooking.wait())++; // Increment the number of pizza in cooking
        _pizzaInCooking.signal();

        // Cook it
        consumeIngredients(pizza);
        ILogger::getLogger().logPizzaCookingStarted(_kitchen.getId(), pizza);
        std::this_thread::sleep_for(std::chrono::milliseconds((unsigned)(pizza.getCookTime() * _multiplier * 1000)));
        ILogger::getLogger().logPizzaCooked(_kitchen.getId(), pizza);

        // Push it in the queue
        (*_pizzaInCooking.wait())--;
        _pizzaInCooking.signal();
        _finishedPizzas.push(pizza);
    }
}

int CookPool::getPizzaInCooking() const
{
    int inCooking = *_pizzaInCooking.wait();
    _pizzaInCooking.signal();
    return _queue.size() + inCooking;
}

CookPool::Ingredients *CookPool::getIngredients()
{
    return &_ingredients;
}

void CookPool::consumeIngredients(const Pizza &pizza)
{
    for (auto &ingredient : pizza.getIngredients())
        _ingredients[ingredient].decrement();
}

PizzaQueue &CookPool::getFinishedPizzas()
{
    return _finishedPizzas;
}
