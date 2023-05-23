/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Test_processForker
*/

#include "doctest.h"
#include "ProcessForker.hpp"
#include <fstream>

TEST_CASE("ProcessForker")
{
    auto p = Process::run([]() {
        // Open a file
        std::ofstream ofs("test.txt", std::ofstream::out);
        ofs << "test" << std::endl;
    });
    usleep(100000);
    p.kill();
    std::ifstream ifs("test.txt", std::ifstream::in);
    std::string str;
    std::getline(ifs, str);
    CHECK_EQ(str, "test");
    ifs.close();

    SUBCASE("Infinite loop, function") {
        auto p = Process::run([]() {
            while (true);
        });
        p.kill();
    }

    SUBCASE("Infinite loop, class") {
        class Test {
            public:
                Test() = default;
                ~Test() = default;
                void run() {
                    while (true);
                }
        };
        Test t;
        auto p = Process::runObject(&t, &Test::run);
        p.kill();
    }

    SUBCASE("exit") {
        auto p = Process::run([]() {
            Process::exit(0);
        });
        p.kill();
    }
}
