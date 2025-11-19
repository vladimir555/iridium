// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2
#define HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2


#include <string>

#include "iridium/io/multiplexer.h"


namespace iridium::system {


/// \~english @brief A class for executing shell commands.
/// \~russian @brief Класс для выполнения команд оболочки.
class Shell: public pattern::IInitializable {
public:
    DEFINE_IMPLEMENTATION(Shell)
    Shell();

    typedef std::chrono::seconds TTimeDuration;

    static TTimeDuration const DEFAULT_TIMEOUT;

    /// \~english @brief The result of a shell command execution.
    /// \~russian @brief Результат выполнения команды оболочки.
    struct TResult {
        std::string output; ///< \~english The standard output of the command. \~russian Стандартный вывод команды.
        int         code; ///< \~english The exit code of the command. \~russian Код завершения команды.
    };

    /// \~english @brief Initializes the shell.
    /// \~russian @brief Инициализирует оболочку.
    void initialize() override;
    /// \~english @brief Finalizes the shell.
    /// \~russian @brief Финализирует оболочку.
    void finalize()   override;

    /// \~english @brief Runs a shell command.
    /// \~russian @brief Выполняет команду оболочки.
    /// \~english @param command_line The command to execute.
    /// \~russian @param command_line Команда для выполнения.
    /// \~english @param timeout The timeout for the command.
    /// \~russian @param timeout Тайм-аут для команды.
    /// \~english @return The result of the command execution.
    /// \~russian @return Результат выполнения команды.
    TResult run(std::string const &command_line, TTimeDuration const &timeout = DEFAULT_TIMEOUT);

private:
    /// \~english @brief The I/O multiplexer for handling process streams.
    /// \~russian @brief Мультиплексор ввода-вывода для обработки потоков процесса.
    io::IMultiplexer::TSharedPtr    m_multiplexer;
};


} // namespace iridium::system


#endif // HEADER_SHELL_46F6D204_C1A7_4BCF_944F_F085BB17E1E2
