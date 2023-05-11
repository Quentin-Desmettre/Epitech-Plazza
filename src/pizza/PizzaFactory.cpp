/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaFactory
*/

#include "pizza/PizzaFactory.hpp"
#include <stdexcept>

const std::map<Pizza::PizzaType, PizzaFactory::PizzaCreator> PizzaFactory::_pizzaCreators = {
    {Pizza::Margarita,   &PizzaFactory::createMargarita},
    {Pizza::Regina,      &PizzaFactory::createRegina},
    {Pizza::Americana,   &PizzaFactory::createAmericana},
    {Pizza::Fantasia,    &PizzaFactory::createFantasia}
};

Pizza PizzaFactory::createPizza(Pizza::PizzaType type, Pizza::PizzaSize size)
{
    if (_pizzaCreators.find(type) == _pizzaCreators.end())
        throw std::runtime_error("Unknown pizza type");
    return _pizzaCreators.at(type)(size);
}

Pizza PizzaFactory::createMargarita(Pizza::PizzaSize size)
{
    return Pizza(Pizza::Margarita, size, 1, {Pizza::Doe, Pizza::Tomato, Pizza::Gruyere});
}

Pizza PizzaFactory::createRegina(Pizza::PizzaSize size)
{
    return Pizza(Pizza::Regina, size, 2, {Pizza::Doe, Pizza::Tomato, Pizza::Gruyere, Pizza::Ham, Pizza::Mushrooms});
}

Pizza PizzaFactory::createAmericana(Pizza::PizzaSize size)
{
    return Pizza(Pizza::Americana, size, 2, {Pizza::Doe, Pizza::Tomato, Pizza::Gruyere, Pizza::Steak});
}

Pizza PizzaFactory::createFantasia(Pizza::PizzaSize size)
{
    return Pizza(Pizza::Fantasia, size, 4, {Pizza::Doe, Pizza::Tomato, Pizza::Eggplant, Pizza::GoatCheese, Pizza::ChiefLove});
}
