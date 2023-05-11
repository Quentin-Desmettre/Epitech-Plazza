/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_pizzaIPC
*/

#include "doctest.h"
#include "ipc/PizzaIPC.hpp"
#include <thread>

void openIpc(InterProcessCom &com, InterProcessCom::OpenMode mode);

void sendPizza(PizzaIPC &com, const Pizza &pizza)
{
    com.sendPizza(pizza);
}

void readPizza(PizzaIPC &com, Pizza &pizza)
{
    pizza = com.receivePizza();
}

TEST_CASE("Test_pizzaIPC")
{
    Pizza p(Pizza::Regina, Pizza::XL, 1.5, {Pizza::Tomato, Pizza::Tomato, Pizza::Doe});
    PizzaIPC ipcRead;
    PizzaIPC ipcWrite(ipcRead);

    {
        std::thread t1(openIpc, std::ref(ipcRead), InterProcessCom::READ);
        std::thread t2(openIpc, std::ref(ipcWrite), InterProcessCom::WRITE);
        t1.join();
        t2.join();
    }

    // Send pizza
    SUBCASE("send pizza")
    {
        std::thread t1(sendPizza, std::ref(ipcWrite), p);
        Pizza p2;
        std::thread t2(readPizza, std::ref(ipcRead), std::ref(p2));
        t1.join();
        t2.join();
        CHECK_EQ(p2.getType(), p.getType());
        CHECK_EQ(p2.getSize(), p.getSize());
        CHECK_EQ(p2.getCookTime(), p.getCookTime());
        CHECK_EQ(p2.getIngredients(), p.getIngredients());
    }
}
