// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `OpenSSL` singleton class, providing a C++ wrapper for OpenSSL functionalities.
///     This includes SSL context and session management, I/O operations, and error handling.
///     Content is compiled only if `BUILD_FLAG_OPENSSL` is defined.
/// \~russian @file
/// @brief Определяет класс-одиночку `OpenSSL`, предоставляющий C++ обертку для функциональностей OpenSSL.
///     Включает управление SSL-контекстами и сессиями, операции ввода-вывода и обработку ошибок.
///     Содержимое компилируется только если определен `BUILD_FLAG_OPENSSL`.

#ifndef HEADER_OPENSSL_356BCFD7_CD30_4EAF_BD8A_1538C082E0E8
#define HEADER_OPENSSL_356BCFD7_CD30_4EAF_BD8A_1538C082E0E8


#include "iridium/build_flags.h"


/// \~english @brief This entire header provides a C++ wrapper for OpenSSL functionalities, compiled only if `BUILD_FLAG_OPENSSL` is defined.
/// \~russian @brief Весь этот заголовочный файл предоставляет C++ обертку для функциональностей OpenSSL, компилируемую только если определен макрос `BUILD_FLAG_OPENSSL`.
#ifdef BUILD_FLAG_OPENSSL


#include "iridium/pattern/singleton.h"
#include "iridium/io/net/socket.h" // Likely for file_descriptor type or related constants
#include "iridium/smart_ptr.h"    // For TSharedPtr
#include "iridium/enum.h"         // For DEFINE_ENUM

#include "iridium/encryption/keys.h" // For default key/cert filenames
//#include "iridium/encryption/ssl.h" // Commented out

#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#include <memory> // For std::shared_ptr if not covered by smart_ptr.h


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Namespace for encryption and cryptographic functionalities.
/// \~russian @brief Пространство имен для функциональности шифрования и криптографии.
namespace encryption {
//namespace implementation { // Original code had this commented out
// todo: rename ...::openssl::API to ...::external::OpenSSL
//namespace openssl { // Original code had this commented out


/// \~english @brief Singleton class providing a C++ wrapper for OpenSSL library operations.
///     Manages OpenSSL initialization, context creation, SSL object handling, I/O operations,
///     and error reporting. This class ensures that OpenSSL is initialized once and provides
///     a more convenient interface to its C API.
/// \~russian @brief Класс-одиночка, предоставляющий C++ обертку для операций библиотеки OpenSSL.
///     Управляет инициализацией OpenSSL, созданием контекста, обработкой объектов SSL, операциями ввода-вывода
///     и сообщениями об ошибках. Этот класс гарантирует однократную инициализацию OpenSSL и предоставляет
///     более удобный интерфейс к ее C API.
class OpenSSL: public pattern::Singleton<OpenSSL> {
public:
    /// \~english @brief Typedef for the OpenSSL SSL context structure (`SSL_CTX`).
    /// \~russian @brief Typedef для структуры SSL-контекста OpenSSL (`SSL_CTX`).
    typedef SSL_CTX TContext;
    /// \~english @brief Typedef for the OpenSSL SSL structure (`SSL`).
    /// \~russian @brief Typedef для структуры SSL OpenSSL (`SSL`).
    typedef SSL     TSSL;

    /// \~english @brief Virtual destructor. Handles cleanup of OpenSSL library resources.
    ///     This may include functions like `ERR_free_strings()`, `EVP_cleanup()`, etc.,
    ///     although specific cleanup calls depend on the OpenSSL versions and initialization flags used.
    /// \~russian @brief Виртуальный деструктор. Обрабатывает очистку ресурсов библиотеки OpenSSL.
    ///     Может включать функции типа `ERR_free_strings()`, `EVP_cleanup()` и т.д.,
    ///     хотя конкретные вызовы очистки зависят от используемых версий OpenSSL и флагов инициализации.
    virtual ~OpenSSL();

