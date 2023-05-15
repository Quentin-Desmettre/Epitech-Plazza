/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Reception
*/

#include <unistd.h>
#include "Reception.hpp"
#include "ProcessForker.hpp"

Reception::Reception(int ac, char **av)
{
    if (ac != 4)
        throw std::invalid_argument("Usage: ./plazza [multiplier] [cooks per kitchen] [restock time (ms)]");
    _multiplier = std::stof(av[1]);
    _cooksPerKitchen = std::stoi(av[2]);
    _restockTimeMs = std::stoi(av[3]);

    if (_multiplier <= 0)
        throw std::invalid_argument("Multiplier must be greater than 0");
    if (_cooksPerKitchen <= 0)
        throw std::invalid_argument("Cooks per kitchen must be greater than 0");
    if (_restockTimeMs <= 0)
        throw std::invalid_argument("Restock time must be greater than 0");
    _parser = ParsePizza();
}

std::vector<Pizza> Reception::getPizzasToCook()
{
    std::string pizzaName;

    if (!std::getline(std::cin, pizzaName))
        throw std::runtime_error("User pressed Ctrl+D");
    if (pizzaName == "exit")
        throw std::runtime_error("User exited");
    _parser.RunChecker(pizzaName);
    return _parser.GetPizzas();
}

void Reception::run()
{
    while (true) {
        if (std::cin.peek() != std::istream::traits_type::eof()) {
            getPizzasToCook();
        }
        for (auto &kitchen : _kitchens) {
            if (kitchen->isKitchenClosed()) {
                //kitchen->close();
            }
            if (kitchen->hasPizzaFinished()) {
            //    auto pizza = kitchen->getPizzaCooked();
            //    std::cout << "Pizza " << pizza.getName() << " cooked by kitchen " << kitchen->getId() << std::endl;
            }
        }
    }
}

void Reception::runKitchen(Kitchen *kitchen)
{
    kitchen->openIpcs(PizzaIPC::READ, PizzaIPC::WRITE);
    kitchen->run();
}


void Reception::addKitchen()
{
    std::unique_ptr<Kitchen> kitchen = std::make_unique<Kitchen>(_multiplier, _cooksPerKitchen, _restockTimeMs);
    Process process;
    kitchen->setProcess(process);
    process.runObject(this, &Reception::runKitchen, kitchen.get());

    kitchen->openIpcs(PizzaIPC::WRITE, PizzaIPC::READ);
    _kitchens.push_back(std::move(kitchen));
}

void Reception::dispatchPizzas()
{

}
