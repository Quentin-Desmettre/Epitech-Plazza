/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Semaphore
*/

#ifndef EPITECH_PLAZZA_SEMAPHORE_HPP
#define EPITECH_PLAZZA_SEMAPHORE_HPP
#include <semaphore>


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
};


#endif //EPITECH_PLAZZA_SEMAPHORE_HPP
