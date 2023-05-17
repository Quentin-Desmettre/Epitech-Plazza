/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"
#include <iostream>

Kitchen::Kitchen(int cooks, int restockTimeMs, float multiplier) : _ipcParentToChild(nullptr), _ipcChildToParent(nullptr), _multiplier(multiplier), _cooks(cooks), _restockTimeMs(restockTimeMs), _isCooking(false), _cookPool(nullptr)
{
    _timeoutClock = std::chrono::high_resolution_clock::now();
    _ipcChildToParent = std::make_unique<PizzaIPC>();
    _ipcParentToChild = std::make_unique<PizzaIPC>();
}

void Kitchen::run()
{
    _cookPool = std::make_unique<CookPool>(_cooks, _multiplier);
    std::thread refillThread(&Kitchen::checkForRefill, this);
    std::thread commandThread(&Kitchen::awaitForCommand, this);
    std::thread cookThread(&Kitchen::awaitFinishedCook, this);

    refillThread.join();
    commandThread.join();
    cookThread.join();
}

void Kitchen::checkForRefill()
{
    CookPool::Ingredients ingredients = _cookPool->getIngredients();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(_restockTimeMs));

        for (auto &ingredient : ingredients)
            ingredient.second.increment();
    }
}

void Kitchen::awaitFinishedCook()
{
    while (true) {
        std::vector<Pizza> finishedPizzas = _cookPool->clearFinishedPizzas();

        for (auto &pizza : finishedPizzas) {
            std::cout << "Pizza finished" << std::endl;
            _ipcChildToParent->sendPizza(pizza);
        }
        if (_cookPool->getPizzaInCooking() == 0) {
            _isCooking = false;
            _timeoutClock = std::chrono::high_resolution_clock::now();
        }
    }
}

void Kitchen::awaitForCommand()
{
    while (true) {
        if (_ipcParentToChild->hasPizza()) {
            std::cout << "Pizza received" << std::endl;
            Pizza pizza = _ipcParentToChild->receivePizza();

            _cookPool->addPizza(pizza);
            _isCooking = true;
        }
    }
}

int Kitchen::getPizzasAwaiting() const
{
    return _cookPool->getPizzaInCooking();
}

int Kitchen::getCapacity() const
{
    return _cooks * 2;
}

void Kitchen::addPizza(const Pizza &pizza)
{
    _ipcParentToChild->sendPizza(pizza);
}

bool Kitchen::hasPizzaFinished()
{
    return _ipcChildToParent->hasPizza();
}

bool Kitchen::isKitchenClosed()
{
    if (_isCooking)
        return false;

    auto now = std::chrono::high_resolution_clock::now();
    double elapsedTimeMS = std::chrono::duration<double, std::milli>(now - _timeoutClock).count();

    return elapsedTimeMS > 10000;
}

Pizza Kitchen::getPizza()
{
    return _ipcChildToParent->receivePizza();
}

void Kitchen::putTheKeyUnderTheDoor()
{
    _process.kill();
}

void Kitchen::setProcess(Process process)
{
    _process = process;
}

void Kitchen::openIpcs(bool isForked)
{
    if (isForked) {
        _ipcChildToParent->open(InterProcessCom::OpenMode::WRITE);
        _ipcParentToChild->open(InterProcessCom::OpenMode::READ);
    } else {
        _ipcChildToParent->open(InterProcessCom::OpenMode::READ);
        _ipcParentToChild->open(InterProcessCom::OpenMode::WRITE);
    }
}
