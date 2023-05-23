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
#include <sys/socket.h>
#include <sys/un.h>
#include <algorithm>
#include <csignal>
#include <fcntl.h>
#include <cstring>
#include "Kitchen.hpp"

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
    std::cout << "read" << std::endl;
    signal(SIGPIPE, &InterProcessCom::handleSigPipe);

    std::cout << "read1" << std::endl;
    // Read data
    std::size_t read = 0;
    std::cout << "read2" << std::endl;
    while (read < size) {
        ssize_t ret = ::read(_fd, (char *)data + read, size - read);
        std::cout << "read3" << std::endl;
        if (ret <= 0) {
            // Restore sigpipe
            signal(SIGPIPE, SIG_DFL);
            std::perror("read");
            if (ret < 0)
                throw std::runtime_error("Cannot read from named pipe. Reason: " + std::string(std::strerror(errno)));
            throw PipeException();
        }
        read += ret;
    }

    std::cout << "read4" << std::endl;
    // Restore sigpipe
    signal(SIGPIPE, SIG_DFL);
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
    std::cout << "sigpipe" << std::endl;
    throw InterProcessCom::PipeException();
}

const std::unique_ptr<Kitchen> *InterProcessCom::waitForDataAvailable(InterProcessCom::InputSource &source, const std::vector<std::unique_ptr<Kitchen>> &kitchens)
{
    fd_set readfds;

    // Create fd set
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    for (auto &kitchen : kitchens)
        FD_SET(kitchen->getReadIpc()._fd, &readfds);

    // Wait for data
    if (select(FD_SETSIZE, &readfds, nullptr, nullptr, nullptr) == -1)
        throw std::runtime_error("Cannot select. Reason: " + std::string(strerror(errno)));

    // Return the kitchen that has data available
    if (FD_ISSET(0, &readfds)) {
        source = InputSource::STDIN;
        return nullptr;
    }
    source = InputSource::KITCHEN;
    for (auto &kitchen : kitchens)
        if (FD_ISSET(kitchen->getReadIpc()._fd, &readfds))
            return &kitchen;
    return nullptr;
}

void InterProcessCom::waitForDataAvailable(const InterProcessCom &com)
{
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(com._fd, &readfds);
    if (select(FD_SETSIZE, &readfds, nullptr, nullptr, nullptr) == -1)
        throw std::runtime_error("Cannot select. Reason: " + std::string(strerror(errno)));
}
