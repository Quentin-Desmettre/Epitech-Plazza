/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CliLogger
*/

#include "logging/CliLogger.hpp"
#include <chrono>

CliLogger::CliLogger():
    _file("plazza.log")
{
}

void CliLogger::log(const std::string &msg)
{
    // Format [HH:MM:SS] msg
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto now_tm = std::localtime(&now_c);
    char buffer[9];
    std::strftime(buffer, 9, "%T", now_tm);
    _file << "[" << buffer << "] " << msg << std::endl;
    _file.flush();
}

void CliLogger::logKitchenCreated(int id)
{
    log("Kitchen #" + std::to_string(id) + " created");
}

void CliLogger::logKitchenClosed(int id)
{
    log("Kitchen #" + std::to_string(id) + " closed");
}

void CliLogger::logPizzaReceived(int kitchenId, const Pizza &pizza)
{
    log("Kitchen #" + std::to_string(kitchenId) + " received " + pizza.toString());
}

void CliLogger::logPizzaCookingStarted(int kitchenId, const Pizza &pizza)
{
    log("Kitchen #" + std::to_string(kitchenId) + " started cooking " + pizza.toString());
}

void CliLogger::logPizzaCooked(int kitchenId, const Pizza &pizza)
{
    log("Kitchen #" + std::to_string(kitchenId) + " cooked " + pizza.toString());
}
