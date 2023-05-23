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
    if (value < 0)
        throw std::runtime_error("sem_init failed");
    _value = value;
#else
    if (sem_init(&_sem, 0, value) == -1)
        throw std::runtime_error("sem_init failed");
#endif
}

Semaphore::~Semaphore()
{
#ifdef __APPLE__
    _value = 0;
#else
    sem_destroy(&_sem);
#endif
}

void Semaphore::increment()
{
#ifdef __APPLE__
    _value++;
#else
    sem_post(&_sem);
#endif
}

void Semaphore::decrement()
{
#ifdef __APPLE__
    while (_value == 0);
    _value--;
#else
    sem_wait(&_sem);
#endif
}

int Semaphore::getValue() const
{
#ifdef __APPLE__
    return _value;
#else
    int value;

    sem_getvalue((sem_t *)&_sem, &value);
    return value;
#endif
}
