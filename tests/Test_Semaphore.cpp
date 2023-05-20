/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_Semaphore
*/

#include "doctest.h"
#include "Semaphore.hpp"

TEST_CASE("semaphore")
{
    SUBCASE("increment decrement") {
        Semaphore sem(0);

        CHECK_EQ(sem.getValue(), 0);
        sem.increment();
        CHECK_EQ(sem.getValue(), 1);
        sem.decrement();
        CHECK_EQ(sem.getValue(), 0);
    }
    SUBCASE("increment decrement multiple") {
        Semaphore sem(60);

        CHECK_EQ(sem.getValue(), 60);
        sem.increment();
        sem.increment();
        sem.increment();
        CHECK_EQ(sem.getValue(), 63);
        sem.decrement();
        sem.decrement();
        CHECK_EQ(sem.getValue(), 61);
    }
}
