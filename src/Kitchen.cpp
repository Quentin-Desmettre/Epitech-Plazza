/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"
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
    _isForked = false;
}

Kitchen::~Kitchen()
{
    _process->kill();
}

void Kitchen::run()
{
    _cookPool = std::make_unique<CookPool>(_cooks, _multiplier, *this);
    std::thread refillThread(&Kitchen::checkForRefill, this);   // Check for refill of ingredients
    std::thread commandThread(&Kitchen::awaitForCommand, this); // Await for command from reception

    // Await finished pizzas and send them to reception
    while (true) {
        Pizza pizza = _cookPool->getFinishedPizzas().pop();
        ILogger::getLogger().logPizzaSentToReception(_id, pizza);
        _ipcChildToParent->sendPizza(pizza);

        if (_pizzaCounter == 0) {
            _lastOrderTime = std::chrono::high_resolution_clock::now();
            std::thread(
            [](Kitchen *kitchen) {
                std::this_thread::sleep_for(std::chrono::seconds(5));

                auto now = std::chrono::high_resolution_clock::now();
                auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - kitchen->_lastOrderTime).count();
                if (diff >= 4995 && kitchen->_pizzaCounter == 0) { // Error margin
                    ILogger::getLogger().logKitchenClosed(kitchen->_id);
                    kitchen->_process->kill();
                }
            }, this).detach();
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
        InterProcessCom::waitForDataAvailable(*_ipcParentToChild);
        Pizza pizza = _ipcParentToChild->receivePizza();
        ILogger::getLogger().logPizzaReceivedByKitchen(_id, pizza);
        _cookPool->addPizza(pizza);
    }
}

int Kitchen::getPizzasAwaiting() const
{
    if (_isForked)
        return _cookPool->getPizzaInCooking();
    return _pizzaCounter;
}

int Kitchen::getCapacity() const
{
    return _cooks * 2;
}

void Kitchen::addPizza(const Pizza &pizza)
{
    _pizzaCounter++;
    _lastOrderTime = std::chrono::high_resolution_clock::now();
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
    double elapsedTimeMS = std::chrono::duration<double, std::milli>(now - _lastOrderTime).count();

    return elapsedTimeMS > 10000;
}

Pizza Kitchen::getPizza()
{
    auto p = _ipcChildToParent->receivePizza();
    _pizzaCounter--;
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
    _isForked = isForked;
}

int Kitchen::getId() const
{
    return _id;
}

const InterProcessCom &Kitchen::getReadIpc() const
{
    if (_isForked)
        return *_ipcParentToChild;
    return *_ipcChildToParent;
}