    /// \~english @brief Enumeration of OpenSSL error reason codes, mapped from `ERR_R_*_LIB` constants.
    ///     These codes can be used to identify the source library of an OpenSSL error.
    /// \~russian @brief Перечисление кодов причин ошибок OpenSSL, сопоставленных с константами `ERR_R_*_LIB`.
    ///     Эти коды могут использоваться для идентификации исходной библиотеки ошибки OpenSSL.
    DEFINE_ENUM(
        TErrorCode,
        /// \~english @brief System library error. Corresponds to `ERR_R_SYS_LIB`.
        /// \~russian @brief Ошибка системной библиотеки. Соответствует `ERR_R_SYS_LIB`.
        ERROR_CODE_R_SYS_LIB    = ERR_R_SYS_LIB,
        /// \~english @brief BIGNUM library error. Corresponds to `ERR_R_BN_LIB`.
        /// \~russian @brief Ошибка библиотеки BIGNUM. Соответствует `ERR_R_BN_LIB`.
        ERROR_CODE_R_BN_LIB     = ERR_R_BN_LIB,
        /// \~english @brief RSA library error. Corresponds to `ERR_R_RSA_LIB`.
        /// \~russian @brief Ошибка библиотеки RSA. Соответствует `ERR_R_RSA_LIB`.
        ERROR_CODE_R_RSA_LIB    = ERR_R_RSA_LIB,
        /// \~english @brief Diffie-Hellman library error. Corresponds to `ERR_R_DH_LIB`.
        /// \~russian @brief Ошибка библиотеки Diffie-Hellman. Соответствует `ERR_R_DH_LIB`.
        ERROR_CODE_R_DH_LIB     = ERR_R_DH_LIB,
        /// \~english @brief EVP (high-level crypto) library error. Corresponds to `ERR_R_EVP_LIB`.
        /// \~russian @brief Ошибка библиотеки EVP (высокоуровневая криптография). Соответствует `ERR_R_EVP_LIB`.
        ERROR_CODE_R_EVP_LIB    = ERR_R_EVP_LIB,
        /// \~english @brief Buffer library error. Corresponds to `ERR_R_BUF_LIB`.
        /// \~russian @brief Ошибка библиотеки буферов. Соответствует `ERR_R_BUF_LIB`.
        ERROR_CODE_R_BUF_LIB    = ERR_R_BUF_LIB,
        /// \~english @brief ASN.1 object library error. Corresponds to `ERR_R_OBJ_LIB`.
        /// \~russian @brief Ошибка библиотеки объектов ASN.1. Соответствует `ERR_R_OBJ_LIB`.
        ERROR_CODE_R_OBJ_LIB    = ERR_R_OBJ_LIB,
        /// \~english @brief PEM library error. Corresponds to `ERR_R_PEM_LIB`.
        /// \~russian @brief Ошибка библиотеки PEM. Соответствует `ERR_R_PEM_LIB`.
        ERROR_CODE_R_PEM_LIB    = ERR_R_PEM_LIB,
        /// \~english @brief DSA library error. Corresponds to `ERR_R_DSA_LIB`.
        /// \~russian @brief Ошибка библиотеки DSA. Соответствует `ERR_R_DSA_LIB`.
        ERROR_CODE_R_DSA_LIB    = ERR_R_DSA_LIB,
        /// \~english @brief X.509 library error. Corresponds to `ERR_R_X509_LIB`.
        /// \~russian @brief Ошибка библиотеки X.509. Соответствует `ERR_R_X509_LIB`.
        ERROR_CODE_R_X509_LIB   = ERR_R_X509_LIB,
        /// \~english @brief ASN.1 library error. Corresponds to `ERR_R_ASN1_LIB`.
        /// \~russian @brief Ошибка библиотеки ASN.1. Соответствует `ERR_R_ASN1_LIB`.
        ERROR_CODE_R_ASN1_LIB   = ERR_R_ASN1_LIB,
        /// \~english @brief EC (Elliptic Curve) library error. Corresponds to `ERR_R_EC_LIB`.
        /// \~russian @brief Ошибка библиотеки EC (эллиптические кривые). Соответствует `ERR_R_EC_LIB`.
        ERROR_CODE_R_EC_LIB     = ERR_R_EC_LIB,
        /// \~english @brief BIO library error. Corresponds to `ERR_R_BIO_LIB`.
        /// \~russian @brief Ошибка библиотеки BIO. Соответствует `ERR_R_BIO_LIB`.
        ERROR_CODE_R_BIO_LIB    = ERR_R_BIO_LIB,
        /// \~english @brief PKCS#7 library error. Corresponds to `ERR_R_PKCS7_LIB`.
        /// \~russian @brief Ошибка библиотеки PKCS#7. Соответствует `ERR_R_PKCS7_LIB`.
        ERROR_CODE_R_PKCS7_LIB  = ERR_R_PKCS7_LIB,
        /// \~english @brief X.509 V3 extension library error. Corresponds to `ERR_R_X509V3_LIB`.
        /// \~russian @brief Ошибка библиотеки расширений X.509 V3. Соответствует `ERR_R_X509V3_LIB`.
        ERROR_CODE_R_X509V3_LIB = ERR_R_X509V3_LIB,
        /// \~english @brief Engine library error. Corresponds to `ERR_R_ENGINE_LIB`.
        /// \~russian @brief Ошибка библиотеки Engine. Соответствует `ERR_R_ENGINE_LIB`.
        ERROR_CODE_R_ENGINE_LIB = ERR_R_ENGINE_LIB,
        /// \~english @brief UI (User Interface) library error. Corresponds to `ERR_R_UI_LIB`.
        /// \~russian @brief Ошибка библиотеки UI (пользовательский интерфейс). Соответствует `ERR_R_UI_LIB`.
        ERROR_CODE_R_UI_LIB     = ERR_R_UI_LIB,
        /// \~english @brief ECDSA library error. Corresponds to `ERR_R_ECDSA_LIB`.
        /// \~russian @brief Ошибка библиотеки ECDSA. Соответствует `ERR_R_ECDSA_LIB`.
        ERROR_CODE_R_ECDSA_LIB  = ERR_R_ECDSA_LIB,
        /// \~english @brief Nested ASN.1 error. Corresponds to `ERR_R_NESTED_ASN1_ERROR`.
        /// \~russian @brief Вложенная ошибка ASN.1. Соответствует `ERR_R_NESTED_ASN1_ERROR`.
        ERROR_CODE_R_NESTED_ASN1_ERROR          = ERR_R_NESTED_ASN1_ERROR,
        /// \~english @brief Missing ASN.1 End-Of-Sequence/ conteúdo. Corresponds to `ERR_R_MISSING_ASN1_EOS`.
        /// \~russian @brief Отсутствует ASN.1 конец последовательности/содержимого. Соответствует `ERR_R_MISSING_ASN1_EOS`.
        ERROR_CODE_R_MISSING_ASN1_EOS           = ERR_R_MISSING_ASN1_EOS,
        /// \~english @brief Fatal error. Corresponds to `ERR_R_FATAL`.
        /// \~russian @brief Фатальная ошибка. Соответствует `ERR_R_FATAL`.
        ERROR_CODE_R_FATAL                          = ERR_R_FATAL,
        /// \~english @brief Malloc failure. Corresponds to `ERR_R_MALLOC_FAILURE`.
        /// \~russian @brief Ошибка выделения памяти (malloc). Соответствует `ERR_R_MALLOC_FAILURE`.
        ERROR_CODE_R_MALLOC_FAILURE                 = ERR_R_MALLOC_FAILURE,
        /// \~english @brief Function should not have been called. Corresponds to `ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED`.
        /// \~russian @brief Функция не должна была быть вызвана. Соответствует `ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED`.
        ERROR_CODE_R_SHOULD_NOT_HAVE_BEEN_CALLED    = ERR_R_SHOULD_NOT_HAVE_BEEN_CALLED,
        /// \~english @brief Passed NULL parameter. Corresponds to `ERR_R_PASSED_NULL_PARAMETER`.
        /// \~russian @brief Передан нулевой параметр. Соответствует `ERR_R_PASSED_NULL_PARAMETER`.
        ERROR_CODE_R_PASSED_NULL_PARAMETER          = ERR_R_PASSED_NULL_PARAMETER,
        /// \~english @brief Internal error. Corresponds to `ERR_R_INTERNAL_ERROR`.
        /// \~russian @brief Внутренняя ошибка. Соответствует `ERR_R_INTERNAL_ERROR`.
        ERROR_CODE_R_INTERNAL_ERROR                 = ERR_R_INTERNAL_ERROR,
        /// \~english @brief Feature disabled. Corresponds to `ERR_R_DISABLED`.
        /// \~russian @brief Функциональность отключена. Соответствует `ERR_R_DISABLED`.
        ERROR_CODE_R_DISABLED                       = ERR_R_DISABLED
    )

