/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaIPC
*/

#ifndef EPITECH_PLAZZA_PIZZAIPC_HPP
#define EPITECH_PLAZZA_PIZZAIPC_HPP
#include "ipc/InterProcessCom.hpp"
#include "pizza/Pizza.hpp"

class PizzaIPC: public InterProcessCom {
public:
    PizzaIPC();
    ~PizzaIPC();

    /**
     * @brief Sends a pizza to the kitchen.
     *
     * Encodes it using PizzaEncoder::pack, and sends it using InterProcessCom::write.
     * @param pizza
     */
    void sendPizza(const Pizza &pizza);

    /**
     * @brief Receives a pizza from the kitchen.
     *
     * Reads the data using InterProcessCom::read, and decodes it using PizzaEncoder::unpack.
     * @return The received pizza.
     */
    Pizza receivePizza();
};

#endif //EPITECH_PLAZZA_PIZZAIPC_HPP
