/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** InterProcessCom
*/

#include "ipc/InterProcessCom.hpp"
#include <random>
#include <sys/stat.h>
#include <stdexcept>
#include <fcntl.h>
#include <csignal>

InterProcessCom::InterProcessCom(): _fd(-1)
{
    // First, find a random name for the named pipe.
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, INT32_MAX);
    do {
        int random = distribution(generator);
        _name = "/tmp/Plazza-fifo-" + std::to_string(random);
    } while (_pipes.find(_name) != _pipes.end());

    // Then, create and open the named pipe.
    if (mkfifo(_name.c_str(), 0666) == -1)
        throw std::runtime_error("Cannot create named pipe");
}

InterProcessCom::~InterProcessCom()
{
    close();

    // Check if the named pipe is not used anymore.
    if (_pipes[_name] == 0) {
        unlink(_name.c_str());
        _pipes.erase(_name);
    }
}

void InterProcessCom::open(InterProcessCom::OpenMode mode)
{
    if (_fd != -1)
        return;
    _fd = ::open(_name.c_str(), mode == READ ? O_RDONLY : O_WRONLY);
    if (_fd == -1)
        throw std::runtime_error("Cannot open named pipe");

    // Increment the reference count of the named pipe.
    if (_pipes.find(_name) == _pipes.end())
        _pipes[_name] = 0;
    _pipes[_name]++;
}

void InterProcessCom::close()
{
    if (_fd == -1)
        return;
    ::close(_fd);
    _fd = -1;
    _pipes[_name]--;
}

void InterProcessCom::write(const void *data, size_t size) const
{
    if (_fd == -1)
        return;

    std::size_t written = 0;
    while (written < size) {
        ssize_t ret = ::write(_fd, (char *)data + written, size - written);
        if (ret == -1)
            throw std::runtime_error("Cannot write to named pipe");
        written += ret;
    }
}

void InterProcessCom::read(void *data, size_t size) const
{
    if (_fd == -1)
        return;

    std::size_t read = 0;
    while (read < size) {
        ssize_t ret = ::read(_fd, (char *)data + read, size - read);
        if (ret == -1)
            throw std::runtime_error("Cannot read from named pipe");
        read += ret;
    }
}

std::map<std::string, int> InterProcessCom::_pipes = std::map<std::string, int>();