    /// \~english @brief Enumeration of SSL error codes, mapped from `SSL_ERROR_*` constants.
    ///     These codes indicate the result of an SSL operation.
    /// \~russian @brief Перечисление кодов ошибок SSL, сопоставленных с константами `SSL_ERROR_*`.
    ///     Эти коды указывают результат операции SSL.
    DEFINE_ENUM(
        TSSLErrorCode,
        /// \~english @brief No error. Operation successful. Corresponds to `SSL_ERROR_NONE`.
        /// \~russian @brief Нет ошибки. Операция успешна. Соответствует `SSL_ERROR_NONE`.
        SSL_ERROR_CODE_NONE             = SSL_ERROR_NONE,
        /// \~english @brief An error occurred in the SSL library. Corresponds to `SSL_ERROR_SSL`.
        ///     Further error details can be obtained from the OpenSSL error queue.
        /// \~russian @brief Произошла ошибка в библиотеке SSL. Соответствует `SSL_ERROR_SSL`.
        ///     Дополнительные детали ошибки можно получить из очереди ошибок OpenSSL.
        SSL_ERROR_CODE_SSL              = SSL_ERROR_SSL,
        /// \~english @brief The SSL operation wants to read more data. Corresponds to `SSL_ERROR_WANT_READ`.
        ///     For non-blocking I/O, this means the operation should be retried later.
        /// \~russian @brief Операция SSL хочет прочитать больше данных. Соответствует `SSL_ERROR_WANT_READ`.
        ///     Для неблокирующего ввода-вывода это означает, что операцию следует повторить позже.
        SSL_ERROR_CODE_WANT_READ        = SSL_ERROR_WANT_READ,
        /// \~english @brief The SSL operation wants to write more data. Corresponds to `SSL_ERROR_WANT_WRITE`.
        ///     For non-blocking I/O, this means the operation should be retried later.
        /// \~russian @brief Операция SSL хочет записать больше данных. Соответствует `SSL_ERROR_WANT_WRITE`.
        ///     Для неблокирующего ввода-вывода это означает, что операцию следует повторить позже.
        SSL_ERROR_CODE_WANT_WRITE       = SSL_ERROR_WANT_WRITE,
        /// \~english @brief The SSL operation wants to perform an X.509 lookup. Corresponds to `SSL_ERROR_WANT_X509_LOOKUP`.
        ///     This can occur during certificate verification.
        /// \~russian @brief Операция SSL хочет выполнить поиск X.509. Соответствует `SSL_ERROR_WANT_X509_LOOKUP`.
        ///     Это может произойти во время проверки сертификата.
        SSL_ERROR_CODE_WANT_X509_LOOKUP = SSL_ERROR_WANT_X509_LOOKUP,
        /// \~english @brief A system call error occurred. Corresponds to `SSL_ERROR_SYSCALL`.
        ///     Check `errno` for details if `ERR_get_error()` returns 0.
        /// \~russian @brief Произошла ошибка системного вызова. Соответствует `SSL_ERROR_SYSCALL`.
        ///     Проверьте `errno` для получения подробной информации, если `ERR_get_error()` возвращает 0.
        SSL_ERROR_CODE_SYSCALL          = SSL_ERROR_SYSCALL,
        /// \~english @brief The connection was closed cleanly (SSL_shutdown). Corresponds to `SSL_ERROR_ZERO_RETURN`.
        /// \~russian @brief Соединение было корректно закрыто (SSL_shutdown). Соответствует `SSL_ERROR_ZERO_RETURN`.
        SSL_ERROR_CODE_ZERO_RETURN      = SSL_ERROR_ZERO_RETURN,
        /// \~english @brief The SSL operation wants to initiate a new connection. Corresponds to `SSL_ERROR_WANT_CONNECT`.
        /// \~russian @brief Операция SSL хочет инициировать новое соединение. Соответствует `SSL_ERROR_WANT_CONNECT`.
        SSL_ERROR_CODE_WANT_CONNECT     = SSL_ERROR_WANT_CONNECT,
        /// \~english @brief The SSL operation wants to accept a new connection. Corresponds to `SSL_ERROR_WANT_ACCEPT`.
        /// \~russian @brief Операция SSL хочет принять новое соединение. Соответствует `SSL_ERROR_WANT_ACCEPT`.
        SSL_ERROR_CODE_WANT_ACCEPT      = SSL_ERROR_WANT_ACCEPT
    )

