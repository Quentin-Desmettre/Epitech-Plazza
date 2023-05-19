/*
** EPITECH PROJECT, 2023
** Epitech_Plazza
** File description:
** Kitchen
*/

#ifndef EPITECH_PLAZZA_KITCHEN_HPP
#define EPITECH_PLAZZA_KITCHEN_HPP
#include <thread>
#include <memory>
#include <chrono>
#include <future>
#include <chrono>
#include <future>

#include "ProcessForker.hpp"
#include "ipc/PizzaIPC.hpp"
#include "CookPool.hpp"

/**
 * @brief Kitchen class
 *
 * This class handles the cooking of pizzas.
 */
class Kitchen {
public:
    Kitchen(int cooks, int restockTimeMs, float multiplier);
    ~Kitchen();

////////////////////////////////////////////////////////////////////////////////
// Methods to be called from the forked process
////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief Runs the kitchen forever.
     *
     * This method will do the main loop of the forked kitchen, i.e:
     *  - Refill ingredients in a separate thread
     *  - Launch a thread that awaits for a cook to finish a pizza
     *  - Check if a pizza has to be cook.
     *
     * This method is meant to be called in the forked process.
     */
    void run();

    void checkForRefill();

    void close() {
        _process->kill();
    }

    /**
     * @brief Waits for a command from the reception.
     *
     * Will call CookPool::addPizza
     */
    void awaitForCommand();

////////////////////////////////////////////////////////////////////////////////
// Methods to be called from the reception
////////////////////////////////////////////////////////////////////////////////

    /**
     * @brief Returns the number of pizzas awaiting to be cooked.
     * @return The number of pizzas awaiting to be cooked.
     */
    int getPizzasAwaiting() const;

    /**
     * @brief Returns the maximum number of pizzas that can be cooked at the same time.
     * @return The maximum number of pizzas that can be cooked at the same time (nbCooks * 2).
     */
    int getCapacity() const;

    /**
     * @brief Add a pizza in the waiting list.
     *
     * This method will increment the number of pizzas awaiting to be cooked,
     * and send via the IPC the pizza to the kitchen.
     * It also stops and sets the timeout clock to 0.
     */
    void addPizza(const Pizza &pizza);

    /**
     * @brief Check if a pizza has been sent by the forked process.
     *
     * It does this by trying to read to the internal IPC.
     * @return true if there is enough data to read 1 pizza, else false.
     */
    bool hasPizzaFinished();

    /**
     * @brief
     * @return
     */
    bool isKitchenClosed();

    /**
     * @brief Returns a finished pizza. If no pizza is available, this call will be blocking.
     *
     * A successful call to this method will decrement the internal pizza counter.
     * If the counter drops to 0, start the timeout clock.
     * @return A std::unique_ptr to the finished pizza.
     */
    Pizza getPizza();

    /**
     * @brief Closes the kitchen.
     */
    void putTheKeyUnderTheDoor();
    void setProcess(Process *process);

    /**
     * @brief Open ipcs.
     */
    void openIpcs(bool isForked);

    int getId() const;

    bool _close = false;
    int _counter = 0;
    // Last order tme
    std::chrono::high_resolution_clock::time_point _lastOrderTime;

private:
    std::unique_ptr<PizzaIPC> _ipcParentToChild, _ipcChildToParent;
    const float _multiplier;
    const int _cooks;
    const int _restockTimeMs;

    bool _isCooking;
    std::chrono::high_resolution_clock::time_point _timeoutClock;

    std::unique_ptr<CookPool> _cookPool;
    Process *_process;

    int _id;
    static int _maxId;
};


#endif //EPITECH_PLAZZA_KITCHEN_HPP
