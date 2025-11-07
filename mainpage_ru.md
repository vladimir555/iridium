<div style="float: left; margin-right: 15px; max-height: 50px;">
@image html logo.png width=100px
</div>

Добро пожаловать в документацию по библиотеке Iridium!

Эта документация предоставляет информацию о том, как использовать различные компоненты библиотеки Iridium. Изучите разделы ниже, чтобы узнать о ее возможностях.

@section sec_parsers_serializers Парсеры и сериализаторы

Библиотека Iridium предоставляет гибкий механизм для парсинга данных из строковых представлений в структурированное дерево узлов и для сериализации (компоновки) этих деревьев узлов обратно в строки. В основном это достигается через интерфейс `iridium::parsing::IParser`.

@subsection subsec_iparser Интерфейс IParser

Интерфейс `iridium::parsing::IParser` (определенный в `iridium/parsing/parser.h`) является основным компонентом для парсинга и сериализации. Он предоставляет два основных метода:
- `parse(const std::string& source)`: Принимает строку в качестве входных данных и возвращает `iridium::parsing::INode::TSharedPtr`, который является корнем древовидной структуры, представляющей разобранные данные.
- `compose(iridium::parsing::INode::TConstSharedPtr const& root_node)`: Принимает корневой узел древовидной структуры и возвращает его строковое представление.

@subsection subsec_obtaining_parser Получение парсера

Конкретные реализации парсеров предоставляются для различных форматов данных. Например, чтобы создать JSON-парсер, вы можете использовать его статический метод `create()`:

@code{.cpp}
#include "iridium/parsing/implementation/parser_json.h" // Для CJSONParser
#include "iridium/parsing/parser.h" // Для IParser
#include "iridium/parsing/node.h"   // Для INode
#include <string>
#include <iostream>

int main() {
    iridium::parsing::IParser::TSharedPtr jsonParser = iridium::parsing::implementation::CJSONParser::create();

    if (jsonParser) {
        // Используйте парсер...
        std::cout << "JSON Parser created successfully." << std::endl;
    } else {
        std::cerr << "Failed to create JSON parser." << std::endl;
    }
    return 0;
}
@endcode

@subsection subsec_parsing_example Пример парсинга (JSON)

Вот как вы можете разобрать JSON-строку в дерево `INode`:

@code{.cpp}
#include "iridium/parsing/implementation/parser_json.h"
#include "iridium/parsing/parser.h"
#include "iridium/parsing/node.h"
#include <string>
#include <iostream>

int main() {
    iridium::parsing::IParser::TSharedPtr jsonParser = iridium::parsing::implementation::CJSONParser::create();
    if (!jsonParser) {
        std::cerr << "Failed to create JSON parser." << std::endl;
        return 1;
    }

    std::string json_string = R"({ "name": "IridiumApp", "version": 1.0, "modules": ["parsing", "logging"] })";

    try {
        iridium::parsing::INode::TSharedPtr rootNode = jsonParser->parse(json_string);

        if (rootNode) {
            std::cout << "JSON parsed successfully!" << std::endl;
            // Теперь вы можете исследовать структуру дерева rootNode.
            // Например, чтобы получить атрибут 'name':
            iridium::parsing::INode::TSharedPtr nameNode = rootNode->getChild("name");
            if (nameNode) {
                std::cout << "Application Name: " << nameNode->getValue() << std::endl;
            }
        } else {
            std::cout << "Parsing returned a null node (empty input or other non-error case)." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Parsing failed: " << e.what() << std::endl;
    }

    return 0;
}
@endcode

@subsection subsec_serializing_example Пример сериализации (JSON)

Чтобы сериализовать дерево `INode` (например, созданное программно или полученное в результате парсинга) обратно в JSON-строку:

@code{.cpp}
#include "iridium/parsing/implementation/parser_json.h"
#include "iridium/parsing/parser.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/implementation/node.h" // Для CNode
#include <string>
#include <iostream>

int main() {
    iridium::parsing::IParser::TSharedPtr jsonParser = iridium::parsing::implementation::CJSONParser::create();
    if (!jsonParser) {
        std::cerr << "Failed to create JSON parser." << std::endl;
        return 1;
    }

    // Создание простой структуры узлов программно
    iridium::parsing::INode::TSharedPtr root = iridium::parsing::implementation::CNode::create("config");
    root->addChild("setting1", "value1");
    iridium::parsing::INode::TSharedPtr subNode = root->addChild("advanced_settings");
    subNode->addChild("timeout", "100");
    subNode->addChild("retries", "3");

    try {
        std::string composed_json = jsonParser->compose(root);
        std::cout << "Composed JSON: " << composed_json << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Composition failed: " << e.what() << std::endl;
    }

    return 0;
}
@endcode

@subsection subsec_serialization_macros Типизированная сериализация с NodeView

Для более сложных структур данных C++ библиотека Iridium предлагает мощный фреймворк сериализации/десериализации, основанный на `iridium::parsing::serialization::NodeView` и наборе макросов (таких как `DEFINE_ROOT_NODE_BEGIN`, `DEFINE_ATTRIBUTE`, `DEFINE_NODE_LIST_BEGIN` и т.д.). Эти инструменты позволяют определять структуры и классы C++, которые могут быть легко преобразованы в дерево `INode` и обратно, а затем в строковые форматы, такие как JSON, и обратно.

Эта система обеспечивает типизированный доступ к дереву узлов и автоматизирует большую часть шаблонного кода для сериализации и десериализации. Для получения подробной информации об использовании и примерах, пожалуйста, обратитесь к заголовочному файлу `iridium/parsing/serialization/node.h`.

Вот более конкретный пример использования этих макросов:

@code{.cpp}
#include "iridium/parsing/serialization/node.h" // Для макросов сериализации
#include "iridium/parsing/implementation/parser_json.h" // Для CJSONParser
#include "iridium/parsing/implementation/node.h" // Для CNode (используется макросами)
#include "iridium/convertion/convert.h" // Для convert (используется макросами)
#include <iostream>
#include <string>
#include <list> // Используется NodeViewList

// Требуется для компиляции макросов, если необходимы преобразования строк.
// Обычно у вас должны быть эти специализации для ваших пользовательских типов, если они не являются базовыми.

// Определение простой структуры элемента данных
DEFINE_ROOT_NODE_BEGIN(MyDataItem) // Корневой узел для прямого использования или вложения, по умолчанию использует разделитель '-'
    DEFINE_ATTRIBUTE(std::string, Name, "DefaultName")
    DEFINE_ATTRIBUTE(int, Value, 0)
DEFINE_ROOT_NODE_END()

// Определение более сложной структуры настроек
DEFINE_ROOT_NODE_BEGIN(MySettings) // Корневой узел, использует разделитель '-'
    DEFINE_ATTRIBUTE(std::string, ConfigName, "DefaultConfig")
    DEFINE_ATTRIBUTE(bool, IsEnabled, true)

    // Определение вложенного узла с использованием ранее определенной структуры корневого узла
    DEFINE_NODE_EXTERNAL(MyDataItem) // Имя члена будет 'MyDataItem', имя узла 'my-data-item'

    // Определение списка MyDataItem
    // Член списка будет называться 'ItemList', а каждый узел элемента будет называться 'item'
    DEFINE_NODE_LIST_BEGIN(Item) // Это определяет TItem и TItemList
        DEFINE_ATTRIBUTE(std::string, ItemName, "DefaultItem")
        DEFINE_ATTRIBUTE(int, ItemId, 0)
    DEFINE_NODE_LIST_END(Item) // Член с именем 'Item' типа TItemList
DEFINE_ROOT_NODE_END()

int main() {
    // Создание экземпляра JSON-парсера
    auto jsonParser = iridium::parsing::implementation::CJSONParser::create();
    if (!jsonParser) {
        std::cerr << "Failed to create JSON parser." << std::endl;
        return 1;
    }

    // --- Пример сериализации ---
    MySettings settingsToSerialize;
    settingsToSerialize.ConfigName = "UserSettings";
    settingsToSerialize.IsEnabled = true;

    settingsToSerialize.MyDataItem.Name = "Special Item";
    settingsToSerialize.MyDataItem.Value = 123;

    // Добавление элементов в список
    MySettings::TItem item1; // Примечание: TItem вложен в MySettings из-за DEFINE_NODE_LIST_BEGIN
    item1.ItemName = "FirstListItem";
    item1.ItemId = 1;
    settingsToSerialize.Item.add(item1);

    MySettings::TItem item2;
    item2.ItemName = "SecondListItem";
    item2.ItemId = 2;
    settingsToSerialize.Item.add(item2);

    try {
        // Получение INode из нашего объекта настроек и его компоновка в JSON
        std::string jsonOutput = jsonParser->compose(settingsToSerialize.getNode());
        std::cout << "Serialized JSON:" << std::endl << jsonOutput << std::endl << std::endl;

        // Ожидаемая структура вывода:
        // {
        //   "config-name": "UserSettings",
        //   "is-enabled": "true",
        //   "my-data-item": {
        //     "name": "Special Item",
        //     "value": "123"
        //   },
        //   "item": [
        //     { "item-name": "FirstListItem", "item-id": "1" },
        //     { "item-name": "SecondListItem", "item-id": "2" }
        //   ]
        // }

    } catch (const std::exception& e) {
        std::cerr << "Serialization failed: " << e.what() << std::endl;
    }

    // --- Пример десериализации ---
    std::string jsonInput = R"({
        "config-name": "LoadedSettings",
        "is-enabled": "false",
        "my-data-item": {
            "name": "LoadedItem",
            "value": "456"
        },
        "item": [
            { "item-name": "LoadedListItem1", "item-id": "10" },
            { "item-name": "LoadedListItem2", "item-id": "20" }
        ]
    })";

    try {
        iridium::parsing::INode::TSharedPtr rootNode = jsonParser->parse(jsonInput);
        if (rootNode) {
            MySettings deserializedSettings(rootNode); // Создание MySettings из INode

            std::cout << "Deserialized Settings:" << std::endl;
            std::cout << "Config Name: " << deserializedSettings.ConfigName.get() << std::endl;
            std::cout << "Is Enabled: " << (deserializedSettings.IsEnabled.get() ? "true" : "false") << std::endl;
            std::cout << "Nested Item Name: " << deserializedSettings.MyDataItem.Name.get() << std::endl;
            std::cout << "Nested Item Value: " << deserializedSettings.MyDataItem.Value.get() << std::endl;

            std::cout << "List Items:" << std::endl;
            for (const auto& item : deserializedSettings.Item) {
                std::cout << "  - Name: " << item.ItemName.get() << ", ID: " << item.ItemId.get() << std::endl;
            }
        } else {
            std::cerr << "Parsing input JSON returned a null node." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Deserialization failed: " << e.what() << std::endl;
    }

    return 0;
}
@endcode

Этот пример демонстрирует определение структур с атрибутами, вложенными узлами и списками узлов. Затем он показывает, как заполнить экземпляр такой структуры, сериализовать его в JSON и десериализовать JSON обратно в структуру, используя интерфейс `IParser` в сочетании с представлениями узлов, генерируемыми макросами. Метод `getNode()` в структуре, определенной макросом, предоставляет `INode`, необходимый для `compose`, а конструктор структуры может принимать `INode` (из `parse`) для своего заполнения. Помните, что фактические имена узлов в JSON (например, "config-name") выводятся из имен членов C++ путем преобразования CamelCase и вставки разделителя (по умолчанию '-').