    /// \~english @brief Creates an SSL context (`SSL_CTX`).
    ///     The context is created using `SSL_CTX_new` with either `TLS_server_method()` or `TLS_client_method()`.
    /// \~russian @brief Создает SSL-контекст (`SSL_CTX`).
    ///     Контекст создается с помощью `SSL_CTX_new` с использованием `TLS_server_method()` или `TLS_client_method()`.
    /// \~english @param is_server_method If true, creates a server-side context; otherwise, a client-side context.
    /// \~russian @param is_server_method Если true, создает серверный контекст; в противном случае - клиентский.
    /// \~english @return Pointer to the created `SSL_CTX` object, or `nullptr` on failure.
    /// \~russian @return Указатель на созданный объект `SSL_CTX` или `nullptr` в случае сбоя.
    TContext *createContext(bool const &is_server_method);

    /// \~english @brief Configures an SSL context with a private key and certificate.
    ///     Uses `SSL_CTX_use_PrivateKey_file` and `SSL_CTX_use_certificate_file`.
    ///     It may also check the consistency of the private key with the certificate.
    /// \~russian @brief Настраивает SSL-контекст с использованием закрытого ключа и сертификата.
    ///     Использует `SSL_CTX_use_PrivateKey_file` и `SSL_CTX_use_certificate_file`.
    ///     Также может проверять согласованность закрытого ключа с сертификатом.
    /// \~english @param context The `SSL_CTX` to configure.
    /// \~russian @param context `SSL_CTX` для настройки.
    /// \~english @param file_name_private_key Path to the private key file (PEM format).
    /// \~russian @param file_name_private_key Путь к файлу закрытого ключа (формат PEM).
    /// \~english @param file_name_certificate Path to the certificate file (PEM format).
    /// \~russian @param file_name_certificate Путь к файлу сертификата (формат PEM).
    /// \~english @throws iridium::db::Exception (or similar) if configuration fails (e.g., file not found, key/cert mismatch).
    /// \~russian @throws iridium::db::Exception (или аналогичное) в случае сбоя конфигурации (например, файл не найден, несоответствие ключа/сертификата).
    void configureContext(
        TContext  * const  context,
        std::string const &file_name_private_key,
        std::string const &file_name_certificate);

