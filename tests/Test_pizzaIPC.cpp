/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_pizzaIPC
*/

#include "doctest.h"
#include "ipc/PizzaIPC.hpp"
#include <thread>
#include <iostream>

void openIpc(InterProcessCom &com, InterProcessCom::OpenMode mode);

TEST_CASE("Test_pizzaIPC")
{
    Pizza p(Pizza::Regina, Pizza::XL, 1.5, {Pizza::Tomato, Pizza::Tomato, Pizza::Doe});
    PizzaIPC ipcRead;
    PizzaIPC ipcWrite(ipcRead);

    // Open IPC
    {
        std::thread t1(openIpc, std::ref(ipcRead), InterProcessCom::READ);
        std::thread t2(openIpc, std::ref(ipcWrite), InterProcessCom::WRITE);
        t1.join();
        t2.join();
    }

    SUBCASE("send and wait for notif") {
        std::thread t1(&PizzaIPC::notifyMessageReceived, std::ref(ipcWrite));
        std::thread t2(&PizzaIPC::waitForNotification, std::ref(ipcRead));
        t1.join();
        t2.join();
    }

    // Send pizza
    SUBCASE("send pizza")
    {
        Pizza p2;
        std::thread t1([](PizzaIPC &ipcRead, Pizza *p2) {
            auto type = ipcRead.getRequestType();
            *p2 = ipcRead.receivePizza();
        }, std::ref(ipcRead), &p2);
        std::thread t2(&PizzaIPC::sendPizza, std::ref(ipcWrite), p);

        t1.join();
        t2.join();
        CHECK_EQ(p2.getType(), p.getType());
        CHECK_EQ(p2.getSize(), p.getSize());
        CHECK_EQ(p2.getCookTime(), p.getCookTime());
        CHECK_EQ(p2.getIngredients(), p.getIngredients());
        ipcWrite.requestCooksOccupancy();
    }
}
