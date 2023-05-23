/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CliLogger
*/

#include "logging/CliLogger.hpp"
#include <chrono>
#include <iostream>

CliLogger::CliLogger()
{
    auto file = _file.wait();
    file->open("plazza.log", std::ios::out | std::ios::trunc);
    _file.signal();
}

void CliLogger::log(const std::string &msg)
{
    // Format [HH:MM:SS] msg
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto now_tm = std::localtime(&now_c);
    char buffer[9];
    std::strftime(buffer, 9, "%T", now_tm);

    // Write to file
    auto file = _file.wait();
    *file << "[" << buffer << "] " << msg << std::endl;
    file->flush();
    _file.signal();
    std::cout << "[" << buffer << "] " << msg << std::endl;
}

void CliLogger::logKitchenCreated(int id)
{
    log("Kitchen #" + std::to_string(id) + " created");
}

void CliLogger::logKitchenClosed(int id)
{
    log("Kitchen #" + std::to_string(id) + " closed");
}

void CliLogger::logPizzaSentToKitchen(int kitchenId, const Pizza &pizza)
{
    log("Sent " + pizza.toString() + " to kitchen #" + std::to_string(kitchenId));
}

void CliLogger::logPizzaReceivedByKitchen(int kitchenId, const Pizza &pizza)
{
    log("Kitchen #" + std::to_string(kitchenId) + " received " + pizza.toString() + " from reception");
}

void CliLogger::logPizzaCookingStarted(int kitchenId, const Pizza &pizza)
{
    log("Kitchen #" + std::to_string(kitchenId) + " started cooking " + pizza.toString());
}

void CliLogger::logPizzaCooked(int kitchenId, const Pizza &pizza)
{
    log("Kitchen #" + std::to_string(kitchenId) + " cooked " + pizza.toString());
}

void CliLogger::logPizzaSentToReception(int kitchenId, const Pizza &pizza)
{
    log("Kitchen #" + std::to_string(kitchenId) + " sent " + pizza.toString() + " to reception");
}

void CliLogger::logPizzaReceivedByReception(int kitchenId, const Pizza &pizza)
{
    log("Reception received " + pizza.toString() + " from kitchen #" + std::to_string(kitchenId));
}

void CliLogger::logIngredientsStockUpdated(int kitchenId, const std::map<Pizza::Ingredient, Semaphore> &ingredients)
{
    std::string msg = "Kitchen #" + std::to_string(kitchenId) + " ingredients stock updated: ";
    for (auto it = ingredients.begin(); it != ingredients.end(); ++it) {
        msg += Pizza::_ingredientToString.at(it->first) + ": " + std::to_string(it->second.getValue());
        if (std::next(it) != ingredients.end())
            msg += ", ";
    }
    log(msg);
}
