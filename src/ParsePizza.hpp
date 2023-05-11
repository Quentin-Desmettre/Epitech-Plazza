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

class ParsePizza {
    public:
        ParsePizza() = default;
        ~ParsePizza() = default;
        bool RunChecker(std::string &input);
        bool CheckInput();
        void SplitInput(std::string &input);
        void GetPizzas();
    private:
        std::vector<std::string> _splitInput{};
        std::vector<std::string> _pizzas{};
};

#endif //PLAZZA_PARSEPIZZA_HPP