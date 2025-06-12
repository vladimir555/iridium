// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines constants for default filenames related to encryption keys and certificates.
/// \~russian @brief Определяет константы для имен файлов по умолчанию, связанных с ключами шифрования и сертификатами.
#ifndef HEADER_KEYS_019AF2BA_D462_4CF2_AF87_FED7C5B62A51
#define HEADER_KEYS_019AF2BA_D462_4CF2_AF87_FED7C5B62A51


#include <string>


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Namespace for encryption and cryptographic functionalities.
/// \~russian @brief Пространство имен для функциональности шифрования и криптографии.
namespace encryption {


/// \~english @brief Default filename for storing or loading a private key.
///     This constant can be used by functions that handle private key files when a specific filename is not provided.
/// \~russian @brief Имя файла по умолчанию для сохранения или загрузки закрытого ключа.
///     Эта константа может использоваться функциями, обрабатывающими файлы закрытых ключей, когда конкретное имя файла не указано.
std::string const DEFAULT_FILE_NAME_PRIVATE_KEY = "iridium.key";

/// \~english @brief Default filename for storing or loading a certificate.
///     This constant can be used by functions that handle certificate files when a specific filename is not provided.
/// \~russian @brief Имя файла по умолчанию для сохранения или загрузки сертификата.
///     Эта константа может использоваться функциями, обрабатывающими файлы сертификатов, когда конкретное имя файла не указано.
std::string const DEFAULT_FILE_NAME_CERTIFICATE = "iridium.crt";


} // encryption
} // iridium


#endif // HEADER_KEYS_019AF2BA_D462_4CF2_AF87_FED7C5B62A51
