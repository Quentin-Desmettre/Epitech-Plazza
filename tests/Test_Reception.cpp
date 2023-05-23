/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_Pizza
*/

#include "Reception.hpp"
#include "doctest.h"

TEST_CASE("Test_Reception")
{
    char *test[] = {"./plazza", "0", "5", "2000"};
    CHECK_THROWS(Reception(4, test));

    char *test2[] = {"./plazza", "2", "0", "2000"};
    CHECK_THROWS(Reception(4, test2));

    char *test3[] = {"./plazza", "2", "5", "0"};
    CHECK_THROWS(Reception(4, test3));

    char *test4[] = {"./plazza", "2", "5", "2000", "2000"};
    CHECK_THROWS(Reception(4, test4));

    char *test5[] = {"./plazza", "2", "5", "2000", "2000"};
    CHECK_THROWS(Reception(3, test5));
}

TEST_CASE("Test_ReceptionParser")
{
    ParsePizza parser;

    std::string test = "regina XXL x2";
    CHECK_NOTHROW(parser.RunChecker(test));
    CHECK(parser.GetPizzas().size() == 2);

    std::string test2 = "regina XXL x2; fantasia M x1";
    CHECK_NOTHROW(parser.RunChecker(test2));
    CHECK(parser.GetPizzas().size() == 3);

    std::string test3 = "regina XXL x2; fantasia M x1; margarita S x1";
    CHECK_NOTHROW(parser.RunChecker(test3));
    CHECK(parser.GetPizzas().size() == 4);

    std::string test4 = "regina XXL x2; ";
    CHECK_NOTHROW(parser.RunChecker(test4));
    CHECK(parser.GetPizzas().size() == 0);

    std::string test5 = "exit";
    CHECK_THROWS(parser.RunChecker(test5));
}
