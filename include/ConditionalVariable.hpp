/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** ConditionalVariable
*/

#ifndef EPITECH_PLAZZA_CONDITIONALVARIABLE_HPP
#define EPITECH_PLAZZA_CONDITIONALVARIABLE_HPP
#include "Semaphore.hpp"
#include <mutex>

template <typename T>
class ConditionalVariable {
public:
    ConditionalVariable() = default;
    ~ConditionalVariable() = default;

    T *wait() {
        _mutex.lock();
        return &_value;
    }

    void signal() {
        _mutex.unlock();
    }

private:
    T _value;
    std::mutex _mutex;
};

#endif //EPITECH_PLAZZA_CONDITIONALVARIABLE_HPP
