// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BUILD_FLAGS_C48326BD_8009_40B6_880F_590C8A2F2371
#define HEADER_BUILD_FLAGS_C48326BD_8009_40B6_880F_590C8A2F2371


/// \~english @file
/// @brief Centralizes preprocessor definitions (build flags) that control conditional compilation.
///     These flags are typically set by the build system (e.g., CMake) to include or exclude
///     features like database support (MySQL, PostgreSQL), OpenSSL integration, or debug logging.
///     The definitions in this file are commented out as they are meant to be defined externally by the build configuration.
/// \~russian @file
/// @brief Централизует определения препроцессора (флаги сборки), которые управляют условной компиляцией.
///     Эти флаги обычно устанавливаются системой сборки (например, CMake) для включения или исключения
///     таких функций, как поддержка баз данных (MySQL, PostgreSQL), интеграция с OpenSSL или отладочное логирование.
///     Определения в этом файле закомментированы, так как они должны быть определены извне конфигурацией сборки.

/// \~english @def BUILD_FLAG_MYSQL
/// @brief If defined, enables features and code related to MySQL database connectivity.
/// \~russian @def BUILD_FLAG_MYSQL
/// @brief Если определено, включает функции и код, связанные с подключением к базе данных MySQL.
//#define BUILD_FLAG_MYSQL

/// \~english @def BUILD_FLAG_POSTGRES
/// @brief If defined, enables features and code related to PostgreSQL database connectivity.
/// \~russian @def BUILD_FLAG_POSTGRES
/// @brief Если определено, включает функции и код, связанные с подключением к базе данных PostgreSQL.
//#define BUILD_FLAG_POSTGRES

/// \~english @def BUILD_FLAG_OPENSSL
/// @brief If defined, enables features that depend on the OpenSSL library, such as cryptographic functions.
/// \~russian @def BUILD_FLAG_OPENSSL
/// @brief Если определено, включает функции, которые зависят от библиотеки OpenSSL, например, криптографические функции.
//#define BUILD_FLAG_OPENSSL

/// \~english @def BUILD_FLAG_FORCE_DEBUG_LOG
/// @brief If defined, forces the inclusion of debug-level logging even in release builds.
/// \~russian @def BUILD_FLAG_FORCE_DEBUG_LOG
/// @brief Если определено, принудительно включает логирование уровня отладки даже в релизных сборках.
//#define BUILD_FLAG_FORCE_DEBUG_LOG

/// \~english @def NANO_PREALLOCATE_BAND_VM
/// @brief If defined, a specific memory preallocation strategy (related to "NANO_PREALLOCATE_BAND_VM") is enabled.
///     If undefined (`#undef`), this feature is disabled.
/// \~russian @def NANO_PREALLOCATE_BAND_VM
/// @brief Если определено, включает специфическую стратегию предварительного выделения памяти (связанную с "NANO_PREALLOCATE_BAND_VM").
///     Если не определено (`#undef`), эта функция отключается.
//#undef NANO_PREALLOCATE_BAND_VM


#endif // HEADER_BUILD_FLAGS_C48326BD_8009_40B6_880F_590C8A2F2371
