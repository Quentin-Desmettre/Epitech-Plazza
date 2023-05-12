/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_Pizza
*/

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "pizza/Pizza.hpp"

TEST_CASE("Test_Pizza")
{
    Pizza p;

    CHECK_EQ(p.getSize(), 0);
    CHECK_EQ(p.getType(), 0);
    CHECK_EQ(p.getCookTime(), 0);
    CHECK_EQ(p.getIngredients().size(), 0);

    p.setSize(Pizza::XL);
    p.setType(Pizza::Regina);
    p.setCookTime(1.5);
    p.addIngredient(Pizza::Tomato);
    CHECK_EQ(p.getSize(), Pizza::XL);
    CHECK_EQ(p.getType(), Pizza::Regina);
    CHECK_EQ(p.getCookTime(), 1.5);
    CHECK_EQ(p.getIngredients().size(), 1);
    CHECK_EQ(p.getIngredients()[0], Pizza::Tomato);
    p.setCookTime(-1);
    CHECK_EQ(p.getCookTime(), -1);
    p.addIngredient(Pizza::Tomato);
    CHECK_EQ(p.getIngredients().size(), 2);
    CHECK_EQ(p.getIngredients()[0], Pizza::Tomato);
    CHECK_EQ(p.getIngredients()[1], Pizza::Tomato);
    p.addIngredient(Pizza::Doe);
    CHECK_EQ(p.getIngredients().size(), 3);
    CHECK_EQ(p.getIngredients()[0], Pizza::Tomato);
    CHECK_EQ(p.getIngredients()[1], Pizza::Tomato);
    CHECK_EQ(p.getIngredients()[2], Pizza::Doe);
}

TEST_CASE("Test_pizza_constructor")
{
    Pizza p(Pizza::Regina, Pizza::XL, 1.5, {Pizza::Tomato, Pizza::Tomato, Pizza::Doe});

    CHECK_EQ(p.getSize(), Pizza::XL);
    CHECK_EQ(p.getType(), Pizza::Regina);
    CHECK_EQ(p.getCookTime(), 1.5);
    CHECK_EQ(p.getIngredients().size(), 3);
    CHECK_EQ(p.getIngredients()[0], Pizza::Tomato);
    CHECK_EQ(p.getIngredients()[1], Pizza::Tomato);
    CHECK_EQ(p.getIngredients()[2], Pizza::Doe);
}