@section sec_logging Использование и конфигурация логгера

Библиотека Iridium включает гибкую систему логирования для помощи в инструментировании вашего приложения и диагностике проблем. Она поддерживает несколько мест назначения вывода (приемников), настраиваемые уровни логирования и асинхронное логирование.

@subsection subsec_logging_macros Базовое логирование

Самый простой способ логирования сообщений — использование предоставленных макросов логирования:
- `LOGT << "Trace message";` (Уровень трассировки, часто включает файл/строку, обычно для отладочных сборок)
- `LOGD << "Debug message";` (Уровень отладки, обычно для отладочных сборок)
- `LOGI << "Informational message";` (Информационный уровень)
- `LOGW << "Warning message";` (Уровень предупреждения)
- `LOGE << "Error message";` (Уровень ошибки)
- `LOGF << "Fatal message";` (Фатальный уровень)

Эти макросы создают временный объект потока лога. Вы можете передавать в них данные с помощью оператора `<<`, аналогично `std::cout`. Когда оператор завершается, сообщение лога автоматически форматируется и отправляется в настроенные приемники логов.

@code{.cpp}
#include "iridium/logging/logger.h" // Основной заголовочный файл для логирования

int main() {
    // Базовая конфигурация (вывод в консоль по умолчанию) обычно настраивается, если не предоставлена пользовательская конфигурация.
    // Для большего контроля см. примеры конфигурации ниже.
    // iridium::logging::setConfig(iridium::logging::config::createDefault()); // Пример установки по умолчанию

    int count = 42;
    LOGI << "Application started. Current count: " << count;
    LOGD << "This is a debug message. It might not appear in release builds.";

    if (count > 100) {
        LOGW << "Count is higher than expected: " << count;
    }

    // LOGE << "An example error occurred.";
    // LOGF << "A fatal error occurred, application might terminate.";

    return 0;
}
@endcode

@subsection subsec_logger_configuration Конфигурация логгера

Логгер настраивается с помощью структуры `iridium::logging::config::TLogger`, которая определена в `iridium/logging/config.h`. Эту структуру можно заполнить программно или потенциально загрузить из конфигурационного файла (если он разобран в дерево `INode`).

Ключевые поля в `iridium::logging::config::TLogger`:
- `IsGmtTime` (bool): Если `true`, временные метки указаны в GMT/UTC; в противном случае используется местное время. По умолчанию `false`.
- `Level` (`iridium::logging::TEvent::TLevel`): Глобальный уровень логирования по умолчанию. Конкретные приемники могут переопределять это значение. По умолчанию `TRACE`.
- `Sink` (список объектов `TLogger::TSink`): Определяет одно или несколько мест назначения вывода для сообщений лога.

Каждый объект `TLogger::TSink` в списке `Sink` имеет следующие ключевые поля:
- `Type` (`iridium::logging::config::TLogger::TSink::TSinkType`): Указывает тип приемника. Распространенные значения:
    - `CONSOLE`: Вывод в стандартную консоль.
    - `FILE`: Вывод в один файл.
    - `FILE_DAILY`: Вывод в файл, который ротируется ежедневно.
    - `FILE_TIMELY`: Вывод в новый файл каждый раз при запуске логгера.
- `IsAsync` (bool): Если `true`, этот приемник обрабатывает сообщения лога асинхронно в отдельном потоке. По умолчанию `false`.
- `Uri` (std::string): Для файловых приемников (`FILE`, `FILE_DAILY`, `FILE_TIMELY`) это путь к файлу лога.
- `Level` (`iridium::logging::TEvent::TLevel`): Конкретный уровень логирования для этого приемника. Если установлено значение `UNKNOWN`, он наследует глобальный уровень логгера. В противном случае он переопределяет глобальный уровень для этого конкретного приемника.

@subsection subsec_logger_config_example Пример программной конфигурации

Вот как настроить логгер программно:

@code{.cpp}
#include "iridium/logging/logger.h"
#include "iridium/logging/config.h" // Для TLogger, TEvent::TLevel и т.д.

int main() {
    // 1. Создание объекта конфигурации TLogger
    iridium::logging::config::TLogger loggerConfig;

    // Установка глобальных свойств
    loggerConfig.IsGmtTime = false;
    loggerConfig.Level = iridium::logging::TEvent::TLevel::INFO; // Глобальный уровень

    // 2. Конфигурация приемника для консоли
    iridium::logging::config::TLogger::TSink consoleSink;
    consoleSink.Type = iridium::logging::config::TLogger::TSink::TSinkType::CONSOLE;
    consoleSink.IsAsync = false;
    consoleSink.Level = iridium::logging::TEvent::TLevel::DEBUG; // Консоль будет показывать DEBUG и выше
    loggerConfig.Sink.add(consoleSink); // Добавление в список приемников

    // 3. Конфигурация файлового приемника
    iridium::logging::config::TLogger::TSink fileSink;
    fileSink.Type = iridium::logging::config::TLogger::TSink::TSinkType::FILE;
    fileSink.IsAsync = true; // Асинхронное логирование в файл
    fileSink.Uri = "application.log";
    fileSink.Level = iridium::logging::TEvent::TLevel::WARNING; // Файл будет получать только WARNING и выше
    loggerConfig.Sink.add(fileSink);

    // 4. Применение конфигурации
    iridium::logging::setConfig(loggerConfig);

    // Теперь логгер настроен.
    LOGT << "This TRACE message will not appear on console or file.";
    LOGD << "This DEBUG message will appear on the console only.";
    LOGI << "This INFO message will appear on the console only.";
    LOGW << "This WARNING message will appear on console AND in application.log.";
    LOGE << "This ERROR message will also appear on console AND in application.log.";

    return 0;
}
@endcode

@subsection subsec_default_config Конфигурация по умолчанию

Библиотека предоставляет вспомогательную функцию для получения простой конфигурации по умолчанию:
`iridium::logging::config::TLogger defaultConfig = iridium::logging::config::createDefault();`

Эта конфигурация по умолчанию обычно настраивает синхронный консольный приемник с уровнем логирования `TRACE`. Вы можете изменить этот объект конфигурации по умолчанию перед его применением с помощью `iridium::logging::setConfig()`.

Помните, что конфигурацию логгера в идеале следует выполнять на ранних этапах жизненного цикла вашего приложения, до того, как произойдет какое-либо значительное логирование.

@section sec_custom_type_conversion Интеграция пользовательских типов с `iridium::convertion::convert`

Библиотека Iridium предоставляет гибкий способ интеграции ваших пользовательских типов данных с ее системами, такими как логирование и макросы сериализации. Это достигается путем предоставления специализаций для шаблонной функции `iridium::convertion::convert<TargetType, SourceType>` для вашего пользовательского типа.

Библиотека предлагает пару макросов для объявления и реализации этих специализаций:
- **`DEFINE_CONVERT(TTo, TFrom)`**: Этот макрос следует использовать в вашем заголовочном файле (например, рядом с определением вашего пользовательского типа). Он объявляет компилятору, что будет доступна конкретная специализация преобразования.
- **`IMPLEMENT_CONVERT(TTo, TFrom, TFunc)`**: Этот макрос используется в исходном файле (.cpp) для предоставления фактической реализации преобразования. `TFunc` должен быть функцией, функтором или лямбда-выражением, которое принимает аргумент `TFrom const&` и возвращает `TTo`.

@subsection subsec_custom_convert_example_fn Пример: Пользовательский тип `Point`

Проиллюстрируем это на примере пользовательской структуры `Point`.

**1. Определение типа и объявление преобразований (например, в `Point.h`)**
@code{.cpp}
#pragma once // Пример защитного включения заголовка

#include "iridium/convertion/convert.h" // Для DEFINE_CONVERT
#include <string> // Требуется для использования std::string

// Пользовательская структура
struct Point {
    int x;
    int y;

    // Необязательно: для прямого сравнения в примерах или тестах
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Объявление того, что мы предоставим специализации для преобразования
// Point в std::string и std::string в Point.
DEFINE_CONVERT(std::string, Point);
DEFINE_CONVERT(Point, std::string);
@endcode

**2. Реализация преобразований (например, в `Point.cpp`)**
@code{.cpp}
#include "Point.h" // Предполагается, что здесь находятся определения структуры Point и объявления DEFINE_CONVERT

#include "iridium/convertion/convert.h" // Для IMPLEMENT_CONVERT
#include <string>
#include <stdexcept> // Для std::invalid_argument, std::out_of_range

// Вспомогательная функция для преобразования Point в std::string
static std::string localPointToString(const Point& p) {
    return "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ")";
}

// Вспомогательная функция для преобразования std::string в Point
static Point localStringToPoint(const std::string& s) {
    if (s.length() < 3 || s.front() != '(' || s.back() != ')') {
        throw std::invalid_argument("Invalid Point string format: Missing parentheses. Input: " + s);
    }
    std::string content = s.substr(1, s.length() - 2); // Удаление скобок
    size_t comma_pos = content.find(',');
    if (comma_pos == std::string::npos) {
        throw std::invalid_argument("Invalid Point string format: Missing comma. Input: " + s);
    }
    try {
        int x_val = std::stoi(content.substr(0, comma_pos));
        int y_val = std::stoi(content.substr(comma_pos + 1));
        return {x_val, y_val};
    } catch (const std::invalid_argument& e_ia) {
        throw std::invalid_argument("Invalid integer in Point string: " + std::string(e_ia.what()) + ". Input: " + s);
    } catch (const std::out_of_range& e_oor) {
        throw std::out_of_range("Integer out of range in Point string: " + std::string(e_oor.what()) + ". Input: " + s);
    }
}

// Реализация объявленных специализаций преобразования с использованием вспомогательных функций
IMPLEMENT_CONVERT(std::string, Point, localPointToString);
IMPLEMENT_CONVERT(Point, std::string, localStringToPoint);
@endcode

**3. Пример использования (например, в `main.cpp` или тестах)**
@code{.cpp}
#include "Point.h" // Заголовочный файл вашего пользовательского типа
#include "iridium/convertion/convert.h" // Для iridium::convertion::convert
#include "iridium/logging/logger.h"   // Для LOGI (пример использования)
// Для примера макросов сериализации (концептуально)
// #include "iridium/parsing/serialization/node.h"
// #include "iridium/parsing/implementation/parser_json.h"
#include <iostream> // Для std::cout

