/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Reception
*/

#include "Reception.hpp"
#include "ProcessForker.hpp"
#include "logging/ILogger.hpp"
#include "logging/CliLogger.hpp"

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
    ILogger::createLogger<CliLogger>();
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

// TODO noa mets moi ça dans une classe
#include <sys/ioctl.h>
int bytesAvailable(int fd)
{
    int bytes = 0;

    if (ioctl(fd, FIONREAD, &bytes) == -1)
        throw std::runtime_error("Cannot get number of bytes available");
    return bytes;
}

void Reception::run()
{
    while (true) {
        if (bytesAvailable(0) > 0) {
            checkOrderAndSendPizzas();
        }
        for (auto &kitchen : _kitchens) {
            //if (kitchen->isKitchenClosed()) {
             //   kitchen->putTheKeyUnderTheDoor();
            //}
            if (kitchen->hasPizzaFinished()) {
                auto pizza = kitchen->getPizza();
                ILogger::getLogger().logPizzaReceivedByReception(kitchen->getId(), pizza);
            }
        }
    }
}

void Reception::runKitchen(Kitchen *kitchen)
{
    kitchen->openIpcs(true);
    kitchen->run();
}

void Reception::addKitchen()
{
    std::unique_ptr<Kitchen> kitchen = std::make_unique<Kitchen>(_cooksPerKitchen, _restockTimeMs, _multiplier);

    Process process;
    kitchen->setProcess(process);
    process.runObject(this, &Reception::runKitchen, kitchen.get());
    kitchen->openIpcs(false);

    ILogger::getLogger().logKitchenCreated(kitchen->getId());
    _kitchens.push_back(std::move(kitchen));
}

void Reception::checkKitchen()
{
    bool kitchensFull = true;

    for (auto &kitchen : _kitchens)
        if (kitchen->getPizzasAwaiting() != kitchen->getCapacity()) {
            kitchensFull = false;
            break;
        }
    if (kitchensFull || _kitchens.empty())
        addKitchen();
}

std::unique_ptr<Kitchen> *Reception::getKitchen()
{
    std::unique_ptr<Kitchen> *ref = &_kitchens.back();
    //TODO: change la getPizzaAwaiting
    int actualSize = (*ref)->getCapacity() - (*ref)->getPizzasAwaiting();

    for (auto &kitchen : _kitchens) {
        //TODO: change la getPizzaAwaiting
        int size = kitchen->getCapacity() - kitchen->getPizzasAwaiting();
        if (size < actualSize && !kitchen->isKitchenClosed())
            ref = &kitchen;
    }
    return ref;
}

void Reception::checkOrderAndSendPizzas()
{
    std::vector<Pizza> pizzas = getPizzasToCook();

    if (pizzas.empty()) {
        std::cout << "Order error" << std::endl;
        return;
    }
    std::cout << "New order" << std::endl;
    dispatchPizzas(pizzas);
}

void Reception::dispatchPizzas(std::vector<Pizza> &pizzas)
{
    for (auto &pizza : pizzas) {
        checkKitchen();
        ILogger::getLogger().logPizzaSentToKitchen(_kitchens.back()->getId(), pizza);
        getKitchen();
        std::cout << "Dispatching pizza " << pizza.getType() << std::endl;
    }
}
