/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"
#include <iostream>

Kitchen::Kitchen(int cooks, int restockTimeMs, float multiplier) : _ipcPtoC(nullptr), _ipcCtoP(nullptr), _multiplier(multiplier), _cooks(cooks), _restockTimeMs(restockTimeMs), _cookPool(cooks, multiplier)
{
    for (int i = 0; i < Pizza::Ingredient::IngredientCount; i++)
        _ingredients[Pizza::Ingredient(i)] = 5;
    _timeoutClock = std::chrono::high_resolution_clock::now();
    _ipcCtoP = std::make_unique<PizzaIPC>();
    _ipcPtoC = std::make_unique<PizzaIPC>();
}

void Kitchen::run()
{
    std::thread refillThread(&Kitchen::checkForRefill, this);
    std::thread commandThread(&Kitchen::awaitForCommand, this);
    std::thread cookThread(&Kitchen::awaitFinishedCook, this);

    refillThread.join();
    commandThread.join();
    cookThread.join();
}

void Kitchen::checkForRefill()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(_restockTimeMs));

        for (auto &ingredient : _ingredients)
            ingredient.second += 1;
    }
}

void Kitchen::awaitFinishedCook()
{

    while (true) {
        std::vector<Pizza> finishedPizzas = _cookPool.clearFinishedPizzas();

        for (auto &pizza : finishedPizzas) {
            std::cout << "Pizza finished" << std::endl;
            _ipcCtoP->sendPizza(pizza);
        }
    }
}

void Kitchen::awaitForCommand()
{
    while (true) {
        if (_ipcPtoC->hasPizza())
            _cookPool.addPizza(_ipcPtoC->receivePizza());
    }
}

bool Kitchen::canCookPizza(const Pizza &pizza)
{
    for (auto &ingredient : pizza.getIngredients()) {
        if (_ingredients[ingredient] == 0)
            return false;
    }
    return true;
}

int Kitchen::getPizzasAwaiting() const
{
    return _cookPool.getPizzaInCooking();
}

int Kitchen::getCapacity() const
{
    return _cooks * 2;
}

void Kitchen::addPizza(const Pizza &pizza)
{
    _ipcPtoC->sendPizza(pizza);
}

bool Kitchen::hasPizzaFinished()
{
    return _ipcCtoP->hasPizza();
}

bool Kitchen::isKitchenClosed()
{
    if (_cookPool.getPizzaInCooking() > 0) return false;

    auto now = std::chrono::high_resolution_clock::now();
    double elapsedTimeMS = std::chrono::duration<double, std::milli>(now - _timeoutClock).count();

    return elapsedTimeMS > 5000;
}

Pizza Kitchen::getPizza()
{
    return _ipcCtoP->receivePizza();
}

void Kitchen::putTheKeyUnderTheDoor()
{
    _process.kill();
}

void Kitchen::setProcess(Process process)
{
    _process = process;
}

void Kitchen::openIpcs(int isForked)
{
    if (isForked) {
        _ipcCtoP->open(InterProcessCom::OpenMode::WRITE);
        _ipcPtoC->open(InterProcessCom::OpenMode::READ);
    } else {
        _ipcCtoP->open(InterProcessCom::OpenMode::READ);
        _ipcPtoC->open(InterProcessCom::OpenMode::WRITE);
    }
}
