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
#include <future>
#include <algorithm>

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

Reception::~Reception()
{
    for (auto &kitchen : _kitchens)
        kitchen->getProcess().kill();
}

std::vector<Pizza> Reception::getPizzasToCook(bool &isOk)
{
    std::string pizzaName;

    if (!std::getline(std::cin, pizzaName))
        throw ErrorParsing("User pressed Ctrl+D");
    if (pizzaName == "exit")
        throw ErrorParsing("User exited");
    if (pizzaName == "status") {
        isOk = true;
        return {};
    }
    isOk = _parser.RunChecker(pizzaName);
    return _parser.GetPizzas();
}

void Reception::run()
{
    InterProcessCom::InputSource inputSource;

    while (true) {
        auto uniqueKitchen = InterProcessCom::waitForDataAvailable(inputSource, _kitchens);

        if (!uniqueKitchen && inputSource != InterProcessCom::STDIN)
            continue;
        if (inputSource == InterProcessCom::STDIN) {
            checkOrderAndSendPizzas();
            continue;
        }
        auto kitchen = uniqueKitchen->get();
        try {
            auto pizza = kitchen->getPizza();
            ILogger::getLogger().logPizzaReceivedByReception(kitchen->getId(), pizza);
        } catch (InterProcessCom::PipeException &) {
            removeKitchen(kitchen);
        }
    }
}

void Reception::addKitchen()
{
    std::unique_ptr<Kitchen> kitchen = std::make_unique<Kitchen>(_cooksPerKitchen, _restockTimeMs, _multiplier);

    // Fork the kitchen process
    kitchen->setProcess(Process::run([](Kitchen *kitchen) {
        kitchen->openIpcs(true); // Open IPCs in forked mode
        kitchen->run();
    }, kitchen.get()));
    // Open IPCs in parent process
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
    int actualSize = (*ref)->getCapacity() - (*ref)->getPizzasAwaiting();

    for (auto &kitchen : _kitchens) {
        int size = kitchen->getCapacity() - kitchen->getPizzasAwaiting();
        if (size > 0 && size < actualSize) {
            ref = &kitchen;
            actualSize = size;
        }
    }
    if (actualSize <= 0) {
        addKitchen();
        ref = &_kitchens.back();
    }
    return ref;
}

void Reception::checkOrderAndSendPizzas()
{
    bool ok;
    std::vector<Pizza> pizzas = getPizzasToCook(ok);

    if (pizzas.empty() && ok) {
        if (_kitchens.empty()) {
            std::cout << "No kitchen available" << std::endl;
            return;
        }
        for (auto &kitchen : _kitchens)
            kitchen->printStatus();
        return;
    }
    if (pizzas.empty()) {
        std::cerr << "Command Error" << std::endl;
        return;
    }
    dispatchPizzas(pizzas);
}

void Reception::removeKitchen(Kitchen *kitchen)
{
    auto it = std::find_if(_kitchens.begin(), _kitchens.end(), [kitchen](std::unique_ptr<Kitchen> &k) {
        return k.get() == kitchen;
    });

    if (it == _kitchens.end())
        return;
    ILogger::getLogger().logKitchenClosed(kitchen->getId());
    kitchen->getProcess().kill();
    _kitchens.erase(it);
}

void Reception::dispatchPizzas(std::vector<Pizza> &pizzas)
{
    pizzas.insert(pizzas.end(), _pizzasBuffer.begin(), _pizzasBuffer.end());
    _pizzasBuffer.clear();
    for (auto it = pizzas.begin(); it != pizzas.end(); it++) {
        auto pizza = *it;
        checkKitchen();
        std::unique_ptr<Kitchen> *kitchen = getKitchen();
        if (_kitchens.size() > 450) {
            std::cerr << "Too many open kitchens. Pizzas that are not dispatched are saved and will be placed during the next order (if possible)." << std::endl;
            _pizzasBuffer.insert(_pizzasBuffer.end(), it, pizzas.end());
            break;
        }
        try {
            kitchen->get()->addPizza(pizza);
            const auto *readIpc = reinterpret_cast<const PizzaIPC *>(&kitchen->get()->getReadIpc());
            readIpc->waitForNotification();
//            ILogger::getLogger().logPizzaSentToKitchen(kitchen->get()->getId(), pizza);
        } catch (InterProcessCom::PipeException &) {
            removeKitchen(kitchen->get());
            it--;
        }
    }
    std::cerr << "Dispatched pizzas: " << pizzas.size() - _pizzasBuffer.size() << "/" << pizzas.size() << std::endl;
}
