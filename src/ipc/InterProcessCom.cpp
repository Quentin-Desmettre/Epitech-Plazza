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
#include <algorithm>
#include <sys/ioctl.h>
#include <unistd.h>

std::vector<std::string> InterProcessCom::_pipes;

InterProcessCom::InterProcessCom(): _fd(-1)
{
    // First, find a random name for the named pipe.
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, INT32_MAX);
    do {
        int random = distribution(generator);
        _name = "/tmp/Plazza-fifo-" + std::to_string(random);
    } while (std::find(_pipes.begin(), _pipes.end(), _name) != _pipes.end());
    _pipes.push_back(_name);

    // Then, create and open the named pipe.
    if (mkfifo(_name.c_str(), 0666) == -1)
        throw std::runtime_error("Cannot create named pipe");
}

InterProcessCom::InterProcessCom(const InterProcessCom &other): _name(other._name), _fd(-1)
{
    _name = other._name;
}

InterProcessCom::~InterProcessCom()
{
    close();
}

#include <iostream>
void InterProcessCom::open(InterProcessCom::OpenMode mode)
{
    std::cout << "Open ipcs" << std::endl;
    std::cout << "fd:  " << _fd << std::endl;
    if (_fd != -1)
        ::close(_fd);
    std::cout << "Open ipcs 2" << std::endl;
    _fd = ::open(_name.c_str(), mode == READ ? O_RDONLY : O_WRONLY);
    _mode = mode;

    std::cout << "Open ipcs 3" << std::endl;
    if (_fd == -1)
        throw std::runtime_error("Cannot open named pipe");
}

void InterProcessCom::close()
{
    if (_fd == -1)
        return;
    ::close(_fd);
    _fd = -1;
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

int InterProcessCom::bytesAvailable() const
{
    if (_mode == WRITE)
        return 0;
    int bytes = 0;

    if (ioctl(_fd, FIONREAD, &bytes) == -1)
        throw std::runtime_error("Cannot get number of bytes available in named pipe");
    return bytes;
}

void InterProcessCom::erasePipes()
{
    for (const std::string &pipe : _pipes)
        unlink(pipe.c_str());
}

std::string InterProcessCom::getPipeName() const
{
    return _name;
}
