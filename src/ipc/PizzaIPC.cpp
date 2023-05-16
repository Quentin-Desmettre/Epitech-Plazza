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
    InterProcessCom::write(&size, sizeof(std::size_t));
    InterProcessCom::write(data.data(), data.size());
    std::cout << "Sending pizza of size " << size << std::endl << std::endl;
}

Pizza PizzaIPC::receivePizza()
{
    if (_buffer.size() < sizeof(std::size_t))
        throw std::runtime_error("Make sure PizzaIPC::hasPizza returns true before calling PizzaIPC::receivePizza");

    std::vector<char> pizzaData;
    std::size_t size = *reinterpret_cast<std::size_t *>(_buffer.data());
    pizzaData.insert(pizzaData.end(), _buffer.begin() + sizeof(std::size_t), _buffer.end());
    _buffer.erase(_buffer.begin(), _buffer.begin() + sizeof(std::size_t) + size);
    return PizzaEncoder::unpack(pizzaData);
}

bool PizzaIPC::hasPizza()
{
    // Read all bytes
    int bytesAvailable = InterProcessCom::bytesAvailable();
    if (bytesAvailable > 0) {
        auto data = std::make_unique<char[]>(bytesAvailable);
        InterProcessCom::read(data.get(), bytesAvailable);
        _buffer.insert(_buffer.end(), data.get(), data.get() + bytesAvailable);
    }

    // Check if there is enough bytes to read the size
    if (_buffer.size() < sizeof(std::size_t))
        return false;
    std::size_t size = *reinterpret_cast<std::size_t *>(_buffer.data());

    // Check if there is enough bytes to read the pizza
    if (_buffer.size() < sizeof(std::size_t) + size)
        return false;
    return true;
}