int main_custom_convert_demo() { // Переименовано, чтобы избежать конфликтов, если mainpage.md компилируется
    Point p1 = {7, 14};

    // Пример логирования:
    // iridium::logging::LogStream обычно использует iridium::convertion::convert<std::string>(value)
    // для типов, у которых нет прямой перегрузки оператора std::ostream& operator<<.
    // Таким образом, после определения преобразования это должно работать:
    // LOGI << "My custom point: " << p1;
    // Для этого примера мы продемонстрируем преобразование явно:
    std::string p1_as_string = iridium::convertion::convert<std::string>(p1);
    std::cout << "Point p1 converted to string: " << p1_as_string << std::endl;


    // Пример десериализации (например, из строки конфигурации):
    std::string input_string = "(100,-200)";
    try {
        Point p2 = iridium::convertion::convert<Point>(input_string);
        std::cout << "String '" << input_string << "' converted to Point: (" << p2.x << "," << p2.y << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error converting string to Point: " << e.what() << std::endl;
    }

    // Как это применяется к макросам сериализации:
    // Если вы определяете сериализуемую структуру следующим образом:
    //
    // DEFINE_ROOT_NODE_BEGIN(MyStructure)
    //   DEFINE_ATTRIBUTE(Point, Position) // Использует Point
    // DEFINE_ROOT_NODE_END()
    //
    // MyStructure data;
    // data.Position = {5,5}; // Установка Point
    //
    // // При сериализации:
    // // auto jsonParser = iridium::parsing::implementation::CJSONParser::create();
    // // std::string json_output = jsonParser->compose(data.getNode());
    // // ...значением узла 'Position' будет "(5,5)" благодаря Convert<std::string, Point>.
    //
    // // При десериализации:
    // // std::string json_input = R"({ "position": "(10,20)" })";
    // // MyStructure loaded_data(jsonParser->parse(json_input));
    // // ...строка "(10,20)" будет преобразована в объект Point для 'Position'
    // //    благодаря Convert<Point, std::string>.

    return 0;
}
@endcode

@subsection subsec_custom_convert_fn_impact Влияние пользовательских специализаций `convert`

**Для логирования:**
После того, как вы предоставите специализацию `DEFINE_CONVERT(std::string, YourCustomType)` и ее реализацию, ваши пользовательские объекты часто можно будет напрямую использовать в операторах логирования (например, `LOGI << yourCustomObject;`). Система логирования обычно вызывает `iridium::convertion::convert<std::string>(yourCustomObject)` для получения строкового представления, если не найдена или не предпочтительна прямая перегрузка `operator<<(std::ostream&, const YourCustomType&)`.

**Для макросов сериализации:**
При использовании макросов сериализации, таких как `DEFINE_ATTRIBUTE(YourCustomType, myField)`:
- Специализация для `Convert<std::string, YourCustomType>` (реализованная через `IMPLEMENT_CONVERT`) используется для преобразования члена `myField` в его строковое представление при сериализации родительской структуры. Эта строка затем сохраняется как значение соответствующего узла (например, строковый атрибут JSON или текстовый узел XML).
- Специализация для `Convert<YourCustomType, std::string>` (реализованная через `IMPLEMENT_CONVERT`) используется во время десериализации. Если парсер считывает строковое значение из входных данных (например, из атрибута JSON), которое необходимо преобразовать в `YourCustomType` для `myField`, будет вызвана эта специализация.

Предоставляя эти специализации функции `convert`, вы позволяете вашим пользовательским типам данных плавно интегрироваться с различными частями библиотеки Iridium, которые полагаются на преобразование типов, особенно для строковых представлений. Не забудьте объявить специализации с помощью `DEFINE_CONVERT` в заголовочном файле и реализовать их с помощью `IMPLEMENT_CONVERT` и вашей логики преобразования в исходном файле.

@section sec_define_enum Обработка перечислений с помощью DEFINE_ENUM

Библиотека Iridium часто предоставляет служебный макрос `DEFINE_ENUM` (обычно ожидаемый в заголовочном файле, таком как `iridium/enum.h` или общем заголовочном файле утилит), чтобы упростить определение типов C++ `enum class` и автоматически интегрировать их с системой `iridium::convertion::convert`. Эта интеграция имеет решающее значение для простого логирования значений перечислений в виде удобочитаемых строк и для их сериализации/десериализации в конфигурационных файлах или форматах обмена данными.

**Ключевая функциональность `DEFINE_ENUM`:**

Когда вы используете `DEFINE_ENUM`, он обычно выполняет следующие действия:
1.  **Определяет `enum class`**: Создает фактический C++ `enum class` с указанным именем и перечислителями.
2.  **Генерирует преобразования строк**: Что наиболее важно, он автоматически генерирует необходимые специализации для `iridium::convertion::convert`, чтобы разрешить:
    - Преобразование значения перечисления в его строковое представление (например, `MyEnum::ValueOne` становится `"ValueOne"`).
    - Преобразование строкового представления обратно в значение перечисления (например, `"ValueOne"` становится `MyEnum::ValueOne`).

**Типичный синтаксис:**

Хотя точный синтаксис может незначительно отличаться в зависимости от конкретной реализации макроса, распространенным шаблоном является:

`DEFINE_ENUM(EnumTypeName, Enumerator1, Enumerator2, ..., [OptionalDefaultOrUnknownEnumerator])`

- `EnumTypeName`: Желаемое имя для вашего `enum class`.
- `Enumerator1, Enumerator2, ...`: Список имен перечислителей.
- `[OptionalDefaultOrUnknownEnumerator]`: Часто последний перечислитель может быть назначен (например, назван `Unknown` или `Default`) для использования в качестве резервного значения, если строка не может быть успешно проанализирована в один из других определенных перечислителей. Если не указано, попытка преобразовать недопустимую строку в тип перечисления может привести к возврату первого перечислителя или, возможно, к ошибке, в зависимости от конструкции макроса.

@subsection subsec_define_enum_example Пример использования

Проиллюстрируем это парой примеров. (Убедитесь, что вы включили заголовочный файл, предоставляющий `DEFINE_ENUM`, например, `#include "iridium/enum.h"`)

@code{.cpp}
// Примечание: Макрос DEFINE_ENUM предоставляется библиотекой Iridium.
// Обычно для его использования вы бы включили заголовочный файл типа <iridium/enum.h>.
// mainpage_example_define_enum.cpp

// Предполагается, что "iridium/enum.h" или аналогичный заголовочный файл, определяющий DEFINE_ENUM
// и "iridium/convertion/convert.h", доступны.
// Для этого примера мы при необходимости имитируем ожидаемое поведение макроса для демонстрации,
// но в идеале он поступает из библиотеки.


// Пример 1: Простое перечисление цветов
DEFINE_ENUM(MyColor,
    Red,    // Сопоставляется с "Red"
    Green,  // Сопоставляется с "Green"
    Blue    // Сопоставляется с "Blue"
) // MyColor будет иметь MyColor::Red, MyColor::Green, MyColor::Blue

// Пример 2: Статус операции с явным значением Unknown/Default
DEFINE_ENUM(OperationStatus,
    Success,
    Pending,
    Failed,
    Unknown // Это может служить значением по умолчанию, если парсинг строки завершается неудачно
)

#include <iostream>
#include "iridium/convertion/convert.h" // Для iridium::convertion::convert
// #include "iridium/logging/logger.h" // Для LOGI (концептуально)

void demo_define_enum_usage() {
    MyColor favoriteColor = MyColor::Green;

    // Преобразование перечисления в строку (например, для логирования или сериализации)
    // Ожидается, что макрос DEFINE_ENUM создал необходимую
    // специализацию iridium::convertion::convert<std::string, MyColor>.
    std::string colorStr = iridium::convertion::convert<std::string>(favoriteColor);
    std::cout << "Favorite color: " << colorStr << std::endl; // Ожидается: "Green"

    // Концептуальное логирование:
    // LOGI << "Current status: " << OperationStatus::Pending; // Выведет "Pending"

    // Преобразование строки в перечисление (например, для десериализации из файла конфигурации)
    std::string inputColorStr = "Blue";
    MyColor parsedColor = iridium::convertion::convert<MyColor>(inputColorStr);
    std::cout << "Parsed color from string '" << inputColorStr << "': "
              << iridium::convertion::convert<std::string>(parsedColor) << std::endl; // Ожидается: "Blue"

    // Пример парсинга недопустимой строки для MyColor
    std::string invalidColorStr = "Purple";
    MyColor defaultOrFirstColor = iridium::convertion::convert<MyColor>(invalidColorStr);
    // Поведение для недопустимой строки зависит от реализации DEFINE_ENUM.
    // Оно может по умолчанию использовать первый перечислитель (Red) или указанное значение 'Unknown', если макрос это поддерживает.
    // Наш концептуальный заполнитель по умолчанию использует 'Unknown', если доступно, или первый, если нет.
    std::cout << "Parsed invalid color string '" << invalidColorStr << "': "
              << iridium::convertion::convert<std::string>(defaultOrFirstColor) << std::endl;

    // Пример с OperationStatus, у которого есть значение по умолчанию 'Unknown'
    std::string invalidStatusStr = "NonExistentStatus";
    OperationStatus status = iridium::convertion::convert<OperationStatus>(invalidStatusStr);
    std::cout << "Parsed invalid status string '" << invalidStatusStr << "': "
              << iridium::convertion::convert<std::string>(status) << std::endl; // Ожидается: "Unknown"

    OperationStatus goodStatus = iridium::convertion::convert<OperationStatus>("Success");
    std::cout << "Parsed valid status string 'Success': "
              << iridium::convertion::convert<std::string>(goodStatus) << std::endl; // Ожидается: "Success"
}
@endcode

@subsection subsec_define_enum_impact Влияние `DEFINE_ENUM`

**Упрощенное логирование:**
Поскольку `DEFINE_ENUM` обычно обрабатывает создание `iridium::convertion::convert<std::string, YourEnumType>`, значения перечислений могут напрямую использоваться в операторах логирования, и они будут автоматически преобразованы в их удобочитаемые строковые имена:
@code{.cpp}
// MyColor color = MyColor::Blue;
// LOGI << "Current color selection: " << color; // Залогирует "Current color selection: Blue"
@endcode

**Бесшовная сериализация:**
Перечисления, определенные с помощью `DEFINE_ENUM`, плавно интегрируются с макросами сериализации Iridium. Если вы используете такое перечисление в качестве типа для атрибута:
@code{.cpp}
// // Предполагая, что MyColor определен с использованием DEFINE_ENUM
// DEFINE_ROOT_NODE_BEGIN(MyWidgetConfig)
//   DEFINE_ATTRIBUTE(MyColor, WidgetColor)
// DEFINE_ROOT_NODE_END()
//
// MyWidgetConfig config;
// config.WidgetColor = MyColor::Red;
//
// // Сериализация 'config' сохранит "Red" для атрибута 'widget-color'.
// // Десериализация структуры, где 'widget-color' равен "Green", установит config.WidgetColor в MyColor::Green.
@endcode
Система сериализации использует автоматически сгенерированные специализации `convert` для обработки преобразования между значениями перечислений и их строковыми представлениями в сериализованных данных (например, JSON, XML).

Таким образом, использование `DEFINE_ENUM` уменьшает количество шаблонного кода, обеспечивает согласованность и делает работу с перечислениями в Iridium намного удобнее. Всегда обращайтесь к конкретному заголовочному файлу `iridium/enum.h` или эквивалентному в вашей версии библиотеки для получения точных возможностей и синтаксиса макроса `DEFINE_ENUM`.

@section sec_threading Примитивы многопоточности

Библиотека Iridium предоставляет несколько примитивов для помощи в разработке многопоточных приложений. К ним относятся инструменты для прямого управления потоками, синхронизации данных и обработки задач с помощью рабочих потоков и пулов.

