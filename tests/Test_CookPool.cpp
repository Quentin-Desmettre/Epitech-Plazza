/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_Pizza
*/

#include "doctest.h"
#include "Kitchen.hpp"

TEST_CASE("Test_CookPool")
{
    Kitchen kitchen(1, 5, 1);
    CHECK_NOTHROW(CookPool cookPool(5, 1, kitchen));

    CookPool cookPool(5, 1, kitchen);
    CHECK(cookPool.getPizzaInCooking() == 0);
    cookPool.addPizza(Pizza(Pizza::Type::Regina, Pizza::Size::S));
    CHECK(cookPool.getPizzaInCooking() == 1);
    cookPool.addPizza(Pizza(Pizza::Type::Regina, Pizza::Size::S));
    CHECK(cookPool.getPizzaInCooking() == 2);
    cookPool.addPizza(Pizza(Pizza::Type::Regina, Pizza::Size::S));
    CHECK(cookPool.getPizzaInCooking() == 3);

    CHECK_NOTHROW(cookPool.getIngredients());
    CHECK_NOTHROW(cookPool.cookThread());

    CHECK(cookPool.getPizzaInCooking() == 0);

    CHECK_NOTHROW(cookPool.getFinishedPizzas());
    CHECK(cookPool.getFinishedPizzas().size() != 0);
}
