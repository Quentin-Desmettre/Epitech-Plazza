/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaIPC
*/

#include "ipc/PizzaIPC.hpp"
#include "pizza/PizzaEncoder.hpp"
#include <memory>

void PizzaIPC::sendPizza(const Pizza &pizza)
{
    std::vector<char> data = PizzaEncoder::_2pac(pizza);
    std::size_t size = data.size();
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

void PizzaIPC::notifyMessageReceived() const
{
    InterProcessCom::write("1", 1);
}

void PizzaIPC::waitForNotification() const
{
    char buffer;
    InterProcessCom::read(&buffer, 1);
}
