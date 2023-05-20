/*
** EPITECH PROJECT, 2023
** PLAZZA_PARSEPIZZA_CPP
** File description:
** ParsePizza.cpp
*/

#include "pizza/ParsePizza.hpp"
#include "pizza/PizzaFactory.hpp"

const std::map<std::string, Pizza::PizzaType> ParsePizza::_pizzaTypeMap = {
        {"regina", Pizza::PizzaType::Regina},
        {"margarita", Pizza::PizzaType::Margarita},
        {"americana", Pizza::PizzaType::Americana},
        {"fantasia", Pizza::PizzaType::Fantasia}
};

const std::map<std::string, Pizza::PizzaSize> ParsePizza::_pizzaSizeMap = {
        {"S", Pizza::PizzaSize::S},
        {"M", Pizza::PizzaSize::M},
        {"L", Pizza::PizzaSize::L},
        {"XL", Pizza::PizzaSize::XL},
        {"XXL", Pizza::PizzaSize::XXL}
};

bool ParsePizza::CreatePizza(std::string &pizzaName, std::string &pizzaSize,
    std::string &pizzaNumber)
{
    int number = 1;
    if (pizzaNumber[0] == 'x')
        number = std::stoi(pizzaNumber.substr(1, pizzaNumber.length() - 1));

    if (_pizzaTypeMap.find(pizzaName) == _pizzaTypeMap.end()) {
        _pizzas.clear();
        return false;
    }
    std::transform(pizzaName.begin(), pizzaName.end(), pizzaName.begin(), ::tolower);
    Pizza::PizzaType type = _pizzaTypeMap.at(pizzaName);
    Pizza::PizzaSize size = _pizzaSizeMap.at(pizzaSize);

    for (int i = 0; i < number; i++) {
        _pizzas.push_back(PizzaFactory::createPizza(type, size));
    }
    return true;
}

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
            pizzaName = pizzaMatchs[1];
            pizzaSize = pizzaMatchs[2];
            pizzaNumber = pizzaMatchs[3];
            if (!CreatePizza(pizzaName, pizzaSize, pizzaNumber)) {
                _pizzas.clear();
                return false;
            }
        } else {
            _pizzas.clear();
            return false;
        }
    }
    return true;
}

void ParsePizza::SplitInput(std::string &input)
{
    std::string delimiter = "; ";
    size_t pos = input.find(delimiter);
    std::string token;

    while (pos != std::string::npos) {
        token = input.substr(0, pos);
        _splitInput.push_back(token);
        input.erase(0, pos + delimiter.length());
        pos = input.find(delimiter);
    }
    token = input.substr(0, pos);
    _splitInput.push_back(token);
}

std::vector<Pizza> ParsePizza::GetPizzas()
{
    return _pizzas;
}

bool ParsePizza::RunChecker(std::string &input)
{
    _pizzas.clear();
    _splitInput.clear();
    SplitInput(input);
    return CheckInput();
}