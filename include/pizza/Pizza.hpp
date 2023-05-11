/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Pizza
*/

#ifndef EPITECH_PLAZZA_PIZZA_HPP
#define EPITECH_PLAZZA_PIZZA_HPP
#include <vector>

class Pizza {
public:
    enum PizzaType
    {
        Regina = 1 ,
        Margarita = 2 ,
        Americana = 4 ,
        Fantasia = 8
    };
    enum PizzaSize
    {
        S = 1 ,
        M = 2 ,
        L = 4 ,
        XL = 8 ,
        XXL = 16
    };
    enum Ingredient {
        Doe,
        Tomato,
        Gruyere,
        Ham,
        Mushrooms,
        Steak,
        Eggplant,
        GoatCheese,
        ChiefLove
    };

    Pizza(PizzaType type, PizzaSize size, std::vector<Ingredient> ingredients);

    PizzaType getType() const;
    PizzaSize getSize() const;
    std::vector<Ingredient> getIngredients() const;

    Pizza &setType(PizzaType type);
    Pizza &setSize(PizzaSize size);
    Pizza &addIngredient(Ingredient ingredient);

private:
    PizzaType _type;
    PizzaSize _size;
    std::vector<Ingredient> _ingredients;
};

#endif //EPITECH_PLAZZA_PIZZA_HPP
