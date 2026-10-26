/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Iridium", "index.html", [
    [ "Лицензия", "index.html#sec_license", null ],
    [ "Парсеры и сериализаторы", "index.html#sec_parsers_serializers", [
      [ "Интерфейс IParser", "index.html#subsec_iparser", null ],
      [ "Пример парсинга (JSON)", "index.html#subsec_parsing_example", null ],
      [ "Пример сериализации (JSON)", "index.html#subsec_serializing_example", null ],
      [ "Типизированная сериализация через схему", "index.html#subsec_serialization_macros", [
        [ "Определение схемы", "index.html#subsubsec_schema_definition", null ],
        [ "Использование схемы", "index.html#subsubsec_schema_usage", null ],
        [ "Семантика копирования и перемещения", "index.html#subsubsec_schema_semantics", null ],
        [ "Текстовые узлы", "index.html#subsubsec_schema_text", null ],
        [ "Уникальность имён типов", "index.html#subsubsec_schema_unique_names", null ]
      ] ]
    ] ],
    [ "Использование и конфигурация логгера", "index.html#sec_logging", [
      [ "Базовое логирование", "index.html#subsec_logging_macros", null ],
      [ "Конфигурация логгера", "index.html#subsec_logger_configuration", null ],
      [ "Пример программной конфигурации", "index.html#subsec_logger_config_example", null ],
      [ "Конфигурация по умолчанию", "index.html#subsec_default_config", null ]
    ] ],
    [ "Интеграция пользовательских типов с <span class=\"tt\">iridium::convertion::convert</span>", "index.html#sec_custom_type_conversion", [
      [ "Пример: Пользовательский тип <span class=\"tt\">Point</span>", "index.html#subsec_custom_convert_example_fn", null ],
      [ "Влияние пользовательских специализаций <span class=\"tt\">convert</span>", "index.html#subsec_custom_convert_fn_impact", null ]
    ] ],
    [ "Обработка перечислений с помощью DEFINE_ENUM", "index.html#sec_define_enum", [
      [ "Пример использования", "index.html#subsec_define_enum_example", null ],
      [ "Влияние <span class=\"tt\">DEFINE_ENUM</span>", "index.html#subsec_define_enum_impact", null ]
    ] ],
    [ "Примитивы многопоточности", "index.html#sec_threading", [
      [ "Использование CThread для прямого управления потоками", "index.html#subsec_cthread", null ],
      [ "Использование Synchronized для защиты данных", "index.html#subsec_synchronized", null ],
      [ "Ожидание по условию и прерывание", "index.html#subsec_synchronized_wait", null ],
      [ "Логирование операций мьютекса", "index.html#subsec_synchronized_logging", null ],
      [ "Использование CWorker для обработки задач", "index.html#subsec_cworker", null ],
      [ "Использование CWorkerPool для параллельной обработки задач", "index.html#subsec_cworkerpool", null ]
    ] ],
    [ "Тестирование с Iridium", "index.html#sec_testing", [
      [ "Основные концепции", "index.html#subsec_testing_core_concepts", null ],
      [ "Написание тестов", "index.html#subsec_testing_writing_tests", [
        [ "Макрос TEST", "index.html#subsubsec_testing_test_macro", null ],
        [ "Макрос утверждений ASSERT", "index.html#subsubsec_testing_assertions", null ]
      ] ],
      [ "Запуск тестов", "index.html#subsec_testing_running_tests", [
        [ "Макрос IMPLEMENT_TEST_MAIN", "index.html#subsubsec_testing_main", null ],
        [ "Роль класса Tester", "index.html#subsubsec_testing_tester", null ],
        [ "Интерфейс ITestRunner (для продвинутых сценариев)", "index.html#subsubsec_testing_itestrunner", null ],
        [ "Аргументы командной строки", "index.html#subsubsec_testing_cli", null ]
      ] ],
      [ "Мокирование зависимостей", "index.html#subsec_testing_mocking", [
        [ "Определение мок-классов", "index.html#subsubsec_testing_defining_mocks", null ],
        [ "Определение поведения моков", "index.html#subsubsec_testing_mock_behavior", null ],
        [ "Автоматическое создание и предоставление мок-объектов через <span class=\"tt\">create()</span>", "index.html#subsubsec_testing_mock_auto_creation", null ]
      ] ],
      [ "Тестирование последовательностей вызовов", "index.html#subsec_testing_sequences", [
        [ "Определение последовательности (DEFINE_MOCK_SEQUENCE)", "index.html#subsubsec_testing_defining_sequences", null ],
        [ "Ожидания в последовательности (DEFINE_MOCK_SEQUENCE_EXPECTATION)", "index.html#subsubsec_testing_sequence_expectations", null ]
      ] ]
    ] ],
    [ "Версионирование", "index.html#sec_versioning", null ],
    [ "Интегрированная система сборки (CMake и Conan)", "index.html#sec_build_system", [
      [ "Необходимая структура проекта", "index.html#subsec_project_structure", null ],
      [ "Интеграция с CMake", "index.html#subsec_cmake_integration", null ],
      [ "Интеграция с Conan", "index.html#subsec_conan_integration", null ]
    ] ],
    [ "Benchmark", "index.html#sec_benchmark", null ],
    [ "Пространства имен", "namespaces.html", [
      [ "Пространства имен", "namespaces.html", "namespaces_dup" ],
      [ "Члены пространств имен", "namespacemembers.html", [
        [ "Указатель", "namespacemembers.html", null ],
        [ "Функции", "namespacemembers_func.html", null ],
        [ "Переменные", "namespacemembers_vars.html", null ],
        [ "Определения типов", "namespacemembers_type.html", null ]
      ] ]
    ] ],
    [ "Классы", "annotated.html", [
      [ "Классы", "annotated.html", "annotated_dup" ],
      [ "Алфавитный указатель классов", "classes.html", null ],
      [ "Иерархия классов", "hierarchy.html", "hierarchy" ],
      [ "Члены классов", "functions.html", [
        [ "Указатель", "functions.html", "functions_dup" ],
        [ "Функции", "functions_func.html", "functions_func" ],
        [ "Переменные", "functions_vars.html", null ],
        [ "Определения типов", "functions_type.html", null ],
        [ "Относящиеся к классу:", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Файлы", "files.html", [
      [ "Файлы", "files.html", "files_dup" ],
      [ "Список членов всех файлов", "globals.html", [
        [ "Указатель", "globals.html", null ],
        [ "Функции", "globals_func.html", null ],
        [ "Определения типов", "globals_type.html", null ],
        [ "Макросы", "globals_defs.html", null ]
      ] ]
    ] ],
    [ "Язык | Language", "user", null ],
    [ "English", "../../en/html/index.html", null ],
    [ "Русский", "../../ru/html/index.html", null ]
  ] ]
];