    /// \~english @brief Releases (frees) an SSL context. Calls `SSL_CTX_free`.
    /// \~russian @brief Освобождает SSL-контекст. Вызывает `SSL_CTX_free`.
    /// \~english @param context The `SSL_CTX` to release.
    /// \~russian @param context `SSL_CTX` для освобождения.
    void releaseContext(TContext *context);

    /// \~english @brief Creates an SSL session object (`SSL`) from a given context and associates it with a file descriptor.
    ///     Calls `SSL_new` to create the SSL object and `SSL_set_fd` to associate the file descriptor.
    /// \~russian @brief Создает объект SSL-сессии (`SSL`) из заданного контекста и связывает его с файловым дескриптором.
    ///     Вызывает `SSL_new` для создания объекта SSL и `SSL_set_fd` для связывания файлового дескриптора.
    /// \~english @param context The `SSL_CTX` (SSL context) from which to create the `SSL` object.
    /// \~russian @param context `SSL_CTX` (SSL-контекст), из которого создается объект `SSL`.
    /// \~english @param file_descriptor The network socket file descriptor for the SSL connection.
    /// \~russian @param file_descriptor Файловый дескриптор сетевого сокета для SSL-соединения.
    /// \~english @return Pointer to the created `SSL` object, or `nullptr` on failure.
    /// \~russian @return Указатель на созданный объект `SSL` или `nullptr` в случае сбоя.
    TSSL *createSSL(TContext *context, int const &file_descriptor);

