/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"
#include "logging/ILogger.hpp"
#include <algorithm>

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
//        ILogger::getLogger().logPizzaSentToReception(_id, pizza);
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
        // If any of the ingredient is at 0, log the refill
        bool logRefill = std::any_of(ingredients->begin(), ingredients->end(), [](auto &ingredient) {
            return ingredient.second.getValue() == 0;
        });
        for (auto &ingredient : *ingredients) {
            ingredient.second.increment();
        }
        if (logRefill)
            ILogger::getLogger().logIngredientsStockUpdated(_id, *ingredients);
        _ingredientsMutex.unlock();
    }
}

void Kitchen::awaitForCommand()
{
    while (true) {
        InterProcessCom::waitForDataAvailable(*_ipcParentToChild);
        auto type = _ipcParentToChild->getRequestType();
        if (type == PizzaIPC::PIZZA) {
            Pizza pizza = _ipcParentToChild->receivePizza();
            _lastOrderTime = std::chrono::high_resolution_clock::now();
            _ipcChildToParent->notifyMessageReceived();
//            ILogger::getLogger().logPizzaReceivedByKitchen(_id, pizza);
            _cookPool->addPizza(pizza);
        } else if (type == PizzaIPC::COOKS_OCCUPANCY) {
            printCooksOccupancy();
        }
    }
}

void Kitchen::printCooksOccupancy()
{
    std::cout << "Kitchen #" << _id << " status:" << std::endl;
    std::cout << "\tPizza in cooking: " << _cookPool->getPizzaInCooking() << std::endl;
    std::cout << "\tCooks occupancy:" << std::endl;
    for (auto &[cookId, pizza] : _cookPool->getCooks())
        std::cout << "\t\tCook #" << cookId << ":\t" << (pizza.getSize() != 0 ? pizza.toString() : "Free") << std::endl;
    std::cout << "\tIngredient stock:" << std::endl;
    for (auto &[ingredient, stock] : *_cookPool->getIngredients())
        std::cout << "\t\t" << Pizza::_ingredientToString.at(ingredient) << ": " << stock.getValue() << std::endl;
    std::cout << std::endl;
    _ipcChildToParent->requestCooksOccupancy();
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
    _ipcChildToParent->getRequestType();
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

void Kitchen::printStatus() const
{
    _ipcParentToChild->requestCooksOccupancy();
    _ipcChildToParent->getRequestType();
}

Process Kitchen::getProcess()
{
    return _process;
}

void Kitchen::setProcess(Process process)
{
    _process = process;
}
