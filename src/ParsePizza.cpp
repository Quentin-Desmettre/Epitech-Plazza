/*
** EPITECH PROJECT, 2023
** PLAZZA_PARSEPIZZA_CPP
** File description:
** ParsePizza.cpp
*/

#include "ParsePizza.hpp"

bool ParsePizza::CheckInput()
{
    std::string pizzaName;
    std::string pizzaSize;
    std::string pizzaNumber;
    std::string pizzaError;

    for (auto &it : _splitInput) {
        std::regex pizzaRegex("^([a-zA-Z]+) (S|M|L|XL|XXL) (x[1-9][0-9]*)$");
        std::smatch pizzaMatchs;
        if (std::regex_search(it, pizzaMatchs, pizzaRegex)) {
            _pizzas.push_back(it);
            pizzaName = pizzaMatchs[1];
            pizzaSize = pizzaMatchs[2];
            pizzaNumber = pizzaMatchs[3];
            std::cout << "Name: " << pizzaName << std::endl;
            std::cout << "Size: " << pizzaSize << std::endl;
            std::cout << "Number: " << pizzaNumber << std::endl;
        } else {
            _pizzas.clear();
            std::cerr << "Error: " << it << std::endl;
            return false;
        }
    }
    return true;
}

void ParsePizza::SplitInput(std::string &input)
{
    std::string delimiter = " ; ";
    size_t pos = 0;
    std::string token;

    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        _splitInput.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    token = input.substr(0, pos);
    _splitInput.push_back(token);
}

void ParsePizza::GetPizzas()
{
    for (auto &it : _pizzas) {
        std::cout << it << std::endl;
    }
}

bool ParsePizza::RunChecker(std::string &input)
{
    SplitInput(input);
    return CheckInput();
}