    /// \~english @brief Releases (frees) an SSL session object. Calls `SSL_free`.
    /// \~russian @brief Освобождает объект SSL-сессии. Вызывает `SSL_free`.
    /// \~english @param ssl The `SSL` object to release.
    /// \~russian @param ssl Объект `SSL` для освобождения.
    void releaseSSL(TSSL *ssl);

    /// \~english @brief Performs the server-side SSL handshake (`SSL_accept`).
    ///     This function is typically called after `setAcceptState`.
    ///     The behavior (blocking/non-blocking) depends on the `is_blocking_mode` and underlying socket settings.
    /// \~russian @brief Выполняет SSL-рукопожатие на стороне сервера (`SSL_accept`).
    ///     Эта функция обычно вызывается после `setAcceptState`.
    ///     Поведение (блокирующее/неблокирующее) зависит от `is_blocking_mode` и настроек базового сокета.
    /// \~english @param ssl The `SSL` object for the connection.
    /// \~russian @param ssl Объект `SSL` для соединения.
    /// \~english @param is_blocking_mode If true, attempts a blocking accept; otherwise, non-blocking.
    ///     Note: Actual blocking behavior also depends on the socket's blocking state.
    /// \~russian @param is_blocking_mode Если true, пытается выполнить блокирующий accept; иначе неблокирующий.
    ///     Примечание: Фактическое блокирующее поведение также зависит от состояния блокировки сокета.
    /// \~english @throws iridium::db::Exception (or similar) on SSL handshake errors if not handled by WANT_READ/WANT_WRITE.
    /// \~russian @throws iridium::db::Exception (или аналогичное) при ошибках SSL-рукопожатия, если они не обрабатываются через WANT_READ/WANT_WRITE.
    void acceptSSL(TSSL *ssl, bool const &is_blocking_mode);

    /// \~english @brief Performs the client-side SSL handshake (`SSL_connect`).
    ///     This function is typically called after `setConnectState`.
    /// \~russian @brief Выполняет SSL-рукопожатие на стороне клиента (`SSL_connect`).
    ///     Эта функция обычно вызывается после `setConnectState`.
    /// \~english @param ssl The `SSL` object for the connection.
    /// \~russian @param ssl Объект `SSL` для соединения.
    /// \~english @return An `TSSLErrorCode` indicating the result of the connect attempt (e.g., `SSL_ERROR_NONE`, `SSL_ERROR_WANT_READ`).
    /// \~russian @return `TSSLErrorCode`, указывающий результат попытки соединения (например, `SSL_ERROR_NONE`, `SSL_ERROR_WANT_READ`).
    TSSLErrorCode connectSSL(TSSL *ssl);

    /// \~english @brief Sets the SSL object to work in client connection mode. Calls `SSL_set_connect_state`.
    /// \~russian @brief Устанавливает объект SSL для работы в режиме клиентского подключения. Вызывает `SSL_set_connect_state`.
    /// \~english @param ssl The `SSL` object.
    /// \~russian @param ssl Объект `SSL`.
    /// \~english @return `SSL_ERROR_CODE_NONE` on success, or an error code if `SSL_set_connect_state` fails (though it usually doesn't return errors itself).
    /// \~russian @return `SSL_ERROR_CODE_NONE` в случае успеха или код ошибки, если `SSL_set_connect_state` завершается неудачно (хотя обычно она сама не возвращает ошибок).
    TSSLErrorCode setConnectState(TSSL *ssl);

