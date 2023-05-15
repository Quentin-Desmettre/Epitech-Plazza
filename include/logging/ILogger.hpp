/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** ILogger
*/

#ifndef EPITECH_PLAZZA_ILOGGER_HPP
#define EPITECH_PLAZZA_ILOGGER_HPP
#include <string>
#include <memory>
#include "pizza/Pizza.hpp"

class ILogger {
public:
    virtual ~ILogger() = default;
    // Delete copy and move constructors and assign operators
    ILogger(ILogger const &) = delete;             // Copy construct
    ILogger(ILogger &&) = delete;                  // Move construct
    ILogger &operator=(ILogger const &) = delete;  // Copy assign
    ILogger &operator=(ILogger &&) = delete;       // Move assign

    static ILogger &getLogger();

    // General
    /**
     * @brief Create the logger from a class.
     */
    template <class T>
    static void createLogger() {
        if (_logger)
            throw std::runtime_error("Logger already created");
        ILogger::_logger = std::make_unique<T>();
    }

    // Events
    /**
     * @brief Log the creation of a kitchen.
     * @param id The id of the kitchen.
     */
    virtual void logKitchenCreated(int id) = 0;

    /**
     * @brief Log the closure of a kitchen.
     * @param id The id of the kitchen.
     */
    virtual void logKitchenClosed(int id) = 0;

    /**
     * @brief Log the reception of a pizza by a kitchen.
     * @param kitchenId The id of the kitchen.
     * @param pizza The pizza received.
     */
    virtual void logPizzaReceived(int kitchenId, const Pizza &pizza) = 0;

    /**
     * @brief Log the start of cooking of a pizza by a kitchen.
     * @param kitchenId The id of the kitchen.
     * @param pizza The pizza cooked.
     */
    virtual void logPizzaCookingStarted(int kitchenId, const Pizza &pizza) = 0;

    /**
     * @brief Log the end of cooking of a pizza by a kitchen.
     * @param kitchenId The id of the kitchen.
     * @param pizza The pizza cooked.
     */
    virtual void logPizzaCooked(int kitchenId, const Pizza &pizza) = 0;

protected:
    ILogger() = default;
private:
    static std::unique_ptr<ILogger> _logger;
};

#endif //EPITECH_PLAZZA_ILOGGER_HPP
