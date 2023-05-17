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
    logger.logPizzaSentToKitchen(1, Pizza(Pizza::Regina, Pizza::S, 1, {}));
    logger.logPizzaReceivedByKitchen(1, Pizza(Pizza::Regina, Pizza::S, 1, {}));
    logger.logPizzaSentToReception(1, Pizza(Pizza::Regina, Pizza::S, 1, {}));
    logger.logPizzaReceivedByReception(1, Pizza(Pizza::Regina, Pizza::S, 1, {}));
    logger.logPizzaCookingStarted(1, Pizza(Pizza::Regina, Pizza::S, 1, {}));
    logger.logPizzaCooked(1, Pizza(Pizza::Regina, Pizza::S, 1, {}));
    logger.logIngredientsStockUpdated(1, {{Pizza::Doe, Semaphore(1)}, {Pizza::ChiefLove, Semaphore(2)}});

    // Get file content
    std::ifstream file("plazza.log");
    std::string line;

    // Kitchen #1 created
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 created");

    // Kitchen #1 closed
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 closed");

    // Kitchen #1 received Regina S
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Sent Regina S to kitchen #1");

    // Kitchen #1 received Regina S
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 received Regina S from reception");

    // Kitchen #1 sent Regina S to reception
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 sent Regina S to reception");

    // Kitchen #1 received Regina S from kitchen #1
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Reception received Regina S from kitchen #1");

    // Kitchen #1 started cooking Regina S
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 started cooking Regina S");

    // Kitchen #1 cooked Regina S
    std::getline(file, line);
    CHECK_EQ(line, "[" + std::string(buffer) + "] Kitchen #1 cooked Regina S");

    // Ingredients stock updated
    std::getline(file, line);
//    CHECK_EQ(line, "[" + std::string(buffer) + "] Ingredients stock updated: Doe=1 ChiefLove=2");
    CHECK_EQ("[" + std::string(buffer) + "] Kitchen #1 ingredients stock updated: 0: 1, 8: 2", line);
}
