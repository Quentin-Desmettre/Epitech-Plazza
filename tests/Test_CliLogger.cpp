/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_CliLogger
*/

#include "doctest.h"
#include "logging/CliLogger.hpp"
#include <fstream>
#include <chrono>

TEST_CASE("clilogger")
{
    CHECK_THROWS_AS(ILogger::getLogger(), std::runtime_error);
    ILogger::createLogger<CliLogger>();
    CHECK_THROWS_AS(ILogger::createLogger<CliLogger>(), std::runtime_error);
    ILogger &logger = ILogger::getLogger();

    // Get timestamp
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    auto now_tm = std::localtime(&now_c);
    char buffer[9];
    std::strftime(buffer, 9, "%T", now_tm);

    // Log
    logger.logKitchenCreated(1);
    logger.logKitchenClosed(1);
    logger.logPizzaReceived(1, Pizza(Pizza::Regina, Pizza::S, 1, {}));
    logger.logPizzaCookingStarted(1, Pizza(Pizza::Regina, Pizza::S, 1, {}));
    logger.logPizzaCooked(1, Pizza(Pizza::Regina, Pizza::S, 1, {}));

    // Get file content
    std::ifstream file("plazza.log");
    std::string line;

    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 created");
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 closed");
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 received Regina S");
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 started cooking Regina S");
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 cooked Regina S");
}
