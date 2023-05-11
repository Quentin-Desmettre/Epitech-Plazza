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

/**
 * @brief Encapsulate the communication between processes, using a named pipe.
 */
class InterProcessCom {
public:
    enum OpenMode {
        READ,
        WRITE
    };
    /**
     * @brief Create a named pipe, placed in /tmp/Plazza.
     */
    InterProcessCom();

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

protected:
    // Counts the number of reference to a named pipe.
    // If the count is 0, the named pipe is deleted.
    static std::map<std::string, int> _pipes;

    //  The name of the named pipe.
    std::string _name;
    // The file descriptor of the named pipe.
    int _fd;
};

#endif //EPITECH_PLAZZA_INTERPROCESSCOM_HPP