var NAVTREEINDEX =
[
"../../en/html/index.html",
"class_u_r_i.html#a254db5754960732769383810fc9bd1be",
"classiridium_1_1io_1_1_i_pipe_manager.html#acb974fb7adf77c72f677b3f28de6dfa9",
"classiridium_1_1io_1_1implementation_1_1_c_multiplexer_base.html#a5feeb27b61d3c8d7713ba42d1afddf26",
"classiridium_1_1parsing_1_1_schema_node_3_01_t_schema_type_1_1_a_r_r_a_y_00_01_t_item_01_4.html#a53e74e91b3fb02e8df8389cfa21287e9",
"classiridium_1_1testing_1_1_unit_test_case.html#a530027791e4105481d575702ff62db15",
"classiridium_1_1threading_1_1implementation_1_1_c_worker_pool_pusher.html#a97dba58842169d039211c207cdeaa5a3",
"functions_func_u.html",
"namespaceiridium_1_1convertion.html#aaf31a5335a6afae5d2fdffcdd0b83483",
"schema_8cpp.html#a6555320a6f5bcbfb0a9f05a3522fd929",
"structiridium_1_1io_1_1_t_event.html#a37c215fef882773544c3f373d0493e11",
"unix_2stream__port__acceptor_8cpp.html"
];

const SYNCONMSG = 'нажмите на выключить для синхронизации панелей';
const SYNCOFFMSG = 'нажмите на включить для синхронизации панелей';
const LISTOFALLMEMBERS = 'Полный список членов класса';