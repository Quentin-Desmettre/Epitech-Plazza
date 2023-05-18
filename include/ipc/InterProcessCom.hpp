/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** InterProcessCom
*/

#ifndef EPITECH_PLAZZA_INTERPROCESSCOM_HPP
#define EPITECH_PLAZZA_INTERPROCESSCOM_HPP
#include <map>
#include <string>
#include <vector>

/**
 * @brief Encapsulate the communication between processes, using a named pipe.
 */
class InterProcessCom {
public:
    enum OpenMode {
        CREATE_SOCKET,
        CONNECT_SOCKET
    };
    /**
     * @brief Create a named pipe, placed in /tmp/Plazza.
     */
    InterProcessCom();

    InterProcessCom(const InterProcessCom &);

    /**
     * @brief Closes the named pipe (if not closed). If the named pipe is not used anymore, it is deleted.
     */
    ~InterProcessCom();

    /**
     * @brief Opens the named pipe.
     */
    void open(OpenMode mode);

    /**
     * @brief Closes the named pipe.
     */
    void close();

    /**
     * @brief Writes raw data to the named pipe.
     */
    void write(const void *data, size_t size) const;

    /**
     * @brief Reads raw data from the named pipe, and block until data all data has been read.
     */
    void read(void *data, size_t size) const;

    std::string getPipeName() const;

    class PipeException: public std::exception {
    public:
        PipeException();

        ~PipeException() override = default;
    };
    static void handleSigPipe(int);

protected:
    [[nodiscard]] int bytesAvailable() const;

    // List of every unix socket created, to delete them when the program exits.
    static std::vector<std::string> _socketNames;

    //  The name of the unix socket
    std::string _name;
    // The file descriptor of the named pipe.
    int _fd = -1;

    OpenMode _mode;

private:
    static void eraseSockets() __attribute__((destructor));
};

#endif //EPITECH_PLAZZA_INTERPROCESSCOM_HPP