    /// \~english @brief Sets the SSL object to work in server accept mode. Calls `SSL_set_accept_state`.
    /// \~russian @brief Устанавливает объект SSL для работы в режиме принятия соединений сервером. Вызывает `SSL_set_accept_state`.
    /// \~english @param ssl The `SSL` object.
    /// \~russian @param ssl Объект `SSL`.
    /// \~english @return `SSL_ERROR_CODE_NONE` on success, or an error code if `SSL_set_accept_state` fails (though it usually doesn't return errors itself).
    /// \~russian @return `SSL_ERROR_CODE_NONE` в случае успеха или код ошибки, если `SSL_set_accept_state` завершается неудачно (хотя обычно она сама не возвращает ошибок).
    TSSLErrorCode setAcceptState(TSSL *ssl);

    /// \~english @brief Performs the SSL handshake (`SSL_do_handshake`).
    ///     This can be used after `SSL_set_accept_state` or `SSL_set_connect_state` to explicitly drive the handshake.
    /// \~russian @brief Выполняет SSL-рукопожатие (`SSL_do_handshake`).
    ///     Может использоваться после `SSL_set_accept_state` или `SSL_set_connect_state` для явного управления рукопожатием.
    /// \~english @param ssl The `SSL` object.
    /// \~russian @param ssl Объект `SSL`.
    /// \~english @return An `TSSLErrorCode` indicating the result of the handshake attempt.
    /// \~russian @return `TSSLErrorCode`, указывающий результат попытки рукопожатия.
    TSSLErrorCode doHandshake(TSSL *ssl);

    /// \~english @brief Writes data to an SSL connection using `SSL_write`.
    /// \~russian @brief Записывает данные в SSL-соединение с помощью `SSL_write`.
    /// \~english @param ssl The `SSL` object for the connection.
    /// \~russian @param ssl Объект `SSL` для соединения.
    /// \~english @param packet A shared pointer to a buffer containing the data to write.
    /// \~russian @param packet Умный указатель на буфер, содержащий данные для записи.
    /// \~english @return The number of bytes written.
    /// \~russian @return Количество записанных байт.
    /// \~english @throws iridium::db::Exception (or similar) or returns a value <= 0 on error, which should be checked with `getSSLErrorCode`.
    /// \~russian @throws iridium::db::Exception (или аналогичное) или возвращает значение <= 0 в случае ошибки, которое следует проверить с помощью `getSSLErrorCode`.
    size_t write(TSSL *ssl, io::Buffer::TSharedPtr const &packet);

    /// \~english @brief Reads data from an SSL connection using `SSL_read`.
    /// \~russian @brief Читает данные из SSL-соединения с помощью `SSL_read`.
    /// \~english @param ssl The `SSL` object for the connection.
    /// \~russian @param ssl Объект `SSL` для соединения.
    /// \~english @param size The maximum number of bytes to read.
    /// \~russian @param size Максимальное количество байт для чтения.
    /// \~english @return A shared pointer to a buffer containing the data read. Buffer may be empty if no data was read or smaller than `size`.
    /// \~russian @return Умный указатель на буфер, содержащий прочитанные данные. Буфер может быть пустым, если данные не были прочитаны, или меньше, чем `size`.
    /// \~english @throws iridium::db::Exception (or similar) or returns a buffer from a read of 0 or less bytes on error, which should be checked with `getSSLErrorCode`.
    /// \~russian @throws iridium::db::Exception (или аналогичное) или возвращает буфер от чтения 0 или менее байт в случае ошибки, которое следует проверить с помощью `getSSLErrorCode`.
    io::Buffer::TSharedPtr read(TSSL *ssl, size_t const &size);

