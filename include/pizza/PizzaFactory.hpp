/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** PizzaFactory
*/

#ifndef EPITECH_PLAZZA_PIZZAFACTORY_HPP
#define EPITECH_PLAZZA_PIZZAFACTORY_HPP
#include <string>
#include <map>
#include "pizza/Pizza.hpp"

class PizzaFactory {
public:
    static Pizza createPizza(Pizza::PizzaType type, Pizza::PizzaSize size);

private:
    static Pizza createMargarita(Pizza::PizzaSize size);
    static Pizza createRegina(Pizza::PizzaSize size);
    static Pizza createAmericana(Pizza::PizzaSize size);
    static Pizza createFantasia(Pizza::PizzaSize size);

    typedef Pizza (*PizzaCreator)(Pizza::PizzaSize size);
    static std::map<std::string, PizzaFactory::PizzaCreator> _pizzaCreators;
};

#endif //EPITECH_PLAZZA_PIZZAFACTORY_HPP
