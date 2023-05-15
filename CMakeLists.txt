cmake_minimum_required(VERSION 3.24.2)
project(Epitech_Plazza)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(include/ipc)
include_directories(include/pizza)

add_executable(Epitech_Plazza
        src/main.cpp include/ProcessForker.hpp
        include/Reception.hpp
        src/pizza/Pizza.cpp
        include/pizza/Pizza.hpp
        src/pizza/PizzaFactory.cpp
        include/pizza/PizzaFactory.hpp
        src/pizza/PizzaEncoder.cpp
        include/pizza/PizzaEncoder.hpp
        src/ipc/PizzaIPC.cpp
        include/ipc/PizzaIPC.hpp
        src/ipc/InterProcessCom.cpp
        include/ipc/InterProcessCom.hpp
        include/Kitchen.hpp
        include/CookPool.hpp
        src/Kitchen.cpp
        src/Reception.cpp
        src/CookPool.cpp
        src/ParsePizza.cpp
        include/pizza/ParsePizza.hpp
        src/Reception.cpp
        include/logging/ILogger.hpp
        src/logging/ILogger.cpp
        src/logging/CliLogger.cpp
        include/logging/CliLogger.hpp
        tests/Test_CliLogger.cpp
)
