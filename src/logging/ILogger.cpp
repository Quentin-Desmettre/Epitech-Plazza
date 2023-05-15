/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** ILogger
*/

#include "logging/ILogger.hpp"

std::unique_ptr<ILogger> ILogger::_logger = nullptr;

ILogger &ILogger::getLogger()
{
    if (!ILogger::_logger)
        throw std::runtime_error("Logger not created");
    return *ILogger::_logger;
}
