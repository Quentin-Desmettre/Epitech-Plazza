/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaEncoder
*/

#ifndef EPITECH_PLAZZA_PIZZAENCODER_HPP
#define EPITECH_PLAZZA_PIZZAENCODER_HPP
#include "pizza/Pizza.hpp"

class PizzaEncoder {
public:
    static std::vector<char> _2pac(const Pizza &pizza);
    static Pizza unpack(const std::vector<char> &data);
};

#endif //EPITECH_PLAZZA_PIZZAENCODER_HPP
