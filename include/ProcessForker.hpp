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
    Process(): _pid(INT32_MAX) {}
    template<typename T, typename ... Args>
    static Process run(T &&func, Args ...args)
    {
        pid_t pid;

        if ((pid = fork()) == -1)
            throw std::runtime_error("fork() failed");
        if (pid == 0) {
            func(args...);
            exit(0);
        }
        return Process(pid);
    }

    template<class Object, typename T, typename ... Args>
    static Process runObject(Object *obj, T &&func, Args ...args)
    {
        pid_t pid;

        if ((pid = fork()) == -1)
            throw std::runtime_error("fork() failed");
        if (pid == 0) {
            (obj->*func)(args...);
            exit(0);
        }
        return Process(pid);
    }

    static void exit(int status = 0)
    {
        ::exit(status);
    }

    void kill() const {
        ::kill(_pid, SIGKILL);
        ::waitpid(_pid, nullptr, 0);
    }

private:
    explicit Process(pid_t pid) : _pid(pid) {}
    pid_t _pid;
};

#endif //EPITECH_PLAZZA_PROCESSFORKER_HPP
