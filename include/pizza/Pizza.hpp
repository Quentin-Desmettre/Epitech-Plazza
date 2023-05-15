/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Pizza
*/

#ifndef EPITECH_PLAZZA_PIZZA_HPP
#define EPITECH_PLAZZA_PIZZA_HPP
#include <vector>
#include <string>
#include <map>

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
        ChiefLove,
        IngredientCount
    };

    Pizza();
    Pizza(PizzaType type, PizzaSize size, float cookTime, const std::vector<Ingredient> &ingredients);

    [[nodiscard]] PizzaType getType() const;
    [[nodiscard]] PizzaSize getSize() const;
    [[nodiscard]] float getCookTime() const;
    [[nodiscard]] std::vector<Ingredient> getIngredients() const;

    Pizza &setType(PizzaType type);
    Pizza &setSize(PizzaSize size);
    Pizza &addIngredient(Ingredient ingredient);
    Pizza &setCookTime(float cookTime);

    // Convert to string
    [[nodiscard]] std::string toString() const;

private:
    PizzaType _type;
    PizzaSize _size;
    std::vector<Ingredient> _ingredients;
    float _cookTime;

    const static std::map<PizzaType, std::string> _pizzaTypeToString;
    const static std::map<PizzaSize, std::string> _pizzaSizeToString;
};

#endif //EPITECH_PLAZZA_PIZZA_HPP
