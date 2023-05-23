/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaIPC
*/

#include "ipc/PizzaIPC.hpp"
#include "pizza/PizzaEncoder.hpp"
#include <memory>
#include <iostream>
void PizzaIPC::sendPizza(const Pizza &pizza)
{
    std::vector<char> data = PizzaEncoder::_2pac(pizza);
    std::size_t size = data.size();
    char type = static_cast<char>(RequestType::PIZZA);
    InterProcessCom::write(&type, 1);
    InterProcessCom::write(&size, sizeof(std::size_t));
    InterProcessCom::write(data.data(), data.size());
}

Pizza PizzaIPC::receivePizza()
{
    // Get size
    std::size_t size;
    InterProcessCom::read(&size, sizeof(std::size_t));
    // Get data
    auto data = std::make_unique<char[]>(size);
    InterProcessCom::read(data.get(), size);

    // Transform data to vector and unpack
    std::vector<char> dataVector(data.get(), data.get() + size);
    return PizzaEncoder::unpack(dataVector);
}

PizzaIPC::RequestType PizzaIPC::getRequestType()
{
    char buffer;
    InterProcessCom::read(&buffer, 1);
    return static_cast<RequestType>(buffer);
}

void PizzaIPC::notifyMessageReceived() const
{
    InterProcessCom::write("1", 1);
}

void PizzaIPC::waitForNotification() const
{
    char buffer;
    InterProcessCom::read(&buffer, 1);
}

void PizzaIPC::requestCooksOccupancy() const
{
    char type = static_cast<char>(RequestType::COOKS_OCCUPANCY);
    InterProcessCom::write(&type, 1);
}
