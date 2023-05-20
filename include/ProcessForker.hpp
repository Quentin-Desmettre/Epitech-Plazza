/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** ProcessForker
*/

#ifndef EPITECH_PLAZZA_PROCESSFORKER_HPP
#define EPITECH_PLAZZA_PROCESSFORKER_HPP
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <stdexcept>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>

class Process {
public:

    template<typename T, typename ... Args>
    static void run(T &&func, Args ...args)
    {
        int pid;

        if ((pid = fork()) == -1)
            throw std::runtime_error("fork() failed");
        if (pid == 0) {
            func(args...);
            exit(0);
        }
    }

    template<class Object, typename T, typename ... Args>
    static void runObject(Object *obj, T &&func, Args ...args)
    {
        int pid;

        if ((pid = fork()) == -1)
            throw std::runtime_error("fork() failed");
        if (pid == 0) {
            (obj->*func)(args...);
            exit(0);
        }
    }

    static void exit(int status = 0)
    {
        ::exit(status);
    }

private:
    Process() = default;
};

#endif //EPITECH_PLAZZA_PROCESSFORKER_HPP