Примечание: Некоторые конкретные классы, такие как `CThread`, `CWorker` и `CWorkerPool`, находятся в пространстве имен `iridium::threading::implementation`. Хотя у них могут быть фабричные методы `::create()`, предполагающие удобство использования, это пространство имен обычно указывает на то, что они являются фундаментальными блоками, возможно, предназначенными для обертывания высокоуровневыми абстракциями в некоторых случаях. Однако их прямое использование документируется здесь в соответствии с общими потребностями.

@subsection subsec_cthread Использование CThread для прямого управления потоками

`iridium::threading::implementation::CThread` позволяет создавать и управлять выделенным потоком. Он выполняет задачу, определенную объектом, реализующим интерфейс `iridium::threading::IRunnable`.

**Интерфейс `IRunnable`:**
Чтобы определить работу для `CThread`, вы создаете класс, который наследуется от `iridium::threading::IRunnable`. Этот интерфейс требует реализации следующих методов:
- `void initialize()`: Вызывается один раз после запуска потока для настройки.
- `void finalize()`: Вызывается один раз перед присоединением потока для очистки.
- `void run(std::atomic<bool>& is_running)`: Это основная функция, выполняемая потоком. Она должна содержать логику вашей задачи и периодически проверять флаг `is_running`. Когда `is_running` становится `false`, метод `run` должен вернуться, чтобы позволить потоку корректно завершиться.

@code{.cpp}
// mainpage_example_cthread.cpp
#include "iridium/threading/implementation/thread.h" // Для CThread
#include "iridium/threading/runnable.h"       // Для IRunnable
#include "iridium/threading/thread.h"         // Для iridium::threading::sleep, IThread (для getNameStatic)
#include <iostream>
#include <atomic>
#include <chrono> // Для std::chrono::milliseconds
#include <memory> // Для std::make_shared

// 1. Реализация IRunnable
class MyPeriodicTask : public iridium::threading::IRunnable {
public:
    void initialize() override {
        std::cout << "MyPeriodicTask (" << iridium::threading::IThread::getNameStatic()
                  << "): Initializing." << std::endl;
    }

    void run(std::atomic<bool>& is_running) override {
        int counter = 0;
        while (is_running) {
            std::cout << "MyPeriodicTask (" << iridium::threading::IThread::getNameStatic()
                      << "): Working, count: " << ++counter << std::endl;
            // Имитация работы путем сна
            for(int i=0; i < 5 && is_running; ++i) { // Проверяйте is_running чаще во время "работы"
                 iridium::threading::sleep(200);
            }
        }
        std::cout << "MyPeriodicTask (" << iridium::threading::IThread::getNameStatic()
                  << "): Exiting run loop." << std::endl;
    }

    void finalize() override {
        std::cout << "MyPeriodicTask (" << iridium::threading::IThread::getNameStatic()
                  << "): Finalizing." << std::endl;
    }
};

void demo_cthread() {
    // 2. Создание экземпляра вашего IRunnable
    auto task = std::make_shared<MyPeriodicTask>();

    // 3. Создание и управление CThread
    // CThread::create принимает (имя, IRunnable::TSharedPtr)
    auto thread_obj = iridium::threading::implementation::CThread::create("MyWorker1", task);

    try {
        std::cout << "Main: Initializing thread..." << std::endl;
        thread_obj->initialize(); // Запускает поток и ожидает его запуска

        std::cout << "Main: Sleeping for 3 seconds while thread runs..." << std::endl;
        iridium::threading::sleep(3000);

        std::cout << "Main: Finalizing thread..." << std::endl;
        thread_obj->finalize(); // Сигнализирует потоку остановиться и ожидает его присоединения

        std::cout << "Main: Thread finished." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Main: Thread operation failed: " << e.what() << std::endl;
    }
}
@endcode
В этом примере `MyPeriodicTask::run` будет выполняться в новом потоке, управляемом `CThread`. `CThread::initialize()` запускает его, а `CThread::finalize()` устанавливает `is_running` в `false` и присоединяет поток.

@subsection subsec_synchronized Использование Synchronized для защиты данных

`iridium::threading::Synchronized<TMutex, bool is_tracable = false>` — это шаблонный класс, предназначенный для защиты общих данных от одновременного доступа. Обычно он служит базовым классом для классов, содержащих данные, нуждающиеся в защите. Он использует мьютекс (например, `std::mutex`) и условную переменную.

**Ключевой механизм: `LOCK_SCOPE()`**
Макрос `LOCK_SCOPE()` является основным способом взаимодействия с `Synchronized`. При размещении внутри метода класса, наследующего от `Synchronized`, он создает блокировку с ограниченной областью действия (экземпляр `Synchronized::Locker`). Мьютекс захватывается при обнаружении `LOCK_SCOPE()` и автоматически освобождается при выходе из области действия (благодаря RAII).

@code{.cpp}
// mainpage_example_synchronized.cpp
#include "iridium/threading/synchronized.h"
#include <vector>
#include <string>
#include <mutex>    // Для std::mutex
#include <iostream>
#include <chrono>   // Для std::chrono
// #include "iridium/threading/implementation/thread.h" // Для более сложной демонстрации с реальными потоками

// Класс, методы которого будут синхронизированы
class SafeDataContainer : public iridium::threading::Synchronized<std::mutex> {
private:
    std::vector<std::string> items_;
    int access_count_ = 0;

public:
    void addItem(const std::string& item) {
        LOCK_SCOPE(); // Блокировка захвачена для этой области
        items_.push_back(item);
        access_count_++;
        // std::cout << "addItem: Added '" << item << "'. Lock released upon exiting scope." << std::endl;
        // Мьютекс автоматически освобождается здесь деструктором _____locked_scope_____
    }

    void addItems(const std::vector<std::string>& items_to_add) {
        LOCK_SCOPE();
        for(const auto& item_to_add : items_to_add) { // Исправлено имя переменной
            items_.push_back(item_to_add);
        }
        access_count_++;
        // std::cout << "addItems: Added multiple items. Lock released upon exiting scope." << std::endl;
    }

    std::vector<std::string> getItems() {
        LOCK_SCOPE();
        // Возврат по значению гарантирует, что копия будет сделана, пока блокировка удерживается.
        // std::cout << "getItems: Returning copy of items. Lock released upon exiting scope." << std::endl;
        return items_;
    }

    int getAccessCount() {
        LOCK_SCOPE();
        return access_count_;
    }

    void waitUntilItemCountReaches(size_t target_count, std::chrono::milliseconds timeout) {
        LOCK_SCOPE();
        // std::cout << "waitUntilItemCountReaches: Waiting for item count " << target_count << std::endl;
        while (items_.size() < target_count) {
            if (!_____locked_scope_____.wait(timeout)) {
                // std::cout << "waitUntilItemCountReaches: Wait timed out or was interrupted." << std::endl;
                break;
            }
            // std::cout << "waitUntilItemCountReaches: Woke up, current count " << items_.size() << std::endl;
        }
        // if (items_.size() >= target_count) {
        //     std::cout << "waitUntilItemCountReaches: Target count reached." << std::endl;
        // }
    }

    void notifyDataAdded() {
        // Деструктор Synchronized::Locker автоматически вызывает notify_one.
        // Для явного уведомления (например, всех) потребуется доступ к самой условной переменной.
        // Для этого шаблона вы обычно добавляете данные (что захватывает и освобождает блокировку),
        // и освобождение блокировки уведомляет одного ожидающего.
        // Если вам нужен notify_all, он не предоставляется напрямую только через LOCK_SCOPE для этого простого шаблона.
        // Класс Synchronized имеет m_cv.notify_all() в своем методе interrupt().
        // Этот пример полагается на неявное notify_one при уничтожении Locker.
        LOCK_SCOPE(); // Захват блокировки для обеспечения согласованности данных перед потенциальным уведомлением
        // (здесь нет изменения данных, но обычно вы изменяете данные перед уведомлением)
        // std::cout << "notifyDataAdded: Change occurred. Locker destruction will notify one waiter." << std::endl;
    }
};

void demo_synchronized() {
    SafeDataContainer container;
    container.addItem("Apple");
    container.addItem("Banana");

    std::vector<std::string> current_items = container.getItems();
    for (const auto& item : current_items) {
        std::cout << "Item: " << item << std::endl;
    }
    std::cout << "Access count: " << container.getAccessCount() << std::endl;

    // Концептуально: поток может ожидать условия
    // std::thread waiter([&](){
    //    std::cout << "Waiter thread: waiting for 3 items." << std::endl;
    //    container.waitUntilItemCountReaches(3, std::chrono::seconds(2));
    //    std::cout << "Waiter thread: finished waiting. Items: " << container.getItems().size() << std::endl;
    // });
    // std::thread adder([&](){
    //    iridium::threading::sleep(500);
    //    std::cout << "Adder thread: adding 'Cherry'." << std::endl;
    //    container.addItem("Cherry"); // Это уведомит одного ожидающего через деструктор Locker
    // });
    // adder.join();
    // waiter.join();
}
@endcode

@subsection subsec_synchronized_logging Логирование операций мьютекса

Второй шаблонный параметр `is_tracable` в `Synchronized<TMutex, bool is_tracable = false>` можно установить в `true` для включения диагностического логирования операций мьютекса. Это может быть полезно для отладки проблем, связанных с блокировками, или для понимания поведения синхронизации в вашем приложении.

Когда трассировка включена, операции захвата и освобождения мьютекса, выполняемые через `LOCK_SCOPE()`, будут выводить диагностическую информацию.

@code{.cpp}
// mainpage_example_synchronized_traceable.cpp
#include "iridium/threading/synchronized.h"
#include "iridium/threading/thread.h" // Для IThread::getNameStatic для контекста в выводе
#include <mutex>
#include <iostream>
#include <thread> // Для std::thread для демонстрации вывода

// Класс с возможностью трассировки операций мьютекса
class TraceableSafeDataContainer : public iridium::threading::Synchronized<std::mutex, true> {
public:
    void doSomething() {
        LOCK_SCOPE(); // Операции блокировки и разблокировки будут залогированы
        // ... какая-то работа с защищенными данными ...
        std::cout << "TraceableSafeDataContainer (" << iridium::threading::IThread::getNameStatic() 
                  << "): Executing doSomething." << std::endl;
    }
};

void demo_synchronized_traceable() {
    TraceableSafeDataContainer container;
    
    std::cout << "Main thread (" << iridium::threading::IThread::getNameStatic() 
              << "): Calling doSomething." << std::endl;
    container.doSomething();

    std::thread t1([&]() {
        iridium::threading::IThread::setNameStatic("WorkerThread1"); // Устанавливаем имя потока для лога
        std::cout << "WorkerThread1 (" << iridium::threading::IThread::getNameStatic() 
                  << "): Calling doSomething." << std::endl;
        container.doSomething();
    });

    t1.join();
    std::cout << "Main thread (" << iridium::threading::IThread::getNameStatic() 
              << "): WorkerThread1 finished." << std::endl;
}

/*
int main() {
    demo_synchronized_traceable();
    return 0;
}
*/
@endcode

Когда используется `LOCK_SCOPE()` с включенной трассировкой, операторы `printf` (или аналогичный механизм логирования низкого уровня, используемый реализацией) будут выводить информацию о захвате (`LM`) и освобождении (`UM`) мьютекса. Этот вывод обычно включает имя текущего потока (если установлено через `iridium::threading::IThread::setNameStatic()`), исходный файл и номер строки, где была выполнена операция блокировки/разблокировки.

