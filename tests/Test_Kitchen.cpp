/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_Pizza
*/

#include "Kitchen.hpp"
#include "doctest.h"

TEST_CASE("Test_Kitchen")
{
    Kitchen kitchen(2, 5, 1);
    CHECK(kitchen.getCapacity() == 4);

    Kitchen kitchen2(4, 5, 2);
    CHECK(kitchen2.getCapacity() == 8);
}

TEST_CASE("Test_addPizza")
{
    Kitchen kitchen(2, 5, 1);
    Pizza pizza = Pizza(Pizza::PizzaType::Regina, Pizza::PizzaSize::S, 1, {Pizza::Ingredient::Doe, Pizza::Ingredient::Tomato});

    kitchen.addPizza(pizza);
    CHECK(kitchen.getPizzasAwaiting() == 1);

    kitchen.getPizza();
    CHECK(kitchen.getPizzasAwaiting() == 0);
}

TEST_CASE("Test_IDS")
{
    Kitchen kitchen(2, 5, 1);
    Kitchen kitchen2(2, 5, 1);

    CHECK(kitchen.getId() != kitchen2.getId());
}