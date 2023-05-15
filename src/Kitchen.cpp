/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"

Kitchen::Kitchen(int cooks, int restockTimeMs, float multiplier) : _cooks(cooks), _restockTimeMs(restockTimeMs), _multiplier(multiplier), _cookPool(cooks, multiplier)
{
    for (int i = 0; i < Pizza::Ingredient::IngredientCount; i++)
        _ingredients[Pizza::Ingredient(i)] = 5;
    _timeoutClock = std::chrono::high_resolution_clock::now();
}

void Kitchen::run()
{
//    _readIpc->open(InterProcessCom::OpenMode::READ);
//    _writeIpc->open(InterProcessCom::OpenMode::WRITE);


    std::thread refillThread(&Kitchen::checkForRefill, this);
    std::thread commandThread(&Kitchen::awaitForCommand, this);

//    while (true) {
//        awaitFinishedCook();
//        awaitForCommand();
//    }
    refillThread.join();
    commandThread.join();
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

}

void Kitchen::awaitForCommand()
{
    while (true) {
        _cookPool.addPizza(_readIpc->receivePizza());
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

}

bool Kitchen::hasPizzaFinished()
{

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

}

void Kitchen::putTheKeyUnderTheDoor()
{
    _process.kill();
}

void Kitchen::setProcess(Process process)
{
    _process = process;
}
