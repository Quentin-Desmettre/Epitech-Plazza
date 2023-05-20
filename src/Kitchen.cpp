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
    _cookPool(nullptr),
    _pizzaCounter(0)
{
    _id = _maxId;
    _maxId++;
    _ipcChildToParent = std::make_unique<PizzaIPC>();
    _ipcParentToChild = std::make_unique<PizzaIPC>();
    _isForked = false;
}

Kitchen::~Kitchen() = default;

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

        _lastOrderTime = std::chrono::high_resolution_clock::now();
        if (_cookPool->getPizzaInCooking() != 0)
            continue;
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::seconds(5));

            auto now = std::chrono::high_resolution_clock::now();
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastOrderTime).count();
            if (diff >= 4995 && _pizzaCounter == 0) { // Error margin
                _ingredientsMutex.lock();
                ILogger::getLogger().logKitchenClosed(_id);
                Process::exit();
            }
        }).detach();
    }
}

void Kitchen::checkForRefill()
{
    CookPool::Ingredients *ingredients = _cookPool->getIngredients();

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(_restockTimeMs));

        _ingredientsMutex.lock();
        for (auto &ingredient : *ingredients)
            ingredient.second.increment();
        ILogger::getLogger().logIngredientsStockUpdated(_id, *ingredients);
        _ingredientsMutex.unlock();
    }
}

void Kitchen::awaitForCommand()
{
    while (true) {
        InterProcessCom::waitForDataAvailable(*_ipcParentToChild);
        Pizza pizza = _ipcParentToChild->receivePizza();
        _lastOrderTime = std::chrono::high_resolution_clock::now();
        _ipcChildToParent->notifyMessageReceived();
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
    _ipcParentToChild->sendPizza(pizza);
    _pizzaCounter++;
}

Pizza Kitchen::getPizza()
{
    auto p = _ipcChildToParent->receivePizza();
    _pizzaCounter--;
    return p;
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
