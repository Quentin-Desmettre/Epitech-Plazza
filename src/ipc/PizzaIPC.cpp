/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaIPC
*/

#include "ipc/PizzaIPC.hpp"
#include "pizza/PizzaEncoder.hpp"

void PizzaIPC::sendPizza(const Pizza &pizza)
{
    std::vector<char> data = PizzaEncoder::_2pac(pizza);
    std::size_t size = data.size();
    InterProcessCom::write(&size, sizeof(std::size_t));
    InterProcessCom::write(data.data(), data.size());
}

Pizza PizzaIPC::receivePizza()
{
    std::size_t size;
    InterProcessCom::read(&size, sizeof(std::size_t));

    std::vector<char> data(size);
    InterProcessCom::read(data.data(), size);
    return PizzaEncoder::unpack(data);
}
