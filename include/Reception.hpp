/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Reception
*/

#ifndef EPITECH_PLAZZA_RECEPTION_HPP
#define EPITECH_PLAZZA_RECEPTION_HPP
#include <vector>
#include <memory>
#include <map>
#include <cstddef>
#include <thread>
#include <semaphore>
#include <queue>
#include "ipc/InterProcessCom.hpp"
#include "pizza/Pizza.hpp"
#include "Kitchen.hpp"
#include "pizza/ParsePizza.hpp"

/**
 * @brief Reception class
 *
 * This class handles input from the user (parse / send pizzas kitchen / open kitchen)
 * And handles the answers from the kitchen, and the timeout of kitchen
 */
class Reception {
public:
    Reception(int ac, char **av);

    class ErrorParsing : public std::exception {
        public:
            ErrorParsing(const std::string &message) : _message(message) {}
            const char *what() const noexcept override { return _message.c_str(); }
        private:
            std::string _message;
    };

    /**
     * @brief Runs the reception forever.
     *
     * Steps:
     *  - check if there is data availabe on the std input
     *      => if so, parse user input
     *  - for each kitchen, check if it is doing something
     *          if so, restart its clock
     *          else, check if the clock > TIME_TO_DIE:
     *              if so, kill the kitchen
     *  - for each kitchen, check if has a pizza cooked
     */
    void run();

    /**
     * @brief Parses the input from the user.
     *
     * Reads the standard input and parse the command.
     * @return Empty vector if there is a parse error, else the pizzas to cook.
     * @throws std::runtime_error If user pressed Ctrl+D
     */
    std::vector<Pizza> getPizzasToCook();

private:
    float _multiplier;
    int _cooksPerKitchen;
    int _restockTimeMs;
    ParsePizza _parser;

    std::vector<std::unique_ptr<Kitchen>> _kitchens;

    /**
     * Creates a new kitchen.
     *
     * Steps:
     *  - create kitchen
     *  - fork
     *  - in child:
     *      - open IPC in read/write
     *  - in parent:
     *      - open IPC in write/read
     *      - add kitchen to the list
     * @return the created kitchen
     */
    void addKitchen();


    void checkKitchen();
    void removeKitchen(Kitchen *kitchen);
    std::unique_ptr<Kitchen> *getKitchen();
    void checkOrderAndSendPizzas();
    /**
     * @brief Dispatches pizzas to the kitchens.
     */
    void dispatchPizzas(std::vector<Pizza> &pizzas);
};

#endif //EPITECH_PLAZZA_RECEPTION_HPP
