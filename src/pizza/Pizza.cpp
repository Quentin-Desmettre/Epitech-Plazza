/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Pizza
*/

#include "pizza/Pizza.hpp"

const std::map<Pizza::PizzaType, std::string> Pizza::_pizzaTypeToString = {
    {Pizza::Margarita,   "Margarita"},
    {Pizza::Regina,      "Regina"},
    {Pizza::Americana,   "Americana"},
    {Pizza::Fantasia,    "Fantasia"}
};

const std::map<Pizza::PizzaSize, std::string> Pizza::_pizzaSizeToString = {
    {Pizza::S,   "S"},
    {Pizza::M,   "M"},
    {Pizza::L,   "L"},
    {Pizza::XL,  "XL"},
    {Pizza::XXL, "XXL"}
};

const std::map<Pizza::Ingredient, std::string> Pizza::_ingredientToString = {
    {Pizza::Doe,        "Doe"},
    {Pizza::Tomato,     "Tomato"},
    {Pizza::Gruyere,    "Gruyere"},
    {Pizza::Ham,        "Ham"},
    {Pizza::Mushrooms,  "Mushrooms"},
    {Pizza::Steak,      "Steak"},
    {Pizza::Eggplant,   "Eggplant"},
    {Pizza::GoatCheese, "GoatCheese"},
    {Pizza::ChiefLove,  "ChiefLove"}
};

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

std::string Pizza::toString() const
{
    return _pizzaTypeToString.at(_type) + " " + _pizzaSizeToString.at(_size);
}
