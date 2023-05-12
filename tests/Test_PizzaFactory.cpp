/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_PizzaFactory
*/

#include "doctest.h"
#include <map>
#include "pizza/PizzaFactory.hpp"

TEST_CASE("PizzaFactory")
{
    std::vector<Pizza::PizzaSize> sizes = {Pizza::S, Pizza::M, Pizza::L, Pizza::XL};
    std::vector<Pizza::PizzaType> types = {Pizza::Regina, Pizza::Margarita, Pizza::Americana, Pizza::Fantasia};
    std::map<Pizza::PizzaType, int> cookTimeForPizza = {
        {Pizza::Regina, 2},
        {Pizza::Margarita, 1},
        {Pizza::Americana, 2},
        {Pizza::Fantasia, 4}
    };
    std::map<Pizza::PizzaType, std::vector<Pizza::Ingredient>> ingredientsForPizza = {
        {Pizza::Regina, {Pizza::Doe, Pizza::Tomato, Pizza::Gruyere, Pizza::Ham, Pizza::Mushrooms}},
        {Pizza::Margarita, {Pizza::Doe, Pizza::Tomato, Pizza::Gruyere}},
        {Pizza::Americana, {Pizza::Doe, Pizza::Tomato, Pizza::Gruyere, Pizza::Steak}},
        {Pizza::Fantasia, {Pizza::Doe, Pizza::Tomato, Pizza::Eggplant, Pizza::GoatCheese, Pizza::ChiefLove}}
    };

    // Create all pizzas
    for (auto &size : sizes) {
        for (auto &type : types) {
            Pizza p = PizzaFactory::createPizza(type, size);
            CHECK_EQ(p.getSize(), size);
            CHECK_EQ(p.getType(), type);
            CHECK_EQ(p.getCookTime(), cookTimeForPizza[type]);
            CHECK_EQ(p.getIngredients(), ingredientsForPizza[type]);
        }
    }

    SUBCASE("PizzaFactory::createPizza() should throw an exception if the pizza type is unknown")
    {
        CHECK_THROWS(PizzaFactory::createPizza(static_cast<Pizza::PizzaType>(0), Pizza::S));
    }
}