    /// \~english @brief Retrieves a human-readable error string for the last error on the OpenSSL error queue.
    ///     Wraps `ERR_get_error` and `ERR_error_string`.
    /// \~russian @brief Получает человекочитаемую строку ошибки для последней ошибки в очереди ошибок OpenSSL.
    ///     Оборачивает `ERR_get_error` и `ERR_error_string`.
    /// \~english @return A string describing the last OpenSSL error.
    /// \~russian @return Строка, описывающая последнюю ошибку OpenSSL.
    std::string getErrorString();

    /// \~english @brief Retrieves a human-readable error string for a given OpenSSL error code.
    ///     Wraps `ERR_error_string`.
    /// \~russian @brief Получает человекочитаемую строку ошибки для заданного кода ошибки OpenSSL.
    ///     Оборачивает `ERR_error_string`.
    /// \~english @param code The OpenSSL error code (packed representation).
    /// \~russian @param code Код ошибки OpenSSL (в упакованном представлении).
    /// \~english @return A string describing the error.
    /// \~russian @return Строка, описывающая ошибку.
    std::string getErrorString(int const &code);

    /// \~english @brief Retrieves a human-readable error string for an SSL operation error code.
    ///     This function is deprecated or less common than using `getSSLErrorCode` and then `getErrorString` for `SSL_ERROR_SSL`.
    ///     It directly calls `SSL_get_error` and attempts to provide a string.
    /// \~russian @brief Получает человекочитаемую строку ошибки для кода ошибки операции SSL.
    ///     Эта функция устарела или менее распространена, чем использование `getSSLErrorCode` и затем `getErrorString` для `SSL_ERROR_SSL`.
    ///     Она напрямую вызывает `SSL_get_error` и пытается предоставить строку.
    /// \~english @param ssl The `SSL` object associated with the error.
    /// \~russian @param ssl Объект `SSL`, связанный с ошибкой.
    /// \~english @param code The return code from the SSL I/O operation (e.g., from `SSL_read`, `SSL_write`).
    /// \~russian @param code Код возврата от операции SSL ввода-вывода (например, из `SSL_read`, `SSL_write`).
    /// \~english @return A string describing the SSL error.
    /// \~russian @return Строка, описывающая ошибку SSL.
    std::string getSSLErrorString(TSSL *ssl, int const &code);

    /// \~english @brief Retrieves the specific SSL error code enum (`TSSLErrorCode`) for an SSL operation.
    ///     Wraps `SSL_get_error` to interpret the result of an SSL I/O function.
    /// \~russian @brief Получает конкретный код ошибки SSL в виде enum (`TSSLErrorCode`) для операции SSL.
    ///     Оборачивает `SSL_get_error` для интерпретации результата функции SSL ввода-вывода.
    /// \~english @param ssl The `SSL` object associated with the operation.
    /// \~russian @param ssl Объект `SSL`, связанный с операцией.
    /// \~english @param code The return code from the SSL I/O operation (e.g., from `SSL_read`, `SSL_write`, `SSL_connect`, `SSL_accept`).
    /// \~russian @param code Код возврата от операции SSL ввода-вывода (например, из `SSL_read`, `SSL_write`, `SSL_connect`, `SSL_accept`).
    /// \~english @return The `TSSLErrorCode` enum value corresponding to the error.
    /// \~russian @return Значение перечисления `TSSLErrorCode`, соответствующее ошибке.
    TSSLErrorCode getSSLErrorCode(TSSL *ssl, int const &code);

private:
    friend class pattern::Singleton<OpenSSL>;
    /// \~english @brief Private constructor for the Singleton pattern.
    ///     Initializes the OpenSSL library (e.g., `SSL_load_error_strings()`, `OpenSSL_add_ssl_algorithms()` or `SSL_library_init()`).
    /// \~russian @brief Закрытый конструктор для паттерна Singleton.
    ///     Инициализирует библиотеку OpenSSL (например, `SSL_load_error_strings()`, `OpenSSL_add_ssl_algorithms()` или `SSL_library_init()`).
    OpenSSL();
};


} // encryption
} // iridium


#endif // BUILD_FLAG_OPENSSL


#endif // HEADER_OPENSSL_356BCFD7_CD30_4EAF_BD8A_1538C082E0E8
