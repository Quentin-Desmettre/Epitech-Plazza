/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_PizzaQueue
*/

#include "doctest.h"
#include "pizza/PizzaQueue.hpp"

TEST_CASE("PizzaQueue")
{
    PizzaQueue queue;
    Pizza p1(Pizza::PizzaType::Regina, Pizza::PizzaSize::S, 1, {Pizza::Ingredient::Doe, Pizza::Ingredient::Tomato});
    Pizza p2(Pizza::PizzaType::Regina, Pizza::PizzaSize::XL, 1, {Pizza::Ingredient::Doe, Pizza::Ingredient::Tomato});

    CHECK(queue.empty());
    CHECK_EQ(queue.size(), 0);

    queue.push(p1);
    CHECK_EQ(queue.size(), 1);
    CHECK(!queue.empty());

    queue.push(p2);
    CHECK_EQ(queue.size(), 2);
    CHECK(!queue.empty());

    auto p = queue.pop();
    CHECK_EQ(queue.size(), 1);
    CHECK_EQ(p, p1);

    p = queue.pop();
    CHECK_EQ(queue.size(), 0);
    CHECK_EQ(p, p2);
    CHECK(queue.empty());
}
