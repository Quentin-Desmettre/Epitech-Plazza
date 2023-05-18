/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** CliLogger
*/

#ifndef EPITECH_PLAZZA_CLILOGGER_HPP
#define EPITECH_PLAZZA_CLILOGGER_HPP
#include "logging/ILogger.hpp"
#include <fstream>

class CliLogger: public ILogger {
public:
    CliLogger();

    // Events
    void logKitchenCreated(int id) override;

    /**
     * @see ILogger::logKitchenClosed
     */
    void logKitchenClosed(int id) override;

    /**
     * @see ILogger::logPizzaSentToKitchen
     */
    void logPizzaSentToKitchen(int kitchenId, const Pizza &pizza) override;

    /**
     * @see ILogger::logPizzaReceivedByKitchen
     */
    void logPizzaReceivedByKitchen(int kitchenId, const Pizza &pizza) override;

    /**
     * @see ILogger::logPizzaCookingStarted
     */
    void logPizzaCookingStarted(int kitchenId, const Pizza &pizza) override;

    /**
     * @see ILogger::logPizzaCooked
     */
    void logPizzaCooked(int kitchenId, const Pizza &pizza) override;

    /**
     * @see ILogger::logPizzaSentToReception
     */
    void logPizzaSentToReception(int kitchenId, const Pizza &pizza) override;

    /**
     * @see ILogger::logPizzaReceivedByReception
     */
    void logPizzaReceivedByReception(int kitchenId, const Pizza &pizza) override;

    /**
     * @see ILogger::logIngredientsStockUpdated
     */
    void logIngredientsStockUpdated(int kitchenId, const std::map<Pizza::Ingredient, Semaphore> &ingredients) override;

private:
    void log(const std::string &msg);

    std::ofstream _file;
};

#endif //EPITECH_PLAZZA_CLILOGGER_HPP
