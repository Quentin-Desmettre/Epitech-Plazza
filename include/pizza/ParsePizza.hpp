/*
** EPITECH PROJECT, 2023
** plazza
** File description:
** ParsePizza.hpp
*/

#ifndef PLAZZA_PARSEPIZZA_HPP
#define PLAZZA_PARSEPIZZA_HPP

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <regex>
#include <map>

class ParsePizza {
    public:
        ParsePizza() = default;
        ~ParsePizza() = default;
        bool CreatePizza(std::string &pizzaName, std::string &pizzaSize,
            std::string &pizzaNumber);
        bool RunChecker(std::string &input);
        bool CheckInput();
        void SplitInput(std::string &input);
        std::vector<Pizza> GetPizzas();
    private:
        std::vector<std::string> _splitInput{};
        std::vector<Pizza> _pizzas{};
};

#endif //PLAZZA_PARSEPIZZA_HPP