Пример ожидаемого формата вывода:
```
ThreadName LM
path/to/source.cpp:LINE_NUMBER
TraceableSafeDataContainer (ThreadName): Executing doSomething.
ThreadName UM
path/to/source.cpp:LINE_NUMBER
```
Здесь `ThreadName` — это имя потока, `LM` означает "Lock Mutex" (захват мьютекса), а `UM` — "Unlock Mutex" (освобождение мьютекса). `path/to/source.cpp:LINE_NUMBER` указывает точное место в коде, где был вызван `LOCK_SCOPE()`.

Операции ожидания на объекте `Locker` (например, `wait()` или `wait(timeout)`) также логируются с маркером "WM" (Wait Mutex), предоставляя аналогичную контекстную информацию. Это помогает отслеживать, когда потоки начинают ожидать по условной переменной и когда они пробуждаются.

Класс `Synchronized` делает методы `SafeDataContainer` потокобезопасными. Деструктор `Locker` также вызывает `notify_one()` для внутренней условной переменной, которая может использоваться с методами `Locker::wait()`.

@subsection subsec_cworker Использование CWorker для обработки задач

`iridium::threading::implementation::CWorker<TInputItem, TOutputItem>` предоставляет один рабочий поток, который обрабатывает элементы из входной очереди и может помещать результаты в выходную очередь.

**Интерфейс `IWorker::IHandler`:**
Вы определяете класс-обработчик, наследующий от `iridium::threading::IWorker<TInputItem, TOutputItem>::IHandler`. Его основной метод:
- `TOutputItems handle(const TInputItems& items)`: Получает список элементов из входной очереди и должен возвращать список выходных элементов.

@code{.cpp}
// mainpage_example_cworker.cpp
#include "iridium/threading/implementation/worker.h" // Для CWorker
#include "iridium/threading/worker.h" // Для IWorker::IHandler
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // Для std::transform
#include <cctype>    // Для toupper
#include <memory>    // Для std::make_shared

// 1. Определение обработчика
class ToUpperStringHandler : public iridium::threading::IWorker<std::string, std::string>::IHandler {
public:
    void initialize() override { /* std::cout << "Handler: Initializing" << std::endl; */ }
    void finalize() override { /* std::cout << "Handler: Finalizing" << std::endl; */ }

    TOutputItems handle(const TInputItems& items) override {
        TOutputItems results;
        // std::cout << "Handler: Received " << items.size() << " items to process." << std::endl;
        for (const auto& item : items) {
            std::string upper_item = item;
            std::transform(upper_item.begin(), upper_item.end(), upper_item.begin(),
                           [](unsigned char c){ return static_cast<char>(std::toupper(c)); }); // Обеспечение приведения к char
            results.push_back(upper_item + "_processed_by_worker");
        }
        return results;
    }
};

void demo_cworker() {
    auto handler = std::make_shared<ToUpperStringHandler>();
    auto worker = iridium::threading::implementation::CWorker<std::string, std::string>::create("MyStringToUpperWorker", handler);

    try {
        // std::cout << "Main: Initializing worker..." << std::endl;
        worker->initialize();

        // std::cout << "Main: Pushing 'alpha' and 'beta'." << std::endl;
        worker->push("alpha");
        worker->push(std::vector<std::string>{"beta", "gamma"});

        iridium::threading::IWorker<std::string, std::string>::TOutputItems processed_batch;

        // CWorkerRunnable обрабатывает по одному элементу из входной очереди за раз,
        // вызывает обработчик (который получает список из 1 элемента),
        // и обработчик возвращает список выходных элементов.
        // Таким образом, каждый вызов pop() будет извлекать результаты одного вызова handle().

        // Результаты для "alpha"
        processed_batch = worker->pop(true); // true означает ожидание
        for (const auto& item : processed_batch) {
            std::cout << "Main: Popped CWorker result: " << item << std::endl;
        }

        // Результаты для "beta"
        processed_batch = worker->pop(true);
        for (const auto& item : processed_batch) {
            std::cout << "Main: Popped CWorker result: " << item << std::endl;
        }

        // Результаты для "gamma"
        processed_batch = worker->pop(true);
        for (const auto& item : processed_batch) {
            std::cout << "Main: Popped CWorker result: " << item << std::endl;
        }

        // std::cout << "Main: Finalizing worker..." << std::endl;
        worker->finalize();
        // std::cout << "Main: Worker finished." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Main: CWorker operation failed: " << e.what() << std::endl;
    }
}
@endcode

@subsection subsec_cworkerpool Использование CWorkerPool для параллельной обработки задач

`iridium::threading::implementation::CWorkerPool<TInputItem, TOutputItem>` управляет пулом из нескольких рабочих потоков для параллельной обработки элементов из общей входной очереди в общую выходную очередь.

**Ключевые аспекты:**
- Конструктор принимает список общих указателей `IHandler` (`THandlers`).
- Помещенные элементы распределяются между рабочими потоками.

@code{.cpp}
// mainpage_example_cworkerpool.cpp
#include "iridium/threading/implementation/worker_pool.h" // Для CWorkerPool
#include "iridium/threading/worker.h" // Для IWorker::IHandler
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>    // Для std::make_shared
#include <algorithm> // Для std::transform
#include <cctype>    // Для toupper

#ifndef TOUPPERSTRINGHANDLER_DEFINED_FOR_POOL_DEMO // Защита, если включено несколько раз
#define TOUPPERSTRINGHANDLER_DEFINED_FOR_POOL_DEMO
class ToUpperStringHandlerForPool : public iridium::threading::IWorker<std::string, std::string>::IHandler {
public:
    std::string handler_id_;
    ToUpperStringHandlerForPool(const std::string& id) : handler_id_(id) {}
    void initialize() override { /*std::cout << handler_id_ << ": Init" << std::endl;*/ }
    void finalize() override { /*std::cout << handler_id_ << ": Final" << std::endl;*/ }
    TOutputItems handle(const TInputItems& items) override {
        TOutputItems results;
        // std::cout << handler_id_ << ": Handling " << items.size() << " items." << std::endl;
        for (const auto& item : items) {
            std::string upper_item = item;
            std::transform(upper_item.begin(), upper_item.end(), upper_item.begin(),
                           [](unsigned char c){ return static_cast<char>(std::toupper(c)); });
            results.push_back(upper_item + "_processed_by_pool_" + handler_id_);
        }
        return results;
    }
};
#endif

void demo_cworkerpool() {
    int num_pool_threads = 2;
    std::list<iridium::threading::IWorker<std::string, std::string>::IHandler::TSharedPtr> handlers_list;
    for (int i = 0; i < num_pool_threads; ++i) {
        handlers_list.push_back(std::make_shared<ToUpperStringHandlerForPool>("H" + std::to_string(i)));
    }

    auto pool = iridium::threading::implementation::CWorkerPool<std::string, std::string>::create("MyDemoPool", handlers_list);

    try {
        // std::cout << "Main: Initializing worker pool..." << std::endl;
        pool->initialize();

        std::vector<std::string> all_tasks = {"task1", "task2", "task3", "task4", "task5"};
        // std::cout << "Main: Pushing " << all_tasks.size() << " tasks to the pool." << std::endl;
        pool->push(all_tasks); // Помещение пакета. Они будут добавлены в общую входную очередь.

        // Сбор результатов. Каждый рабочий поток обрабатывает по одному элементу из входной очереди за раз,
        // его обработчик создает список выходных элементов, которые добавляются в выходную очередь пула.
        // Таким образом, мы ожидаем извлечь 'all_tasks.size()' списков результатов.
        for (size_t i = 0; i < all_tasks.size(); ++i) {
            auto results_batch = pool->pop(true); // Ожидание пакета результатов от одного вызова обработчика
            for (const auto& res : results_batch) {
                std::cout << "Main: Pool result: " << res << std::endl;
            }
        }

        // std::cout << "Main: Finalizing worker pool..." << std::endl;
        pool->finalize();
        // std::cout << "Main: Worker pool finished." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Main: Worker Pool operation failed: " << e.what() << std::endl;
    }
}
@endcode
`CWorkerPool` эффективен для распараллеливания независимых задач. Порядок результатов из `pop` может не строго соответствовать порядку `push`.

Эти примитивы предоставляют фундаментальные инструменты для создания параллельных и многопоточных приложений с использованием Iridium.

@section sec_testing Тестирование с Iridium

Библиотека Iridium предоставляет встроенный фреймворк для написания и запуска модульных тестов, а также для создания мок-объектов для изоляции тестируемого кода. Этот фреймворк помогает обеспечить качество и надежность кода, разработанного с использованием Iridium.

@subsection subsec_testing_core_concepts Основные концепции

Фундаментом системы тестирования Iridium являются два ключевых интерфейса/класса:

-   `iridium::testing::ITest` (определен в `iridium/testing/test.h`): Это базовый интерфейс для любого запускаемого тестового случая. Каждый класс, представляющий собой тест, должен реализовывать метод `virtual void run() = 0;`. В этом методе заключается логика самого теста, включая вызовы тестируемого кода и проверки утверждений (assertions).

-   `iridium::testing::UnitTest` (определен в `iridium/testing/unit_test.h`): Этот класс служит базовым классом для ваших тестов (обычно неявно через макрос `TEST`). Он предоставляет набор методов для выполнения различных утверждений, таких как проверка равенства, сравнения, истинности условий или выбрасывания ожидаемых исключений. В случае провала утверждения, эти методы генерируют специальное исключение, которое сигнализирует о неудаче теста.

@subsection subsec_testing_writing_tests Написание тестов

Основным инструментом для создания тестовых случаев в Iridium является макрос `TEST`.

@subsubsection subsubsec_testing_test_macro Макрос TEST

Макрос `TEST(TestCaseName)` (определенный в `iridium/testing/tester.h`) используется для определения тестового случая. `TestCaseName` должно быть уникальным идентификатором для вашего теста. Этот макрос автоматически создает класс, наследующий `iridium::testing::ITest` и `iridium::testing::UnitTest`, и регистрирует его в системе тестирования. Вам нужно только предоставить тело для метода `run()`, где будет содержаться логика теста.

Пример:
@code{.cpp}
#include "iridium/testing/tester.h" // Для TEST и макроса ASSERT
#include <string>                   // Для std::string в примере
#include <stdexcept>                // Для std::runtime_error в примере
#include "iridium/convertion/convert.h" // Для iridium::convertion::convert (если нужны свои типы в ASSERT)

// Пример пользовательского типа для демонстрации вывода в ASSERT
struct MyCustomType {
    int id;
    std::string value;

    // Для сравнения в тестах
    bool operator==(const MyCustomType& other) const {
        return id == other.id && value == other.value;
    }
};

// Специализация convert для MyCustomType, чтобы ASSERT мог ее печатать
template<>
std::string iridium::convertion::convert(MyCustomType const &obj) {
    return "MyCustomType(id=" + iridium::convertion::convert<std::string>(obj.id) + ", value="" + obj.value + "")";
}

