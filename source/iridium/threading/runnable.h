// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_RUNNUBLE_82187C6C_2498_4042_A14B_902FECFE4570
#define HEADER_RUNNUBLE_82187C6C_2498_4042_A14B_902FECFE4570


#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include <atomic>


namespace iridium::threading {


/// \~english @brief An interface for objects that can be run in a separate thread.
/// \~russian @brief Интерфейс для объектов, которые могут быть запущены в отдельном потоке.
class IRunnable: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IRunnable)
    /// \~english @brief The main function to be executed in a separate thread.
    /// \~russian @brief Основная функция, которая будет выполняться в отдельном потоке.
    /// \~english @param is_running A flag to control the execution of the runnable.
    /// \~russian @param is_running Флаг для управления выполнением объекта.
    virtual void run(std::atomic<bool> &is_running) = 0;
};


} // namespace iridium::threading


#endif // HEADER_RUNNUBLE_82187C6C_2498_4042_A14B_902FECFE4570
