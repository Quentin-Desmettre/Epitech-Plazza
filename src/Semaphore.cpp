/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Semaphore
*/

#include "Semaphore.hpp"

Semaphore::Semaphore(int value)
{
    if (sem_init(&_sem, 0, value) == -1)
        throw std::runtime_error("sem_init failed");
}

Semaphore::~Semaphore()
{
    sem_destroy(&_sem);
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
