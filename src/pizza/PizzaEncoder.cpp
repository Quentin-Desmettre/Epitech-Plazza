/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaEncoder
*/

#include <stdexcept>
#include "pizza/PizzaEncoder.hpp"

std::vector<char> PizzaEncoder::_2pac(const Pizza &pizza)
{
    std::vector<char> data;
    float cookTime = pizza.getCookTime();
    std::size_t nbIngredients = pizza.getIngredients().size();

    // General data
    data.push_back(pizza.getType());
    data.push_back(pizza.getSize());

    // Cook time
    for (std::size_t i = 0; i < sizeof(float); i++)
        data.push_back(*((char *)(&cookTime) + i));

    // Ingredients
    for (std::size_t i = 0; i < sizeof(std::size_t); i++)
        data.push_back(*((char *)(&nbIngredients) + i));
    for (auto ingredient : pizza.getIngredients())
        data.push_back(ingredient);
    return data;
}

Pizza PizzaEncoder::unpack(const std::vector<char> &data)
{
    Pizza pizza;

    // Get general data
    pizza.setType((Pizza::PizzaType)data[0]);
    pizza.setSize((Pizza::PizzaSize)data[1]);

    // Get cook time
    float cookTime = *(float *)(data.data() + 2);
    pizza.setCookTime(cookTime);

    // Get ingredients
    std::size_t nbIngredients = *(std::size_t *)(data.data() + 2 + sizeof(float));
    for (std::size_t i = 0; i < nbIngredients; i++)
        pizza.addIngredient((Pizza::Ingredient)data[2 + sizeof(float) + sizeof(std::size_t) + i]);
    return pizza;
}
