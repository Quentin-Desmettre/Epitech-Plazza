/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Kitchen
*/

#include "Kitchen.hpp"

Kitchen::Kitchen(int cooks, int restockTimeMs, float multiplier) : _cooks(cooks), _restockTimeMs(restockTimeMs), _multiplier(multiplier)
{

}

void Kitchen::run()
{
    std::thread refillThread(&Kitchen::checkForRefill, this);

//    while (true) {
//        awaitFinishedCook();
//        awaitForCommand();
//    }
}

void Kitchen::checkForRefill()
{
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(_restockTimeMs));
//        _ingredients.restock();
    }
}