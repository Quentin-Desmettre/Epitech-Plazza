/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Pizza
*/

#include "pizza/Pizza.hpp"

Pizza::Pizza(Pizza::PizzaType type, Pizza::PizzaSize size, float cookTime,
             const std::vector<Ingredient> &ingredients)
{
    _type = type;
    _size = size;
    _ingredients = ingredients;
    _cookTime = cookTime;
}

Pizza::Pizza()
{
    _type = (Pizza::PizzaType)0;
    _size = (Pizza::PizzaSize)0;
    _cookTime = 0;
}

Pizza::PizzaType Pizza::getType() const
{
    return _type;
}

Pizza::PizzaSize Pizza::getSize() const
{
    return _size;
}

std::vector<Pizza::Ingredient> Pizza::getIngredients() const
{
    return _ingredients;
}

float Pizza::getCookTime() const
{
    return _cookTime;
}

Pizza &Pizza::setType(Pizza::PizzaType type)
{
    _type = type;
    return *this;
}

Pizza &Pizza::setSize(Pizza::PizzaSize size)
{
    _size = size;
    return *this;
}

Pizza &Pizza::addIngredient(Pizza::Ingredient ingredient)
{
    _ingredients.push_back(ingredient);
    return *this;
}

Pizza &Pizza::setCookTime(float cookTime)
{
    _cookTime = cookTime;
    return *this;
}
