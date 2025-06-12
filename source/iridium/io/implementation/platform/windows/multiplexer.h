#ifndef HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
#define HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/multiplexer.h"

#include "iridium/threading/synchronized.h"

#include <unordered_map>

#include <Windows.h>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {

/// \~english @brief Windows-specific implementation of `IMultiplexer` using I/O Completion Ports (IOCP).
///     This class allows monitoring multiple stream handles for I/O completion events.
///     Operations are synchronized using a mutex.
/// \~russian @brief Реализация `IMultiplexer` для Windows, использующая порты завершения ввода-вывода (IOCP).
///     Этот класс позволяет отслеживать несколько дескрипторов потоков на предмет событий завершения ввода-вывода.
///     Операции синхронизируются с использованием мьютекса.
class CMultiplexer : public IMultiplexer, public threading::Synchronized<std::mutex> {
public:
    /// \~english @brief Constructs a `CMultiplexer` instance.
    ///     Initializes the IOCP handle to `NULL`.
    /// \~russian @brief Конструирует экземпляр `CMultiplexer`.
    ///     Инициализирует дескриптор IOCP значением `NULL`.
    CMultiplexer();
    DEFINE_IMPLEMENTATION(CMultiplexer)

    /// \~english @brief Initializes the IOCP-based multiplexer.
    ///     Creates an I/O Completion Port.
    /// \~russian @brief Инициализирует мультиплексор на основе IOCP.
    ///     Создает порт завершения ввода-вывода.
    /// \~english @throws std::runtime_error if `CreateIoCompletionPort` fails.
    /// \~russian @throws std::runtime_error, если `CreateIoCompletionPort` завершается с ошибкой.
    void initialize() override;

    /// \~english @brief Finalizes the IOCP-based multiplexer.
    ///     Closes the I/O Completion Port handle.
    /// \~russian @brief Завершает работу мультиплексора на основе IOCP.
    ///     Закрывает дескриптор порта завершения ввода-вывода.
    /// \~english @throws std::runtime_error if `CloseHandle` fails.
    /// \~russian @throws std::runtime_error, если `CloseHandle` завершается с ошибкой.
    void finalize() override;

    /// \~english @brief Waits for I/O completion events on subscribed streams.
    ///     Uses `GetQueuedCompletionStatus` to retrieve completion events.
    ///     Note: The current implementation has a placeholder comment "todo: ... return events"
    ///     and returns an empty list, indicating that the event generation logic is not yet fully implemented.
    /// \~russian @brief Ожидает событий завершения ввода-вывода на подписанных потоках.
    ///     Использует `GetQueuedCompletionStatus` для извлечения событий завершения.
    ///     Примечание: Текущая реализация содержит комментарий-заполнитель "todo: ... return events"
    ///     и возвращает пустой список, указывая, что логика генерации событий еще не полностью реализована.
    /// \~english @return A list of `Event::TSharedPtr`. Currently returns an empty list.
    /// \~russian @return Список `Event::TSharedPtr`. В настоящее время возвращает пустой список.
    /// \~english @throws std::runtime_error if `GetQueuedCompletionStatus` fails in an unexpected way (though typical error handling for IOCP involves checking the return status and `lpNumberOfBytesTransferred`).
    /// \~russian @throws std::runtime_error, если `GetQueuedCompletionStatus` завершается с неожиданной ошибкой (хотя типичная обработка ошибок для IOCP включает проверку возвращаемого статуса и `lpNumberOfBytesTransferred`).
    std::list<Event::TSharedPtr> waitEvents() override;

    /// \~english @brief Subscribes a stream for I/O completion event monitoring.
    ///     Associates the stream's handles with the IOCP.
    /// \~russian @brief Подписывает поток на мониторинг событий завершения ввода-вывода.
    ///     Связывает дескрипторы потока с IOCP.
    /// \~english @param stream The stream to subscribe. Its handles must be valid.
    /// \~russian @param stream Поток для подписки. Его дескрипторы должны быть действительными.
    /// \~english @throws std::runtime_error if `CreateIoCompletionPort` fails to associate a handle or if the stream provides no handles.
    /// \~russian @throws std::runtime_error, если `CreateIoCompletionPort` не удается связать дескриптор или если поток не предоставляет дескрипторов.
    void   subscribe(IStream::TSharedPtr const &stream) override;

    /// \~english @brief Unsubscribes a stream from I/O completion event monitoring.
    ///     Removes the association of the stream's handles from the IOCP mapping.
    ///     Note: IOCP doesn't have an explicit "unsubscribe" for handles in the same way epoll or kqueue do.
    ///     This method primarily cleans up internal mappings. Associated handles are typically implicitly unsubscribed when closed.
    /// \~russian @brief Отписывает поток от мониторинга событий завершения ввода-вывода.
    ///     Удаляет сопоставление дескрипторов потока из карты IOCP.
    ///     Примечание: IOCP не имеет явной операции "отписки" для дескрипторов так, как это делают epoll или kqueue.
    ///     Этот метод в основном очищает внутренние сопоставления. Связанные дескрипторы обычно неявно отписываются при закрытии.
    /// \~english @param stream The stream to unsubscribe.
    /// \~russian @param stream Поток для отписки.
    void unsubscribe(IStream::TSharedPtr const &stream) override;

private:
    /// \~english @brief Handle to the I/O Completion Port.
    /// \~russian @brief Дескриптор порта завершения ввода-вывода.
    HANDLE m_iocp;
    /// \~english @brief Maps a Windows HANDLE (from a stream) to its corresponding `IStream::TSharedPtr`.
    ///     Used to retrieve the stream object when a completion event occurs for a handle.
    /// \~russian @brief Сопоставляет Windows HANDLE (из потока) с соответствующим `IStream::TSharedPtr`.
    ///     Используется для извлечения объекта потока при возникновении события завершения для дескриптора.
    std::unordered_map<HANDLE, IStream::TSharedPtr> m_map_id_stream;
};


} // namespace platform
} // namespace implementation
} // namespace io
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
