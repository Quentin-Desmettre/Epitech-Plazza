/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_pizzaEncoder
*/

#include "doctest.h"
#include "pizza/PizzaFactory.hpp"
#include "pizza/PizzaEncoder.hpp"

TEST_CASE("Test_pizzaEncoder")
{
    // Encode
    Pizza reginaXl = PizzaFactory::createPizza(Pizza::Regina, Pizza::XL);
    std::vector<char> encodedPizza = PizzaEncoder::_2pac(reginaXl);

    CHECK_EQ(encodedPizza.size(), 2 + sizeof(float) + sizeof(std::size_t) + reginaXl.getIngredients().size());
    CHECK_EQ(encodedPizza[0], Pizza::Regina);
    CHECK_EQ(encodedPizza[1], Pizza::XL);
    CHECK_EQ(*reinterpret_cast<float *>(&encodedPizza[2]), reginaXl.getCookTime());
    CHECK_EQ(*reinterpret_cast<std::size_t *>(&encodedPizza[2 + sizeof(float)]), reginaXl.getIngredients().size());
    CHECK_EQ(encodedPizza[2 + sizeof(float) + sizeof(std::size_t)], Pizza::Doe);
    CHECK_EQ(encodedPizza[2 + sizeof(float) + sizeof(std::size_t) + 1], Pizza::Tomato);
    CHECK_EQ(encodedPizza[2 + sizeof(float) + sizeof(std::size_t) + 2], Pizza::Gruyere);

    // Decode
    Pizza decodedPizza = PizzaEncoder::unpack(encodedPizza);
    CHECK_EQ(decodedPizza.getSize(), Pizza::XL);
    CHECK_EQ(decodedPizza.getType(), Pizza::Regina);
    CHECK_EQ(decodedPizza.getCookTime(), reginaXl.getCookTime());
    CHECK_EQ(decodedPizza.getIngredients().size(), reginaXl.getIngredients().size());
    CHECK_EQ(decodedPizza.getIngredients(), reginaXl.getIngredients());
}
