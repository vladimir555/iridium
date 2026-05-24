// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `CSinkAsync` class, an asynchronous wrapper for `ISink` implementations.
///     It uses a worker thread to process log events, improving logging performance by offloading I/O.
/// \~russian @file
/// @brief Определяет класс `CSinkAsync`, асинхронную обертку для реализаций `ISink`.
///     Использует рабочий поток для обработки событий журнала, улучшая производительность логирования за счет выгрузки операций ввода-вывода.

#ifndef HEADER_SINK_175E1AB3_5BEE_4AFC_A1E2_DB31099E39EB
#define HEADER_SINK_175E1AB3_5BEE_4AFC_A1E2_DB31099E39EB


#include "iridium/pattern/non_copyable.h"
#include "iridium/threading/worker.h"
#include "iridium/logging/sink.h"


namespace iridium::logging::implementation {


// todo: CSinkAsync(ISink::TSharedPtr ... ) wrapper
// cached async sink
/// \~english @brief Provides an asynchronous wrapper around an `ISink` implementation.
///     It uses a worker thread to process log events, allowing the logging call to return quickly.
/// \~russian @brief Предоставляет асинхронную обертку вокруг реализации `ISink`.
///     Использует рабочий поток для обработки событий журнала, позволяя вызову логирования быстро возвращаться.
class CSinkAsync: public ISink, pattern::NonCopyable {
public:
    /// \~english @brief Defines standard implementation helpers for `CSinkAsync`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CSinkAsync`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_IMPLEMENTATION(CSinkAsync)
    /// \~english @brief Constructs an asynchronous sink that wraps a given synchronous sink.
    /// \~russian @brief Конструирует асинхронный приемник, который оборачивает данный синхронный приемник.
    /// \~english @param sink The synchronous sink implementation to be wrapped for asynchronous operation.
    /// \~russian @param sink Реализация синхронного приемника, которая будет обернута для асинхронной работы.
    CSinkAsync(ISink::TSharedPtr const &sink);
    /// \~english @brief Initializes the asynchronous sink, typically starting its worker thread.
    /// \~russian @brief Инициализирует асинхронный приемник, обычно запуская его рабочий поток.
    void initialize() override;
    /// \~english @brief Finalizes the asynchronous sink, typically stopping its worker thread and flushing any remaining events.
    /// \~russian @brief Завершает работу асинхронного приемника, обычно останавливая его рабочий поток и сбрасывая все оставшиеся события.
    void finalize() override;
    /// \~english @brief Logs an event by queueing it for processing by the worker thread.
    /// \~russian @brief Логирует событие, помещая его в очередь для обработки рабочим потоком.
    /// \~english @param event The log event to be processed.
    /// \~russian @param event Событие журнала для обработки.
    void log(TEvent::TConstSharedPtr const &event) override;
    /// \~english @brief Flushes events queued in the worker thread and then flushes the underlying synchronous sink.
    /// \~russian @brief Сбрасывает события, находящиеся в очереди рабочего потока, а затем сбрасывает базовый синхронный приемник.
    void flush() override;

private:
    /// \~english @brief Typedef for the worker pusher interface, specialized for log events.
    /// \~russian @brief Typedef для интерфейса рабочего-отправителя, специализированного для событий журнала.
    typedef threading::IWorkerPusher<TEvent::TConstSharedPtr> IWorkerPusher;
    /// \~english @brief Worker thread handler responsible for processing log events passed to the `CSinkAsync`.
    /// \~russian @brief Обработчик рабочего потока, отвечающий за обработку событий журнала, переданных в `CSinkAsync`.
    class CWorkerHandler: public IWorkerPusher::IHandler {
    public:
        /// \~english @brief Defines standard implementation helpers for `CWorkerHandler`.
        /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CWorkerHandler`.
        DEFINE_IMPLEMENTATION(CWorkerHandler)
        /// \~english @brief Constructs a worker handler that forwards events to the specified sink.
        /// \~russian @brief Конструирует обработчик рабочего потока, который пересылает события указанному приемнику.
        /// \~english @param sink The underlying sink that will process the log events.
        /// \~russian @param sink Базовый приемник, который будет обрабатывать события журнала.
        CWorkerHandler(ISink::TSharedPtr const &sink);

        /// \~english @brief Initializes the worker handler. Currently a no-op.
        /// \~russian @brief Инициализирует обработчик рабочего потока. В настоящее время не выполняет никаких действий.
        void initialize() override;
        /// \~english @brief Finalizes the worker handler. Calls `flush()` on the underlying sink.
        /// \~russian @brief Завершает работу обработчика рабочего потока. Вызывает `flush()` у базового приемника.
        void finalize() override;
        /// \~english @brief Handles a batch of log events by forwarding them to the underlying sink's `log` method.
        /// \~russian @brief Обрабатывает пакет событий журнала, пересылая их методу `log` базового приемника.
        /// \~english @param e A collection of log events to handle.
        /// \~russian @param e Коллекция событий журнала для обработки.
        void handle(TInputItems const &e) override;

    private:
        /// \~english @brief Shared pointer to the underlying synchronous sink that this handler wraps.
        /// \~russian @brief Разделяемый указатель на базовый синхронный приемник, который оборачивает этот обработчик.
        ISink::TSharedPtr m_sink;
    };

    IWorkerPusher::TSharedPtr m_worker;
};


} // iridium::logging::implementation


#endif // HEADER_SINK_175E1AB3_5BEE_4AFC_A1E2_DB31099E39EB
