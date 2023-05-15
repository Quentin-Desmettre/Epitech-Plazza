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
    /**
     * @brief Log the creation of a kitchen.
     * @param id The id of the kitchen.
     */
    void logKitchenCreated(int id) override;

    /**
     * @brief Log the closure of a kitchen.
     * @param id The id of the kitchen.
     */
    void logKitchenClosed(int id) override;

    /**
     * @brief Log the reception of a pizza by a kitchen.
     * @param kitchenId The id of the kitchen.
     * @param pizza The pizza received.
     */
    void logPizzaReceived(int kitchenId, const Pizza &pizza) override;

    /**
     * @brief Log the start of cooking of a pizza by a kitchen.
     * @param kitchenId The id of the kitchen.
     * @param pizza The pizza cooked.
     */
    void logPizzaCookingStarted(int kitchenId, const Pizza &pizza) override;

    /**
     * @brief Log the end of cooking of a pizza by a kitchen.
     * @param kitchenId The id of the kitchen.
     * @param pizza The pizza cooked.
     */
    void logPizzaCooked(int kitchenId, const Pizza &pizza) override;

private:
    void log(const std::string &msg);

    std::ofstream _file;
};

#endif //EPITECH_PLAZZA_CLILOGGER_HPP
