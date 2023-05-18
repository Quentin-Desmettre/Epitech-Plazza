/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Semaphore
*/

#include "Semaphore.hpp"

Semaphore::Semaphore(int value)
{
    #ifdef __APPLE__
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, INT32_MAX);
    int random = distribution(generator);
    _name = "semaphore/" + std::to_string(random);
    _sem = *sem_open(_name.c_str(), value);
    #else
    if (sem_init(&_sem, 0, value) == -1)
        throw std::runtime_error("sem_init failed");
    #endif
}

Semaphore::~Semaphore()
{
    #ifdef __APPLE__
    sem_close(&_sem);
    sem_unlink(_name.c_str());
    #else
    sem_destroy(&_sem);
    #endif
}

void Semaphore::increment()
{
    sem_post(&_sem);
}

void Semaphore::decrement()
{
    sem_wait(&_sem);
}

int Semaphore::getValue() const
{
    int value;

    sem_getvalue((sem_t *)&_sem, &value);
    return value;
}