// Пример тестируемого класса (замените на свой)
class MyClassToTest {
    std::string param_;
public:
    MyClassToTest(const std::string& p = "") : param_(p) {}
    std::string getParameter() const { return param_; }
    int add(int a, int b) { return a + b; }
    MyCustomType getCustomType(int id, const std::string& val) { return {id, val}; }
    void doSomethingThatThrows() { throw std::runtime_error("Expected exception"); }
    void doSomethingThatDoesNotThrow() { /* No-op */ }
};

TEST(MyClassConstruction) {
    MyClassToTest obj("test_param");
    ASSERT(obj.getParameter(), equal, "test_param"); 
}

TEST(MyClassAddsNumbers) {
    MyClassToTest obj;
    ASSERT(obj.add(2, 3), equal, 5);
    ASSERT(obj.add(1, 1), greaterEqual, 2);
    ASSERT(obj.add(1, 1), lessEqual, 2);
    ASSERT(obj.add(5, 5), greater, 9);
    ASSERT(obj.add(1, 1), less, 3);
}

TEST(MyCustomTypeAssertion) {
    MyClassToTest obj;
    MyCustomType expected = {1, "hello"};
    ASSERT(obj.getCustomType(1, "hello"), equal, expected);
}

TEST(BooleanAssertions) {
    ASSERT(true); // Проверка истинности
    bool my_flag = false;
    ASSERT(!my_flag); // Проверка ложности через отрицание
}

TEST(MyClassThrowsException) {
    MyClassToTest obj;
    ASSERT(obj.doSomethingThatThrows(), std::runtime_error);
}

TEST(MyClassDoesNotThrowException) {
    MyClassToTest obj;
    bool did_not_throw = true;
    try {
        obj.doSomethingThatDoesNotThrow();
    } catch (...) {
        did_not_throw = false;
    }
    ASSERT(did_not_throw);
}
@endcode

@subsubsection subsubsec_testing_assertions Макрос утверждений ASSERT

Класс `iridium::testing::UnitTest` (от которого неявно наследуются тесты, созданные макросом `TEST`) предоставляет различные методы для проверки условий. Основным способом их использования в тестах является макрос `ASSERT(...)` (определенный в `iridium/testing/tester.h`). Если утверждение не выполняется, выбрасывается исключение, и тест помечается как проваленный.

Макрос `ASSERT` является вариативным (принимает разное количество аргументов) и используется следующим образом:

1.  **Проверка булева условия: `ASSERT(condition)`**
    -   Используется для проверки истинности (`ASSERT(true_condition);`) или ложности (`ASSERT(!false_condition);`).
    -   Технически, это вызов `ASSERT_1`, который передает `condition` в метод `UnitTest::assert_()`.
    -   Пример: `ASSERT(x > 5);`

2.  **Проверка на сравнение: `ASSERT(value1, comparison_symbol, value2)`**
    -   Используется для различных видов сравнений, где `value1` представляет ожидаемое значение, а `value2` — фактическое (полученное) значение. `comparison_symbol` — это имя метода сравнения из класса `UnitTest`.
    -   При сравнении (`ASSERT(value1, comparison_symbol, value2)`), ожидаемое значение (`value1`) приводится к типу фактического значения (`value2`). Например, в шаблонных методах `iridium::testing::UnitTest` (см. `iridium/testing/unit_test.h`) используется конструкция вида `TRight left_(left);` или `static_cast<TRight>(left)`, где `left` — это `value1` (ожидаемое), а `TRight` — это тип `value2` (фактическое). Таким образом, фактическое сравнение (`left_ == right`, `left_ < right` и т.д.) происходит между значениями одного типа — типа `TRight`. Для преобразования значений в строки при выводе сообщений об ошибках используется `iridium::convertion::convert`.
    -   Технически, это вызов `ASSERT_3`, который вызывает `UnitTest::comparison_symbol(value1, value2, "value1 symbol value2", "file:line")`.
    -   Доступные `comparison_symbol`:
        -   `equal`: Проверяет, что `value1 == value2`.
        -   `less`: Проверяет, что `value1 < value2`.
        -   `lessEqual`: Проверяет, что `value1 <= value2`.
        -   `greater`: Проверяет, что `value1 > value2`.
        -   `greaterEqual`: Проверяет, что `value1 >= value2`.
    -   Примеры:
        @code{.cpp}
        ASSERT(sum, equal, 10);
        ASSERT(count, less, max_count);
        ASSERT(actual_value, greaterEqual, expected_minimum);
        @endcode
    -   Примечание: Хотя вы также можете написать `ASSERT(value1 == value2);` (используя `ASSERT_1`), использование `ASSERT(value1, equal, value2);` предпочтительнее для сравнений, так как это позволяет фреймворку потенциально выводить значения `value1` и `value2` при ошибке более информативно.

3.  **Проверка на выбрасывание исключения: `ASSERT(expression, ExpectedExceptionType)`**
    -   Технически, это вызов `ASSERT_2`, который передает лямбда-функцию с `expression` и `ExpectedExceptionType` в метод `UnitTest::assert_<TFunction, TException>()`.
    -   Проверяет, что выполнение `expression` приводит к выбрасыванию исключения типа `ExpectedExceptionType` (или его наследника).
    -   Пример: `ASSERT(myObject.methodThatThrows(), std::runtime_error);`

4.  **Безусловный провал теста:**
    -   Прямого макроса `FAIL(message)` нет. Для безусловного провала теста используйте `ASSERT(false);`. Вы можете добавить комментарий для пояснения причины.
        @code{.cpp}
        ASSERT(false); // Тест провален, так как условие X не выполнено
        @endcode

**Вывод значений при ошибках и `iridium::convertion::convert`:**
Очень важной особенностью макроса `ASSERT` (особенно при сравнениях) является то, как он отображает значения при сбое утверждения. Чтобы вывести значения `value1` и `value2` (или значение из `condition` для `ASSERT_1`) в читаемом виде, система тестирования использует `iridium::convertion::convert<std::string>(your_value)`.
Если вы используете в `ASSERT` пользовательские типы, убедитесь, что для них существует специализация `iridium::convertion::convert` в `std::string`. В противном случае, вы можете увидеть неинформативный вывод (например, только имя типа или адрес). Подробнее о создании специализаций `convert` см. в разделе @ref sec_custom_type_conversion.

Макрос `ASSERT` автоматически включает имя файла и номер строки в информацию об ошибке, что помогает быстро найти место провала теста.

@subsection subsec_testing_running_tests Запуск тестов

После написания тестов их необходимо скомпилировать и запустить. Фреймворк Iridium упрощает этот процесс.

@subsubsection subsubsec_testing_main Макрос IMPLEMENT_TEST_MAIN

Для создания исполняемого файла, который запустит все определенные тесты, используется макрос `IMPLEMENT_TEST_MAIN()` (определенный в `iridium/testing/tester.h`). Этот макрос генерирует стандартную функцию `main()`, которая инициализирует и запускает тестовый фреймворк.

Обычно вы создаете отдельный `.cpp` файл (например, `tests_main.cpp`), который включает все ваши тестовые файлы (или заголовки, если тесты определены в них) и затем вызывает `IMPLEMENT_TEST_MAIN()`.

Пример (`tests_main.cpp`):
@code{.cpp}
#include "iridium/testing/tester.h" // Для IMPLEMENT_TEST_MAIN

// Включите здесь ваши тестовые файлы или файлы, где определены тесты TEST(...)
// Например:
// #include "my_class_tests.cpp" 
// #include "another_module_tests.cpp"

IMPLEMENT_TEST_MAIN()
@endcode
При компиляции этого файла вместе с вашими тестами и библиотекой Iridium вы получите исполняемый файл, который при запуске выполнит все обнаруженные тесты.

@subsubsection subsubsec_testing_tester Роль класса Tester

Класс `iridium::testing::Tester` (определенный в `iridium/testing/tester.h`) является центральным компонентом системы тестирования. Это синглтон, который:
-   Регистрирует все тестовые случаи, определенные с помощью макроса `TEST`.
-   Управляет выполнением тестов. Метод `Tester::run(argc, argv, main_cpp_path)` вызывается из сгенерированной `main()` функции.
-   Позволяет фильтровать тесты на основе аргументов командной строки (эта функциональность может быть не полностью детализирована здесь, но `Tester` ее предусматривает).
-   Собирает результаты тестов.

@subsubsection subsubsec_testing_itestrunner Интерфейс ITestRunner (для продвинутых сценариев)

Для более гибкого управления процессом выполнения тестов существует интерфейс `iridium::testing::ITestRunner` (определенный в `iridium/testing/test_runner.h`). Этот интерфейс абстрагирует способ запуска тестов. Библиотека Iridium предоставляет как минимум две его реализации:
-   `iridium::testing::implementation::TestRunnerRaw`: Запускает тесты в том же процессе.
-   `iridium::testing::implementation::TestRunnerFork`: Может запускать тесты в отдельных процессах (форках), что обеспечивает лучшую изоляцию (обычно на POSIX-системах).

Хотя прямое взаимодействие с `ITestRunner` обычно не требуется для написания и запуска тестов, знание о его существовании полезно для понимания архитектуры фреймворка и для возможных расширений или кастомизации процесса тестирования. Класс `Tester` использует реализацию `ITestRunner` для фактического выполнения тестов.

@subsubsection subsubsec_testing_cli Аргументы командной строки

Исполняемый файл тестов, созданный с помощью `IMPLEMENT_TEST_MAIN()`, поддерживает следующие аргументы командной строки для управления процессом тестирования. Предположим, ваше приложение для тестов называется `ваше_приложение_тестов`.

1.  **`help`**: Выводит справочную информацию по использованию.
    @code{.sh}
    ./ваше_приложение_тестов help
    @endcode
    Пример вывода:
    @code{.txt}
    main thread: 281473424756000
    2025-06-06 10:30:56.839 I 281473424756000 
    usage:
    ./ваше_приложение_тестов help
    ./ваше_приложение_тестов list
    ./ваше_приложение_тестов run [ --mode=raw|serial|parallel ] [ --print-result=json ] [ --timeout=seconds ] [ include_path ] [ exclude_path ] ... [ exclude_path ]
    example:
    ./ваше_приложение_тестов run / 
    @endcode
    (Примечание: фактический номер потока и временная метка в вашем выводе будут отличаться.)

2.  **`list`**: Отображает иерархический список (дерево) всех тестовых случаев, обнаруженных фреймворком. Тесты группируются по пути к файлу и затем по имени теста, определенному в макросе `TEST()`.
    @code{.sh}
    ./ваше_приложение_тестов list
    @endcode
    Это полезно для просмотра структуры тестов и их полных путей, которые могут использоваться в качестве `путь_включения` или `путь_исключения` для команды `run`. Пример вывода:
    @code{.txt}
    main thread: 281473794346272
    2025-06-06 10:30:22.166 I 281473794346272 
    'root'
      'convertion'
        'convert.cpp'
          'enum_'
          'exception'
          'strings'
          'types'
      'testing'
        'example.cpp'
          'bool_'
          'comparing_equal'
          'exception'
          'mock'
      # ... (и так далее для других модулей и тестов)
    @endcode
    (Примечание: фактический номер потока, временная метка и полный список тестов в вашем выводе будут отличаться.)

