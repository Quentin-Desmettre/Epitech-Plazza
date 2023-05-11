/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** ProcessForker
*/

#ifndef EPITECH_PLAZZA_PROCESSFORKER_HPP
#define EPITECH_PLAZZA_PROCESSFORKER_HPP
#include <unistd.h>
#include <cstdlib>
#include <utility>
#include <stdexcept>
#include <sys/types.h>
#include <sys/wait.h>

class Process {
public:
    Process() {
        _pid = -1;
        _isRunnning = false;
    }

    template<typename T, typename ... Args>
    void run(T &&func, Args ...args)
    {
        if (_isRunnning)
            throw std::runtime_error("Process already running");
        if ((_pid = fork()) == -1)
            throw std::runtime_error("fork() failed");
        _isRunnning = true;
        if (_pid == 0) {
            func(args...);
            exit(0);
        }
    }

    template<class Object, typename T, typename ... Args>
    void runObject(Object *obj, T &&func, Args ...args)
    {
        if (_isRunnning)
            throw std::runtime_error("Process already running");
        if ((_pid = fork()) == -1)
            throw std::runtime_error("fork() failed");
        _isRunnning = true;
        if (_pid == 0) {
            (obj->*func)(args...);
            exit(0);
        }
    }

    void join()
    {
        waitpid(_pid, nullptr, 0);
        _isRunnning = false;
    }

private:
    pid_t _pid;
    bool _isRunnning;
};

#endif //EPITECH_PLAZZA_PROCESSFORKER_HPP
