/*
** EPITECH PROJECT, 2022
** Epitech-Template
** File description:
** main.c
*/

#include <stdio.h>
#include <string>
#include <iostream>
#include "ParsePizza.hpp"

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
#include <semaphore>
#include <thread>

int main()
{
    std::string pizzaName;

    std::getline(std::cin, pizzaName);
    ParsePizza parsePizza;
    while (pizzaName != "exit") {
        if (parsePizza.RunChecker(pizzaName))
            parsePizza.GetPizzas();
        else {
            std::cout << "Error: " << pizzaName << std::endl;
        }
        std::getline(std::cin, pizzaName);
    }
    std::counting_semaphore<1> sem(0);
    return (0);
}