3.  **`run`**: Запускает тесты. Это команда по умолчанию, если не указана другая.
    @code{.sh}
    ./ваше_приложение_тестов run [опции] [путь_включения] [путь_исключения_1] [путь_исключения_2] ...
    @endcode
    Стандартный вывод при запуске тестов (без `--print-result=json`) включает логирование каждого запускаемого теста и его результата:
    @code{.txt}
    main thread: 281473243745568
    2025-06-06 10:30:39.744 I 281473243745568 RUN  /testing/example.cpp/bool_
    2025-06-06 10:30:39.745 I 281473243745568 OK   /testing/example.cpp/bool_
    2025-06-06 10:30:39.745 I 281473243745568 RUN  /testing/example.cpp/comparing_equal
    2025-06-06 10:30:39.745 I 281473243745568 OK   /testing/example.cpp/comparing_equal
    # ... (и так далее для других тестов)
    # ... (в случае ошибки будет FAILED вместо OK и сообщение об ошибке)
    2025-06-06 10:30:39.747 I 281473243745568 
    passed: 8
    failed: 0
    total:  51 
    @endcode
    (Примечание: фактические номера потоков, временные метки и результаты в вашем выводе будут отличаться.)

    Доступные опции для команды `run`:
    -   `--mode=РЕЖИМ`: Определяет режим запуска тестов. Возможные значения для `РЕЖИМ`:
        -   `raw` (по умолчанию): Тесты запускаются в том же процессе.
        -   `serial`: Тесты запускаются последовательно, каждый в отдельном процессе.
        -   `parallel`: Тесты запускаются параллельно в отдельных процессах.
    -   `--print-result=ФОРМАТ`: Управляет форматом вывода результатов.
        -   Если не указана, используется стандартный текстовый формат (см. пример выше).
        -   `json`: Результаты выводятся в формате JSON.
    -   `--timeout=СЕКУНДЫ`: Устанавливает максимальное время ожидания для тестов в секундах (по умолчанию 60).

    Параметры команды `run`:
    -   `путь_включения` (необязательный): Если указан, запускаются только тесты, чей путь (как в выводе `list`) начинается с `путь_включения`. По умолчанию `/` (все тесты).
    -   `путь_исключения_...` (необязательные): Один или несколько путей для исключения. Тесты, чей путь начинается с одного из этих путей, не будут запущены.

Примеры использования команды `run`:

-   Запустить все тесты:
    @code{.sh}
    ./ваше_приложение_тестов run
    @endcode
    (или просто `./ваше_приложение_тестов`)

-   Запустить все тесты в параллельном режиме с таймаутом 120 секунд:
    @code{.sh}
    ./ваше_приложение_тестов run --mode=parallel --timeout=120
    @endcode

-   Запустить только тесты из модуля `testing` и файла `example.cpp`:
    @code{.sh}
    ./ваше_приложение_тестов run /testing/example.cpp/
    @endcode

-   Запустить все тесты из модуля `parsing`, но исключить те, что находятся в `node.cpp` внутри `parsing`:
    @code{.sh}
    ./ваше_приложение_тестов run /parsing/ /parsing/node.cpp/
    @endcode

-   Запустить все тесты и вывести результат в формате JSON:
    @code{.sh}
    ./ваше_приложение_тестов run --print-result=json
    @endcode

@subsection subsec_testing_mocking Мокирование зависимостей

Мокирование (от англ. mocking) — это процесс создания объектов-заменителей (моков или мок-объектов) для имитации поведения реальных зависимостей тестируемого компонента. Это позволяет изолировать тестируемый код и сделать тесты более предсказуемыми и стабильными. Фреймворк тестирования Iridium предоставляет инструменты для создания моков (определенные в `iridium/testing/mock.h`).

@subsubsection subsubsec_testing_defining_mocks Определение мок-классов

Для создания мока для интерфейса или класса используется макрос `DEFINE_MOCK_CLASS(InterfaceName)`. Этот макрос создает новый класс с именем `InterfaceNameMock`, который наследует ваш `InterfaceName` и `iridium::testing::Mock<InterfaceName>`.

Внутри этого мок-класса вы объявляете, какие методы интерфейса вы хотите мокировать, используя макросы `DEFINE_MOCK_METHOD` (для неконстантных методов) и `DEFINE_MOCK_METHOD_CONST` (для константных методов).

Пример:
@code{.cpp}
#include "iridium/testing/mock.h"
#include <string>
#include <vector>

// Предположим, это интерфейс, который мы хотим мокировать
class IMyDependency {
public:
    virtual ~IMyDependency() = default;
    virtual int getValue(int key) = 0;
    virtual std::string getName() const = 0;
    virtual void processData(const std::vector<int>& data) = 0;
    // Конструктор с аргументами для демонстрации DEFINE_MOCK_CONSTRUCTOR
    IMyDependency(const std::string& /* initial_config */) {} 
    IMyDependency() = default; // Добавим конструктор по умолчанию, если он тоже нужен
};

// Определение мок-класса для IMyDependency
DEFINE_MOCK_CLASS(IMyDependency) {
public:
    // Если базовый класс IMyDependency имеет конструктор с аргументами,
    // и вы хотите его вызывать из мока:
    DEFINE_MOCK_CONSTRUCTOR(IMyDependency)
    
    // Мокирование методов интерфейса
    DEFINE_MOCK_METHOD(int, getValue, int)          // int getValue(int key)
    DEFINE_MOCK_METHOD_CONST(std::string, getName)  // std::string getName() const
    DEFINE_MOCK_METHOD(void, processData, const std::vector<int> &) // void processData(const std::vector<int>& data)
};
@endcode
Макросы `DEFINE_MOCK_METHOD` принимают тип возвращаемого значения, имя метода и в круглых скобках типы аргументов метода (без имен переменных).

@subsubsection subsubsec_testing_mock_behavior Определение поведения моков

После создания экземпляра мок-объекта вы можете определить его поведение с помощью макроса `DEFINE_MOCK_BEHAVIOR` (для неконстантных методов) или `DEFINE_MOCK_BEHAVIOR_CONST` (для константных методов). Этот макрос позволяет присвоить лямбда-функцию, которая будет вызываться при обращении к мокированному методу.

Пример:
@code{.cpp}
#include "iridium/testing/tester.h" // Для ASSERT
#include "iridium/testing/mock.h"   // Для макросов мокирования
#include <string>
#include <vector>
#include <stdexcept> // Для std::runtime_error

// Для полноты примера, скопируем определения IMyDependency и IMyDependencyMock сюда
// В реальном коде они были бы в заголовочных файлах
class IMyDependency {
public:
    virtual ~IMyDependency() = default;
    virtual int getValue(int key) = 0;
    virtual std::string getName() const = 0;
    virtual void processData(const std::vector<int>& data) = 0;
    IMyDependency(const std::string& /* initial_config */) {} 
    IMyDependency() = default; 
};

DEFINE_MOCK_CLASS(IMyDependency) {
public:
    DEFINE_MOCK_CONSTRUCTOR(IMyDependency)
    DEFINE_MOCK_METHOD(int, getValue, int)
    DEFINE_MOCK_METHOD_CONST(std::string, getName)
    DEFINE_MOCK_METHOD(void, processData, const std::vector<int> &)
};
// Конец скопированных определений


// Класс, который использует IMyDependency
class MyClassUsesDependency {
    IMyDependency* dependency_;
public:
    MyClassUsesDependency(IMyDependency* dep) : dependency_(dep) {}

    int fetchValue(int key) {
        if (key < 0) {
            throw std::runtime_error("Key cannot be negative");
        }
        return dependency_->getValue(key);
    }

    std::string getDepName() const {
        return dependency_->getName();
    }

    void sendData(const std::vector<int>& data) {
        dependency_->processData(data);
    }
};


TEST(MyClassUsesDependency_Behavior) {
    IMyDependencyMock mockDep; // Создаем экземпляр мока

    // Определяем поведение для getValue
    DEFINE_MOCK_BEHAVIOR(int, getValue, mockDep, int key) {
        // Это лямбда-функция: [=](int key_param) -> int { ... }
        // Имена параметров в лямбде могут быть любыми, но типы должны совпадать с объявленными в DEFINE_MOCK_METHOD
        if (key == 1) return 100;
        if (key == 42) return 420;
        return -1;
    };

    // Определяем поведение для getName (константный метод)
    DEFINE_MOCK_BEHAVIOR_CONST(std::string, getName, mockDep) {
        // Лямбда для метода без аргументов: [=]() -> std::string { ... }
        return "MockedName";
    };
    
    // Определяем поведение для processData (void метод)
    std::vector<int> received_data;
    DEFINE_MOCK_BEHAVIOR(void, processData, mockDep, const std::vector<int> &data) {
        // Лямбда для void метода: [=](const std::vector<int>& data_param) -> void { ... }
        received_data = data; // Копируем данные для проверки
    };

    MyClassUsesDependency mainObj(&mockDep);

    ASSERT(mainObj.fetchValue(1),   equal, 100);
    ASSERT(mainObj.fetchValue(42),  equal, 420);
    ASSERT(mainObj.fetchValue(10),  equal, -1);
    ASSERT(mainObj.getDepName(),    equal, "MockedName");

    std::vector<int> data_to_send = {1, 2, 3};
    mainObj.sendData(data_to_send);
    ASSERT(received_data.size(),    equal, 3);
    ASSERT(received_data[0],        equal, 1);
    
    // Проверка вызова исключения из основного класса, не из мока
    ASSERT(mainObj.fetchValue(-1), std::runtime_error);
}
@endcode
В лямбда-функции, определяющей поведение, вы можете получить доступ к аргументам, с которыми был вызван мокированный метод, и вернуть соответствующее значение или выполнить необходимые действия. Типы аргументов в лямбде должны совпадать с типами, указанными в `DEFINE_MOCK_METHOD`.

Использование `DEFINE_MOCK_CONSTRUCTOR` необходимо, если ваш исходный интерфейс/класс имеет конструкторы с параметрами, которые вы хотите вызывать при создании мок-объекта (например, если мок наследует от класса, а не чистого интерфейса, и конструктор базового класса должен быть вызван).

@subsubsection subsubsec_testing_mock_auto_creation Автоматическое создание и предоставление мок-объектов через `create()`

Фреймворк Iridium предоставляет механизм, позволяющий статическим методам `create()` возвращать мок-объекты вместо реальных во время тестирования. Это особенно полезно, когда тестируемый класс создает свои зависимости внутри себя. Понимание этого механизма требует рассмотрения нескольких макросов и пользовательских соглашений.

**Ключевые макросы и их взаимодействие:**

1.  **`DEFINE_MOCK_CREATE` (символ препроцессора):**
    *   Это **символ препроцессора**, который пользователи должны определять для своих тестовых сборок. Это можно сделать, например, добавив `#define DEFINE_MOCK_CREATE` в начале тестового файла (до включения заголовочных файлов Iridium, таких как `iridium/smart_ptr.h`) или через опцию компилятора (например, `-DDEFINE_MOCK_CREATE`).
    *   Макрос Iridium `DEFINE_CREATE(TClass)` (находящийся в `iridium/smart_ptr.h`) использует директиву препроцессора `#ifdef DEFINE_MOCK_CREATE` для условной компиляции статического метода `TClass::create(...)`.

