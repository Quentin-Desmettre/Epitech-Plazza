/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_IPC
*/

#include "doctest.h"
#include "ipc/InterProcessCom.hpp"
#include <memory>
#include <iostream>
#include <thread>
#include <unistd.h>

void openIpc(InterProcessCom &com, InterProcessCom::OpenMode mode)
{
    com.open(mode);
}

void readData(InterProcessCom &com, char *buffer, size_t size)
{
    com.read(buffer, size);
}

void sendData(InterProcessCom &com, const char *data, size_t size)
{
    com.write(data, size);
}

TEST_CASE("interProcessCom")
{
    InterProcessCom ipc;
    InterProcessCom ipc2(ipc);

    {
        std::thread t1(openIpc, std::ref(ipc), InterProcessCom::READ);
        std::thread t2(openIpc, std::ref(ipc2), InterProcessCom::WRITE);
        t1.join();
        t2.join();
    }

    // Send data
    {
        std::thread t1(sendData, std::ref(ipc2), "Hello World", 12);
        char buffer[1024];
        std::thread t2(readData, std::ref(ipc), buffer, 12);
        t1.join();
        t2.join();
        CHECK_EQ(std::string(buffer), "Hello World");
    }

    // Send enormous data
    {
        std::string msg(1000 * 1000 * 1000, 'a');
        std::thread t1(sendData, std::ref(ipc2), msg.c_str(), msg.size() + 1);
        char *buffer = new char[1000 * 1000 * 1000 + 1];
        std::thread t2(readData, std::ref(ipc), buffer, 1000 * 1000 * 1000 + 1);
        t1.join();
        t2.join();
        CHECK_EQ(std::string(buffer), msg);
    }

    SUBCASE("invalid write")
    {
        InterProcessCom ipc3;

        ipc3.write(nullptr, 12);
        ipc3.read(nullptr, 12);
    }

    SUBCASE("invalid write 2")
    {
        CHECK_THROWS_AS(ipc.write(nullptr, 12), std::runtime_error);
        CHECK_THROWS_AS(ipc2.read(nullptr, 12), std::runtime_error);
        ipc2.open(InterProcessCom::WRITE);
    }

    SUBCASE("Cannot mkfifo")
    {
        InterProcessCom ipc3;
        unlink(ipc3.getPipeName().c_str());
        CHECK_THROWS_AS(ipc3.open(InterProcessCom::READ), std::runtime_error);
    }
}
