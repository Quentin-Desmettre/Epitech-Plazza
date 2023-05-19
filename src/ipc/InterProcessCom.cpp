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
#include <sys/ioctl.h>
#include <unistd.h>
#include <cerrno>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <csignal>
#include <fcntl.h>
#include <cstring>

std::vector<std::string> InterProcessCom::_pipes;

InterProcessCom::InterProcessCom(): _fd(-1)
{
    // Find a random name for the pipe
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, INT32_MAX);
    do {
        int random = distribution(generator);
        _name = "/tmp/Plazza-unix_socket-" + std::to_string(random);
    } while (std::find(_pipes.begin(), _pipes.end(), _name) != _pipes.end());
    _pipes.push_back(_name);

    // Create named pipe
    if (mkfifo(_name.c_str(), 0666) == -1)
        throw std::runtime_error("Cannot create named pipe. Reason: " + std::string(std::strerror(errno)));
}

InterProcessCom::InterProcessCom(const InterProcessCom &other): _name(other._name), _fd(-1)
{
    _name = other._name;
}

InterProcessCom::~InterProcessCom()
{
    close();
}

void InterProcessCom::open(InterProcessCom::OpenMode mode)
{
    if (_fd != -1)
        this->close();

    // Open named pipe
    _fd = ::open(_name.c_str(), mode == OpenMode::READ ? O_RDONLY : O_WRONLY);
    if (_fd == -1)
        throw std::runtime_error("Cannot open named pipe. Reason: " + std::string(std::strerror(errno)));
    _mode = mode;
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

    // Redirect sigpipe to avoid crashing the program
    signal(SIGPIPE, &InterProcessCom::handleSigPipe);

    // Write data
    std::size_t written = 0;
    while (written < size) {
        ssize_t ret = ::write(_fd, (char *)data + written, size - written);
        if (ret == -1) {
            // Restore sigpipe
            signal(SIGPIPE, SIG_DFL);
            throw std::runtime_error("Cannot write to named pipe. Reason: " + std::string(std::strerror(errno)));
        }
        written += ret;
    }

    // Restore sigpipe
    signal(SIGPIPE, SIG_DFL);
}

void InterProcessCom::read(void *data, size_t size) const
{
    if (_fd == -1)
        return;

    // Redirect sigpipe to avoid crashing the program
    signal(SIGPIPE, &InterProcessCom::handleSigPipe);

    // Read data
    std::size_t read = 0;
    while (read < size) {
        ssize_t ret = ::read(_fd, (char *)data + read, size - read);
        if (ret == -1) {
            // Restore sigpipe
            signal(SIGPIPE, SIG_DFL);
            throw std::runtime_error("Cannot read from named pipe. Reason: " + std::string(std::strerror(errno)));
        }
        read += ret;
    }

    // Restore sigpipe
    signal(SIGPIPE, SIG_DFL);
}

int InterProcessCom::bytesAvailable() const
{
    int bytes = 0;

    signal(SIGPIPE, &InterProcessCom::handleSigPipe);
    if (ioctl(_fd, FIONREAD, &bytes) == -1) {
        signal(SIGPIPE, SIG_DFL);
        throw std::runtime_error("Cannot get number of bytes available of named pipe");
    }
    signal(SIGPIPE, SIG_DFL);
    return bytes;
}

void InterProcessCom::erasePipes()
{
    for (const std::string &socket : _pipes)
        unlink(socket.c_str());
}

std::string InterProcessCom::getPipeName() const
{
    return _name;
}

void InterProcessCom::handleSigPipe(int)
{
    throw InterProcessCom::PipeException();
}