2.  **`DEFINE_CREATE(TClass)` (макрос Iridium из `iridium/smart_ptr.h`):**
    *   Этот макрос генерирует статический метод `TClass::create(...)`.
    *   Благодаря проверке `#ifdef DEFINE_MOCK_CREATE` внутри `DEFINE_CREATE`:
        *   Если `DEFINE_MOCK_CREATE` определен на момент обработки `DEFINE_CREATE(TClass)`, то результирующий метод `TClass::create(...)` будет вызывать `iridium::testing::Mock<TClass>::create(...)`.
        *   Если `DEFINE_MOCK_CREATE` *не* определен, то `TClass::create(...)` будет вызывать `std::make_shared<TClass>(...)` для создания реального объекта.

3.  **`iridium::testing::Mock<TClass>::create(...)` (из `iridium/testing/mock.h`):**
    *   Этот метод вызывается, когда `TClass::create(...)` был сгенерирован в "мок-режиме" (благодаря `DEFINE_MOCK_CREATE`).
    *   Он проверяет внутренний статический флаг `Mock<TClass>::m_is_mocked`.
    *   Если `m_is_mocked == true` (этот флаг устанавливается при создании экземпляра мок-класса, например, `TClassMock`), то `Mock<TClass>::create(...)` пытается вернуть ранее зарегистрированный экземпляр `TClassMock`. Если подходящих моков нет, выбрасывается исключение.
    *   Если `m_is_mocked == false`, то `Mock<TClass>::create(...)` пытается создать реальный объект через `std::make_shared<TClass>(...)`. **Важно:** если `TClass` является абстрактным интерфейсом, эта попытка приведет к ошибке компиляции.

4.  **`DEFINE_IMPLEMENTATION(TClass)` (из `iridium/smart_ptr.h`):**
    *   Этот макрос Iridium используется для конкретных классов. Он внутри себя вызывает `DEFINE_CREATE(TClass)`.
    *   Следовательно, статический метод `create()` для классов, определенных с `DEFINE_IMPLEMENTATION`, будет автоматически поддерживать переключение между реальным созданием и мок-созданием в зависимости от наличия `DEFINE_MOCK_CREATE`.

5.  **`DEFINE_INTERFACE(Interface)` (из `iridium/smart_ptr.h`):**
    *   Этот макрос Iridium используется для определения интерфейсов. Он определяет typedef-ы для умных указателей и виртуальный деструктор.
    *   **Важно:** `DEFINE_INTERFACE` **не использует** `DEFINE_CREATE`. Таким образом, интерфейсы, определенные только с `DEFINE_INTERFACE`, не получают автоматически статический метод `create()`, управляемый `DEFINE_MOCK_CREATE`.

**Сценарии использования:**

*   **Мокирование конкретного класса:** Если у вас есть конкретный класс `CMyImpl`, определенный с `DEFINE_IMPLEMENTATION(CMyImpl)`, и вы определяете `DEFINE_MOCK_CREATE` в вашем тесте, то вызов `CMyImpl::create(...)` автоматически перенаправится на `iridium::testing::Mock<CMyImpl>::create(...)`. Затем, если вы создали экземпляр `CMyImplMock`, он будет предоставлен.

*   **Мокирование через интерфейс:** Если вы хотите, чтобы `IMyInterface::create(...)` возвращал мок, а `IMyInterface` определен только с `DEFINE_INTERFACE`, вам нужно **самостоятельно реализовать** статический метод `IMyInterface::create(...)`. В этой вашей реализации вы можете использовать `DEFINE_MOCK_CREATE` (или другой ваш флаг) для выбора между `std::make_shared<CMyRealImpl>(...)` и `iridium::testing::Mock<IMyInterface>::create(...)`.

**Пример (мокирование конкретной реализации, как в `iridium-test/testing/example.cpp`):**

@code{.cpp}
// --- Начало примера кода ---

// Включаем заголовки Iridium. Порядок важен, если `DEFINE_MOCK_CREATE` влияет на них.
// В данном случае, `DEFINE_MOCK_CREATE` должен быть определен до smart_ptr.h, где находится DEFINE_CREATE.
// Для этого примера, мы разместим DEFINE_MOCK_CREATE прямо в "тестовом файле".

// Файл: IDataService.h
#pragma once
#include "iridium/smart_ptr.h" // Для DEFINE_INTERFACE
#include <string>
#include <memory>

class IDataService {
public:
    DEFINE_INTERFACE(IDataService); // Не создает IDataService::create()
    virtual std::string fetchData(int id) = 0;
};

// Файл: CDataServiceImpl.h
#pragma once
#include "IDataService.h"
#include "iridium/smart_ptr.h" // Для DEFINE_IMPLEMENTATION

class CDataServiceImpl : public IDataService {
    std::string m_serviceName;
public:
    // Конструктор для реальной реализации
    CDataServiceImpl(const std::string& name) : m_serviceName(name) {}

    // DEFINE_IMPLEMENTATION включает DEFINE_CREATE, который будет учитывать DEFINE_MOCK_CREATE
    DEFINE_IMPLEMENTATION(CDataServiceImpl);

    std::string fetchData(int id) override {
        return "Real data for id " + std::to_string(id) + " from " + m_serviceName;
    }
};

// Файл: CDataServiceImplMock.h (или прямо в тестовом .cpp)
#pragma once
#include "CDataServiceImpl.h" // Мокируем конкретный класс
#include "iridium/testing/mock.h"

DEFINE_MOCK_CLASS(CDataServiceImpl) { // Создает CDataServiceImplMock
public:
    // Так как CDataServiceImpl имеет конструктор с параметрами,
    // мок должен его вызвать через DEFINE_MOCK_CONSTRUCTOR.
    DEFINE_MOCK_CONSTRUCTOR(CDataServiceImpl)

    DEFINE_MOCK_METHOD(std::string, fetchData, int);
};

// Файл: DataConsumer.h
#pragma once
#include "CDataServiceImpl.h" // Зависит от конкретной реализации для вызова CDataServiceImpl::create()
#include <memory>
#include <string>

class DataConsumer {
    std::shared_ptr<CDataServiceImpl> m_dataService; // Использует конкретный класс
public:
    DataConsumer() {
        // Вызываем CDataServiceImpl::create(), который будет управляться DEFINE_MOCK_CREATE
        m_dataService = CDataServiceImpl::create("MyRealService");
    }

    std::string processData(int recordId) {
        if (!m_dataService) return "Error: Service not created";
        return "Consumed: " + m_dataService->fetchData(recordId);
    }
};

// Файл: test_main.cpp (или ваш тестовый файл)
// ЭТО КЛЮЧЕВОЙ МОМЕНТ: DEFINE_MOCK_CREATE должен быть определен ПЕРЕД включением
// iridium/smart_ptr.h, если он должен повлиять на DEFINE_CREATE в нем.
// В данном случае, для простоты, мы предполагаем, что заголовки CDataServiceImpl.h
// (который тянет smart_ptr.h) будут обработаны компилятором уже после этого дефайна.
#define DEFINE_MOCK_CREATE

#include "iridium/testing/tester.h"
// #include "DataConsumer.h"       // Уже включены концептуально выше
// #include "CDataServiceImplMock.h"

TEST(DataConsumer_UsesMockService) {
    // 1. Создаем экземпляр мока CDataServiceImplMock.
    //    Это действие установит Mock<CDataServiceImpl>::m_is_mocked = true;
    //    и зарегистрирует данный mockService в списке доступных моков.
    //    Передаем аргумент конструктора, как и для реального CDataServiceImpl.
    CDataServiceImplMock mockService("MockedServiceInstance");

    // 2. Задаем поведение для мок-метода fetchData
    DEFINE_MOCK_BEHAVIOR(std::string, fetchData, mockService, int id) {
        if (id == 101) {
            return "mocked_payload_for_101";
        }
        return "generic_mock_payload";
    };

    // 3. Создаем DataConsumer. Его конструктор вызовет CDataServiceImpl::create("MyRealService").
    //    Поскольку DEFINE_MOCK_CREATE активен, CDataServiceImpl::create() был сгенерирован так,
    //    чтобы вызвать iridium::testing::Mock<CDataServiceImpl>::create(...).
    //    Так как mockService (типа CDataServiceImplMock) был создан и зарегистрирован,
    //    он будет возвращен (аргумент "MyRealService" будет использован конструктором мока, если он его принимает,
    //    или проигнорирован, если Mock<T>::create так решит для моков).
    DataConsumer consumer;

    // 4. Проверяем, что DataConsumer получил и использует мокированные данные
    ASSERT(consumer.processData(101), equal, "Consumed: mocked_payload_for_101");
    ASSERT(consumer.processData(200), equal, "Consumed: generic_mock_payload");
}
// --- Конец примера кода ---
@endcode

**Итог по интерфейсам:**

Если вы хотите получить мок для интерфейса `IExample` через вызов `IExample::create()`, то, поскольку `DEFINE_INTERFACE` не предоставляет `create()`, вы должны написать этот метод сами. Например:

@code{.cpp}
// В IExample.h или IExample.cpp
// #include "CRealExampleImpl.h" // Ваша реальная реализация
// #include "iridium/testing/mock.h" // Для Mock<IExample>::create()

// std::shared_ptr<IExample> IExample::create() { // Предполагаем, что вы объявили static create() в IExample
// #ifdef USER_TEST_FLAG_FOR_IEXAMPLE_MOCK
//     return iridium::testing::Mock<IExample>::create();
// #else
//     return std::make_shared<CRealExampleImpl>();
// #endif
// }
@endcode
В этом случае, `DEFINE_MOCK_CREATE` напрямую не используется Iridium для `IExample::create()`, но вы можете использовать его или аналогичный флаг (`USER_TEST_FLAG_FOR_IEXAMPLE_MOCK`) в вашей собственной реализации `IExample::create()`.

Эта детальная информация должна корректно отражать механизм мокирования при использовании `create()` в Iridium.

@subsection subsec_testing_sequences Тестирование последовательностей вызовов

Иногда важно не только то, какие методы мок-объекта вызываются, но и в каком порядке это происходит. Фреймворк мокирования Iridium предоставляет средства для определения и проверки последовательностей вызовов.

***Примечание:** Эта функциональность может быть неполной или находиться в разработке. Макросы, представленные в коде, могут не соответствовать полной реализации класса `MockSequence`.*

@subsubsection subsubsec_testing_defining_sequences Определение последовательности (DEFINE_MOCK_SEQUENCE)

Макрос `DEFINE_MOCK_SEQUENCE(name)` используется для создания объекта последовательности.
-   `name`: Имя, которое вы даете этой последовательности (будет создана переменная `sequence_<name>`).

@subsubsection subsubsec_testing_sequence_expectations Ожидания в последовательности (DEFINE_MOCK_SEQUENCE_EXPECTATION)

После определения объекта последовательности, вы добавляете в нее ожидаемые вызовы с помощью макроса `DEFINE_MOCK_SEQUENCE_EXPECTATION(sequence_name, mock, method)`.
-   `sequence_name`: Имя ранее определенной последовательности.
-   `mock`: Мок-объект.
-   `method`: Имя мокированного метода, который должен быть вызван.

В текущей реализации этот макрос не позволяет указывать ожидаемые аргументы для вызова метода.
