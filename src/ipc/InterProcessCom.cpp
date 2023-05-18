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
#include <algorithm>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

std::vector<std::string> InterProcessCom::_socketNames;

InterProcessCom::InterProcessCom(): _fd(-1)
{
    // Find a random name for the unix socket.
    thread_local std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, INT32_MAX);
    do {
        int random = distribution(generator);
        _name = "/tmp/Plazza-unix_socket-" + std::to_string(random);
    } while (std::find(_socketNames.begin(), _socketNames.end(), _name) != _socketNames.end());
    _socketNames.push_back(_name);
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
    sockaddr_un addr{};

    if (_fd != -1)
        this->close();
    // Open socket
    _fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (_fd == -1)
        throw std::runtime_error("Cannot create unix socket. Reason: " + std::string(std::strerror(errno)));

    // Set socket options
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, _name.c_str(), sizeof(addr.sun_path) - 1);

    // Bind if we are creating the socket
    // Else, connect to the socket
    int retval;
    if (mode == CREATE_SOCKET)
        retval = bind(_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));
    else
        retval = connect(_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));
    if (retval == -1)
        throw std::runtime_error("Cannot bind/connect to unix socket. Reason: " + std::string(std::strerror(errno)));
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
            throw std::runtime_error("Cannot write to unix socket. Reason: " + std::string(std::strerror(errno)));
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
            throw std::runtime_error("Cannot read from socket. Reason: " + std::string(std::strerror(errno)));
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
        throw std::runtime_error("Cannot get number of bytes available of unix socket");
    }
    signal(SIGPIPE, SIG_DFL);
    return bytes;
}

void InterProcessCom::eraseSockets()
{
    for (const std::string &socket : _socketNames)
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
