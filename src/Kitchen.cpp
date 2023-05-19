/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"
#include <iostream>
#include "logging/ILogger.hpp"

int Kitchen::_maxId = 0;

Kitchen::Kitchen(int cooks, int restockTimeMs, float multiplier) : _ipcParentToChild(nullptr), _ipcChildToParent(nullptr), _multiplier(multiplier), _cooks(cooks), _restockTimeMs(restockTimeMs), _isCooking(false), _cookPool(nullptr)
{
    _id = _maxId;
    _maxId++;
    _timeoutClock = std::chrono::high_resolution_clock::now();
    _ipcChildToParent = std::make_unique<PizzaIPC>();
    _ipcParentToChild = std::make_unique<PizzaIPC>();
}

Kitchen::~Kitchen()
{
    _process->kill();
}

void Kitchen::run()
{
    _cookPool = std::make_unique<CookPool>(_cooks, _multiplier, *this);
    std::thread refillThread(&Kitchen::checkForRefill, this);
    std::thread commandThread(&Kitchen::awaitForCommand, this);

    while (true) {
        Pizza pizza = _cookPool->getFinishedPizzas().pop();
        ILogger::getLogger().logPizzaSentToReception(_id, pizza);
        _ipcChildToParent->sendPizza(pizza);

        if (_cookPool->getPizzaInCooking() == 0) {
            _isCooking = false;
            _timeoutClock = std::chrono::high_resolution_clock::now();
        }
    }
}

void Kitchen::checkForRefill()
{
    CookPool::Ingredients *ingredients = _cookPool->getIngredients();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(_restockTimeMs));

        for (auto &ingredient : *ingredients)
            ingredient.second.increment();
        ILogger::getLogger().logIngredientsStockUpdated(_id, *ingredients);
    }
}

void Kitchen::awaitForCommand()
{
    while (true) {
        if (_ipcParentToChild->hasPizza()) {
            Pizza pizza = _ipcParentToChild->receivePizza();
            ILogger::getLogger().logPizzaReceivedByKitchen(_id, pizza);

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
    _counter++;
    _lastOrderTime = std::chrono::high_resolution_clock::now();
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
    auto p = _ipcChildToParent->receivePizza();
    _counter--;
    _lastOrderTime = std::chrono::high_resolution_clock::now();
    return p;
}

void Kitchen::putTheKeyUnderTheDoor()
{
    _close = true;
}

void Kitchen::setProcess(Process *process)
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

int Kitchen::getId() const
{
    return _id;
}
