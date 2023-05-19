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

Kitchen::Kitchen(int cooks, int restockTimeMs, float multiplier) :
    _ipcParentToChild(nullptr),
    _ipcChildToParent(nullptr),
    _multiplier(multiplier),
    _cooks(cooks),
    _restockTimeMs(restockTimeMs),
    _pizzaCounter(0),
    _cookPool(nullptr)
{
    _id = _maxId;
    _maxId++;
    _ipcChildToParent = std::make_unique<PizzaIPC>();
    _ipcParentToChild = std::make_unique<PizzaIPC>();
}

Kitchen::~Kitchen()
{
    _process.kill();
}

void Kitchen::run()
{
    _cookPool = std::make_unique<CookPool>(_cooks, _multiplier, *this);
    std::thread refillThread(&Kitchen::checkForRefill, this);
    std::thread commandThread(&Kitchen::awaitForCommand, this);
    std::thread cookThread(&Kitchen::awaitFinishedCook, this);

    refillThread.join();
    commandThread.join();
    cookThread.join();
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

void Kitchen::awaitFinishedCook()
{
    while (true) {
        std::vector<Pizza> finishedPizzas = _cookPool->clearFinishedPizzas();

        for (auto &pizza : finishedPizzas) {
            ILogger::getLogger().logPizzaSentToReception(_id, pizza);
            _ipcChildToParent->sendPizza(pizza);
            _pizzaCounter.decrement();
        }
        if (_cookPool->getPizzaInCooking() == 0) {
            _lastPizzaFinished = std::chrono::high_resolution_clock::now();
            std::thread(
            [](Kitchen *kitchen) {
                std::this_thread::sleep_for(std::chrono::seconds(5));

                auto now = std::chrono::high_resolution_clock::now();
                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - kitchen->_lastPizzaFinished).count();
                if (diff >= 4995 && kitchen->_pizzaCounter.getValue() == 0) { // Error margin
                    ILogger::getLogger().logKitchenClosed(kitchen->_id);
                    kitchen->_process.kill();
                }
            }, this).detach();
        }
    }
}

void Kitchen::awaitForCommand()
{
    while (true) {
        if (_ipcParentToChild->hasPizza()) {
            Pizza pizza = _ipcParentToChild->receivePizza();
            ILogger::getLogger().logPizzaReceivedByKitchen(_id, pizza);

            _pizzaCounter.increment();
            _cookPool->addPizza(pizza);
        }
    }
}

int Kitchen::getPizzasAwaiting() const
{
    return _pizzaCounter.getValue();
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
    // TODO A CHECK
    auto now = std::chrono::high_resolution_clock::now();
    double elapsedTimeMS = std::chrono::duration<double, std::milli>(now - _lastPizzaFinished).count();

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

const Process &Kitchen::getProcess() const
{
    return _process;
}

int Kitchen::getId() const
{
    return _id;
}
