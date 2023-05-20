/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Semaphore
*/

#ifndef EPITECH_PLAZZA_SEMAPHORE_HPP
#define EPITECH_PLAZZA_SEMAPHORE_HPP
#include <semaphore>
#ifdef __APPLE__
    #include <sys/semaphore.h>
#endif
#include <random>
#include <string>


class Semaphore {
    public:
        Semaphore() = default;
        explicit Semaphore(int value);
        ~Semaphore();

        void increment();
        void decrement();
        int getValue() const;

    private:
        sem_t _sem;
        std::string _name;
};


#endif //EPITECH_PLAZZA_SEMAPHORE_HPP
