/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Reception
*/

#include <unistd.h>
#include "Reception.hpp"

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

void Reception::addKitchen()
{
    std::unique_ptr<Kitchen> kitchen = std::make_unique<Kitchen>(_multiplier, _cooksPerKitchen, _restockTimeMs);
    InterProcessCom ipc1;
    InterProcessCom ipc2;
    int pid = fork();
    if (pid == -1)
        throw std::runtime_error("fork failed");
    if (pid == 0) {
        ipc1.open(InterProcessCom::OpenMode::READ);
        ipc2.open(InterProcessCom::OpenMode::WRITE);
        //open IPC
        kitchen->run();
        exit(0);
    }
    ipc1.open(InterProcessCom::OpenMode::WRITE);
    ipc2.open(InterProcessCom::OpenMode::READ);
    //open IPC;
    kitchen->setPid(pid);
    _kitchens.push_back(std::move(kitchen));
}

void Reception::dispatchPizzas() {
    //at the end of the function, all pizzas are dispatched and all kitchens have the same number of pizzas to cook
    std::vector<Pizza> pizzas = getPizzasToCook();
    std::vector<std::unique_ptr<Kitchen>> kitchens;

    int index;
    int nbPizzas = 100;

    while (!pizzas.empty()) {
        index = 0;

        for (int i = 0; i < kitchens.size(); i++)
            if (kitchen[i]->getPizzasAwaiting() < nbPizzas &&
                kitchen[i]->getCapacity() > kitchen[i]->getPizzasAwaiting())
                index = kitchen;

        if (kitchens.empty())
            addKitchen();
        kitchens[index]->addPizza(pizzas.back());
        pizzas.pop_back();
    }
}
