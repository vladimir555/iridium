<div style="float: left; margin-right: 15px; max-height: 50px;">
@image html logo.png width=100px
</div>

Welcome to the Iridium Library documentation!

This documentation provides information on how to use various components of the Iridium library. Explore the sections below to learn about its features.

@section sec_parsers_serializers Parsers and Serializers

The Iridium library provides a flexible mechanism for parsing data from string representations into a structured node tree, and for serializing (composing) these node trees back into strings. This is primarily achieved through the `iridium::parsing::IParser` interface.

@subsection subsec_iparser The IParser Interface

The `iridium::parsing::IParser` interface (defined in `iridium/parsing/parser.h`) is the core component for parsing and serialization. It exposes two main methods:
- `parse(const std::string& source)`: Takes a string as input and returns an `iridium::parsing::INode::TSharedPtr`, which is the root of a tree structure representing the parsed data.
- `compose(iridium::parsing::INode::TConstSharedPtr const& root_node)`: Takes the root node of a tree structure and returns its string representation.

@subsection subsec_obtaining_parser Obtaining a Parser

Specific parser implementations are provided for different data formats. For example, to create a JSON parser, you can use its static `create()` method:

@code{.cpp}
#include "iridium/parsing/implementation/parser_json.h" // For CJSONParser
#include "iridium/parsing/parser.h" // For IParser
#include "iridium/parsing/node.h"   // For INode
#include <string>
#include <iostream>

int main() {
    iridium::parsing::IParser::TSharedPtr jsonParser = iridium::parsing::implementation::CJSONParser::create();

    if (jsonParser) {
        // Use the parser...
        std::cout << "JSON Parser created successfully." << std::endl;
    } else {
        std::cerr << "Failed to create JSON parser." << std::endl;
    }
    return 0;
}
@endcode

@subsection subsec_parsing_example Parsing Example (JSON)

Here's how you can parse a JSON string into an `INode` tree:

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
            // You can now inspect the rootNode tree structure.
            // For example, to get the 'name' attribute:
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

@subsection subsec_serializing_example Serialization Example (JSON)

To serialize an `INode` tree (for example, one you constructed programmatically or obtained from parsing) back into a JSON string:

@code{.cpp}
#include "iridium/parsing/implementation/parser_json.h"
#include "iridium/parsing/parser.h"
#include "iridium/parsing/node.h"
#include "iridium/parsing/implementation/node.h" // For CNode
#include <string>
#include <iostream>

int main() {
    iridium::parsing::IParser::TSharedPtr jsonParser = iridium::parsing::implementation::CJSONParser::create();
    if (!jsonParser) {
        std::cerr << "Failed to create JSON parser." << std::endl;
        return 1;
    }

    // Create a simple node structure programmatically
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

@subsection subsec_serialization_macros Typed Serialization with NodeView

For more complex C++ data structures, the Iridium library offers a powerful serialization/deserialization framework based on `iridium::parsing::serialization::NodeView` and a set_of macros (like `DEFINE_ROOT_NODE_BEGIN`, `DEFINE_ATTRIBUTE`, `DEFINE_NODE_LIST_BEGIN`, etc.). These tools allow you to define C++ structs and classes that can be easily converted to and from an `INode` tree, and subsequently to/from string formats like JSON.

This system provides typed access to the node tree and automates much of the boilerplate code for serialization and deserialization. For detailed usage and examples, please refer to the header file `iridium/parsing/serialization/node.h`.

Here's a more concrete example of how to use these macros:

@code{.cpp}
#include "iridium/parsing/serialization/node.h" // For serialization macros
#include "iridium/parsing/implementation/parser_json.h" // For CJSONParser
#include "iridium/parsing/implementation/node.h" // For CNode (used by macros)
#include "iridium/convertion/convert.h" // For convert (used by macros)
#include <iostream>
#include <string>
#include <list> // Used by NodeViewList

// Required for the macros to compile if string conversions are needed.
// Normally, you'd have these specializations for your custom types if they aren't basic.

// Define a simple data item structure
DEFINE_ROOT_NODE_BEGIN(MyDataItem) // Root node for direct use or nesting, uses '-' delimiter by default
    DEFINE_ATTRIBUTE(std::string, Name, "DefaultName")
    DEFINE_ATTRIBUTE(int, Value, 0)
DEFINE_ROOT_NODE_END()

// Define a more complex settings structure
DEFINE_ROOT_NODE_BEGIN(MySettings) // Root node, uses '-' delimiter
    DEFINE_ATTRIBUTE(std::string, ConfigName, "DefaultConfig")
    DEFINE_ATTRIBUTE(bool, IsEnabled, true)

    // Define a nested node using a previously defined root node structure
    DEFINE_NODE_EXTERNAL(MyDataItem) // Member name will be 'MyDataItem', node name 'my-data-item'

    // Define a list of MyDataItem
    // The list member will be named 'ItemList', and each item node will be named 'item'
    DEFINE_NODE_LIST_BEGIN(Item) // This defines TItem and TItemList
        DEFINE_ATTRIBUTE(std::string, ItemName, "DefaultItem")
        DEFINE_ATTRIBUTE(int, ItemId, 0)
    DEFINE_NODE_LIST_END(Item) // Member name 'Item' of type TItemList
DEFINE_ROOT_NODE_END()

int main() {
    // Create an instance of the JSON parser
    auto jsonParser = iridium::parsing::implementation::CJSONParser::create();
    if (!jsonParser) {
        std::cerr << "Failed to create JSON parser." << std::endl;
        return 1;
    }

    // --- Serialization Example ---
    MySettings settingsToSerialize;
    settingsToSerialize.ConfigName = "UserSettings";
    settingsToSerialize.IsEnabled = true;

    settingsToSerialize.MyDataItem.Name = "Special Item";
    settingsToSerialize.MyDataItem.Value = 123;

    // Add items to the list
    MySettings::TItem item1; // Note: TItem is nested in MySettings due to DEFINE_NODE_LIST_BEGIN
    item1.ItemName = "FirstListItem";
    item1.ItemId = 1;
    settingsToSerialize.Item.add(item1);

    MySettings::TItem item2;
    item2.ItemName = "SecondListItem";
    item2.ItemId = 2;
    settingsToSerialize.Item.add(item2);

    try {
        // Get the INode from our settings object and compose it to JSON
        std::string jsonOutput = jsonParser->compose(settingsToSerialize.getNode());
        std::cout << "Serialized JSON:" << std::endl << jsonOutput << std::endl << std::endl;

        // Expected output structure:
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

    // --- Deserialization Example ---
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
            MySettings deserializedSettings(rootNode); // Construct MySettings from the INode

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

This example demonstrates defining structures with attributes, nested nodes, and lists of nodes. It then shows how to populate an instance of such a structure, serialize it to JSON, and deserialize JSON back into the structure using the `IParser` interface in conjunction with the node views generated by the macros. The `getNode()` method on the macro-defined struct provides the `INode` needed for `compose`, and the struct's constructor can take an `INode` (from `parse`) to populate itself. Remember that the actual node names in the JSON (e.g., "config-name") are derived from the C++ member names by converting CamelCase and inserting a delimiter (default is '-').

@section sec_logging Logger Usage and Configuration

The Iridium library includes a flexible logging facility to help instrument your application and diagnose issues. It supports multiple output destinations (sinks), configurable log levels, and asynchronous logging.

@subsection subsec_logging_macros Basic Logging

The easiest way to log messages is by using the provided logging macros:
- `LOGT << "Trace message";` (Trace level, often includes file/line, typically for debug builds)
- `LOGD << "Debug message";` (Debug level, typically for debug builds)
- `LOGI << "Informational message";` (Info level)
- `LOGW << "Warning message";` (Warning level)
- `LOGE << "Error message";` (Error level)
- `LOGF << "Fatal message";` (Fatal level)

These macros create a temporary log stream object. You can stream data into them using the `<<` operator, similar to `std::cout`. When the statement ends, the log message is automatically formatted and sent to the configured log sinks.

@code{.cpp}
#include "iridium/logging/logger.h" // Main header for logging

int main() {
    // Basic configuration (default console output) is usually set up if no custom config is provided.
    // For more control, see configuration examples below.
    // iridium::logging::setConfig(iridium::logging::config::createDefault()); // Example of setting default

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

@subsection subsec_logger_configuration Logger Configuration

The logger is configured using the `iridium::logging::config::TLogger` structure, which is defined in `iridium/logging/config.h`. This structure can be populated programmatically or potentially loaded from a configuration file (if parsed into an `INode` tree).

Key fields in `iridium::logging::config::TLogger`:
- `IsGmtTime` (bool): If `true`, timestamps are in GMT/UTC; otherwise, local time is used. Defaults to `false`.
- `Level` (`iridium::logging::TEvent::TLevel`): The global default log level. Specific sinks can override this. Defaults to `TRACE`.
- `Sink` (a list of `TLogger::TSink` objects): Defines one or more output destinations for log messages.

Each `TLogger::TSink` object in the `Sink` list has the following key fields:
- `Type` (`iridium::logging::config::TLogger::TSink::TSinkType`): Specifies the sink type. Common values:
    - `CONSOLE`: Output to the standard console.
    - `FILE`: Output to a single file.
    - `FILE_DAILY`: Output to a file that rotates daily.
    - `FILE_TIMELY`: Output to a new file every time the logger starts.
- `IsAsync` (bool): If `true`, this sink processes log messages asynchronously in a separate thread. Defaults to `false`.
- `Uri` (std::string): For file-based sinks (`FILE`, `FILE_DAILY`, `FILE_TIMELY`), this is the path to the log file.
- `Level` (`iridium::logging::TEvent::TLevel`): A specific log level for this sink. If set to `UNKNOWN`, it inherits the global logger level. Otherwise, it overrides the global level for this particular sink.

@subsection subsec_logger_config_example Programmatic Configuration Example

Here's how to configure the logger programmatically:

@code{.cpp}
#include "iridium/logging/logger.h"
#include "iridium/logging/config.h" // For TLogger, TEvent::TLevel, etc.

int main() {
    // 1. Create a TLogger configuration object
    iridium::logging::config::TLogger loggerConfig;

    // Set global properties
    loggerConfig.IsGmtTime = false;
    loggerConfig.Level = iridium::logging::TEvent::TLevel::INFO; // Global level

    // 2. Configure a Console Sink
    iridium::logging::config::TLogger::TSink consoleSink;
    consoleSink.Type = iridium::logging::config::TLogger::TSink::TSinkType::CONSOLE;
    consoleSink.IsAsync = false;
    consoleSink.Level = iridium::logging::TEvent::TLevel::DEBUG; // Console will show DEBUG and above
    loggerConfig.Sink.add(consoleSink); // Add to the list of sinks

    // 3. Configure a File Sink
    iridium::logging::config::TLogger::TSink fileSink;
    fileSink.Type = iridium::logging::config::TLogger::TSink::TSinkType::FILE;
    fileSink.IsAsync = true; // Log to file asynchronously
    fileSink.Uri = "application.log";
    fileSink.Level = iridium::logging::TEvent::TLevel::WARNING; // File will only get WARNING and above
    loggerConfig.Sink.add(fileSink);

    // 4. Apply the configuration
    iridium::logging::setConfig(loggerConfig);

    // Now the logger is configured.
    LOGT << "This TRACE message will not appear on console or file.";
    LOGD << "This DEBUG message will appear on the console only.";
    LOGI << "This INFO message will appear on the console only.";
    LOGW << "This WARNING message will appear on console AND in application.log.";
    LOGE << "This ERROR message will also appear on console AND in application.log.";

    return 0;
}
@endcode

@subsection subsec_default_config Default Configuration

The library provides a helper function to get a simple default configuration:
`iridium::logging::config::TLogger defaultConfig = iridium::logging::config::createDefault();`

This default configuration typically sets up a synchronous console sink with the `TRACE` log level. You can modify this default configuration object before applying it with `iridium::logging::setConfig()`.

Remember that logger configuration should ideally be done early in your application's lifecycle, before any significant logging occurs.

@section sec_custom_type_conversion Integrating Custom Types with `iridium::convertion::convert`

The Iridium library provides a flexible way to integrate your custom data types with its systems, such as logging and the serialization macros. This is achieved by providing specializations for the `iridium::convertion::convert<TargetType, SourceType>` template function for your custom type.

The library offers a pair of macros to help declare and implement these specializations:
- **`DEFINE_CONVERT(TTo, TFrom)`**: This macro should be used in your header file (e.g., alongside your custom type definition). It declares to the compiler that a specific conversion specialization will be made available.
- **`IMPLEMENT_CONVERT(TTo, TFrom, TFunc)`**: This macro is used in a source file (.cpp) to provide the actual implementation for the conversion. `TFunc` must be a function, functor, or lambda that takes a `TFrom const&` argument and returns a `TTo`.

@subsection subsec_custom_convert_example_fn Example: Custom `Point` Type

Let's illustrate with a custom `Point` struct.

**1. Define the Type and Declare Conversions (e.g., in `Point.h`)**
@code{.cpp}
#pragma once // Example header guard

#include "iridium/convertion/convert.h" // For DEFINE_CONVERT
#include <string> // Required for std::string usage

// Custom struct
struct Point {
    int x;
    int y;

    // Optional: For direct comparison in examples or tests
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Declare that we will provide specializations for converting
// Point to std::string and std::string to Point.
DEFINE_CONVERT(std::string, Point);
DEFINE_CONVERT(Point, std::string);
@endcode

**2. Implement the Conversions (e.g., in `Point.cpp`)**
@code{.cpp}
#include "Point.h" // Assuming Point struct and DEFINE_CONVERT declarations are here

#include "iridium/convertion/convert.h" // For IMPLEMENT_CONVERT
#include <string>
#include <stdexcept> // For std::invalid_argument, std::out_of_range

// Helper function to convert Point to std::string
static std::string localPointToString(const Point& p) {
    return "(" + std::to_string(p.x) + "," + std::to_string(p.y) + ")";
}

// Helper function to convert std::string to Point
static Point localStringToPoint(const std::string& s) {
    if (s.length() < 3 || s.front() != '(' || s.back() != ')') {
        throw std::invalid_argument("Invalid Point string format: Missing parentheses. Input: " + s);
    }
    std::string content = s.substr(1, s.length() - 2); // Remove parentheses
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

// Implement the declared conversion specializations using the helper functions
IMPLEMENT_CONVERT(std::string, Point, localPointToString);
IMPLEMENT_CONVERT(Point, std::string, localStringToPoint);
@endcode

**3. Usage Example (e.g., in `main.cpp` or tests)**
@code{.cpp}
#include "Point.h" // Your custom type's header
#include "iridium/convertion/convert.h" // For iridium::convertion::convert
#include "iridium/logging/logger.h"   // For LOGI (example usage)
// For serialization macros example (conceptual)
// #include "iridium/parsing/serialization/node.h"
// #include "iridium/parsing/implementation/parser_json.h"
#include <iostream> // For std::cout

int main_custom_convert_demo() { // Renamed to avoid collision if mainpage.md is compiled
    Point p1 = {7, 14};

    // Logging Example:
    // The iridium::logging::LogStream typically uses iridium::convertion::convert<std::string>(value)
    // for types that don't have a direct std::ostream& operator<< overload.
    // So, after defining the conversion, this should work:
    // LOGI << "My custom point: " << p1;
    // For this example, we'll demonstrate the conversion explicitly:
    std::string p1_as_string = iridium::convertion::convert<std::string>(p1);
    std::cout << "Point p1 converted to string: " << p1_as_string << std::endl;


    // Deserialization Example (e.g., from a configuration string):
    std::string input_string = "(100,-200)";
    try {
        Point p2 = iridium::convertion::convert<Point>(input_string);
        std::cout << "String '" << input_string << "' converted to Point: (" << p2.x << "," << p2.y << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error converting string to Point: " << e.what() << std::endl;
    }

    // How this applies to serialization macros:
    // If you define a serializable struct like this:
    //
    // DEFINE_ROOT_NODE_BEGIN(MyStructure)
    //   DEFINE_ATTRIBUTE(Point, Position) // Uses Point
    // DEFINE_ROOT_NODE_END()
    //
    // MyStructure data;
    // data.Position = {5,5}; // Set the Point
    //
    // // When serializing:
    // // auto jsonParser = iridium::parsing::implementation::CJSONParser::create();
    // // std::string json_output = jsonParser->compose(data.getNode());
    // // ...the 'Position' node's value would be "(5,5)" due to Convert<std::string, Point>.
    //
    // // When deserializing:
    // // std::string json_input = R"({ "position": "(10,20)" })";
    // // MyStructure loaded_data(jsonParser->parse(json_input));
    // // ...the string "(10,20)" would be converted to a Point object for 'Position'
    // //    due to Convert<Point, std::string>.

    return 0;
}
@endcode

@subsection subsec_custom_convert_fn_impact Impact of Custom `convert` Specializations

**For Logging:**
Once you provide a specialization `DEFINE_CONVERT(std::string, YourCustomType)` and its implementation, your custom objects can often be directly used in logging statements (e.g., `LOGI << yourCustomObject;`). The logging framework will typically call `iridium::convertion::convert<std::string>(yourCustomObject)` to get a string representation if no direct `operator<<(std::ostream&, const YourCustomType&)` is found or preferred.

**For Serialization Macros:**
When using serialization macros like `DEFINE_ATTRIBUTE(YourCustomType, myField)`:
- The specialization for `Convert<std::string, YourCustomType>` (implemented via `IMPLEMENT_CONVERT`) is used to convert the `myField` member into its string representation when serializing the parent structure. This string is then stored as the value of the corresponding node (e.g., a JSON string attribute or an XML text node).
- The specialization for `Convert<YourCustomType, std::string>` (implemented via `IMPLEMENT_CONVERT`) is used during deserialization. If the parser reads a string value from the input data (e.g., from a JSON attribute) that needs to be converted to `YourCustomType` for `myField`, this specialization will be invoked.

By providing these `convert` function specializations, you enable your custom data types to integrate smoothly with various parts of the Iridium library that rely on type conversion, particularly for string-based representations. Remember to declare the specializations with `DEFINE_CONVERT` in a header and implement them with `IMPLEMENT_CONVERT` and your conversion logic in a source file.

@section sec_define_enum Handling Enumerations with DEFINE_ENUM

The Iridium library often provides a utility macro, `DEFINE_ENUM` (typically expected in a header like `iridium/enum.h` or a general utilities header), to simplify the definition of C++ `enum class` types and automatically integrate them with the `iridium::convertion::convert` system. This integration is crucial for easily logging enum values as human-readable strings and for serializing/deserializing them in configuration files or data interchange formats.

**Key Functionality of `DEFINE_ENUM`:**

When you use `DEFINE_ENUM`, it generally performs the following actions:
1.  **Defines the `enum class`**: It creates the actual C++ `enum class` with the specified name and enumerators.
2.  **Generates String Conversions**: Most importantly, it automatically generates the necessary specializations for `iridium::convertion::convert` to allow:
    - Converting an enum value to its string representation (e.g., `MyEnum::ValueOne` becomes `"ValueOne"`).
    - Converting a string representation back to an enum value (e.g., `"ValueOne"` becomes `MyEnum::ValueOne`).

**Typical Syntax:**

While the exact syntax can vary slightly based on the specific macro implementation, a common pattern is:

`DEFINE_ENUM(EnumTypeName, Enumerator1, Enumerator2, ..., [OptionalDefaultOrUnknownEnumerator])`

- `EnumTypeName`: The desired name for your `enum class`.
- `Enumerator1, Enumerator2, ...`: The list of enumerator names.
- `[OptionalDefaultOrUnknownEnumerator]`: Often, the last enumerator can be designated (e.g., named `Unknown` or `Default`) to serve as a fallback value if a string cannot be successfully parsed into one of the other defined enumerators. If not specified, attempting to convert an invalid string to the enum type might result in the first enumerator being returned, or potentially an error, depending on the macro's design.

@subsection subsec_define_enum_example Example Usage

Let's illustrate with a couple of examples. (Ensure you include the header that provides `DEFINE_ENUM`, e.g., `#include "iridium/enum.h"`)

@code{.cpp}
// Note: The DEFINE_ENUM macro is provided by the Iridium library.
// You would typically include a header like <iridium/enum.h> to use it.
// mainpage_example_define_enum.cpp

// It's assumed that "iridium/enum.h" or a similar header defining DEFINE_ENUM
// and "iridium/convertion/convert.h" are available.
// For this example, we'll mock the macro's expected behavior for demonstration if needed,
// but ideally, it comes from the library.


// Example 1: Simple Color Enumeration
DEFINE_ENUM(MyColor,
    Red,    // Mapped to "Red"
    Green,  // Mapped to "Green"
    Blue    // Mapped to "Blue"
) // MyColor will have MyColor::Red, MyColor::Green, MyColor::Blue

// Example 2: Operation Status with an explicit Unknown/Default value
DEFINE_ENUM(OperationStatus,
    Success,
    Pending,
    Failed,
    Unknown // This can serve as a default if parsing a string fails
)

#include <iostream>
#include "iridium/convertion/convert.h" // For iridium::convertion::convert
// #include "iridium/logging/logger.h" // For LOGI (conceptual)

void demo_define_enum_usage() {
    MyColor favoriteColor = MyColor::Green;

    // Convert enum to string (e.g., for logging or serialization)
    // The DEFINE_ENUM macro is expected to have created the necessary
    // iridium::convertion::convert<std::string, MyColor> specialization.
    std::string colorStr = iridium::convertion::convert<std::string>(favoriteColor);
    std::cout << "Favorite color: " << colorStr << std::endl; // Expected: "Green"

    // Conceptual logging:
    // LOGI << "Current status: " << OperationStatus::Pending; // Would output "Pending"

    // Convert string to enum (e.g., for deserialization from a config file)
    std::string inputColorStr = "Blue";
    MyColor parsedColor = iridium::convertion::convert<MyColor>(inputColorStr);
    std::cout << "Parsed color from string '" << inputColorStr << "': "
              << iridium::convertion::convert<std::string>(parsedColor) << std::endl; // Expected: "Blue"

    // Example of parsing an invalid string for MyColor
    std::string invalidColorStr = "Purple";
    MyColor defaultOrFirstColor = iridium::convertion::convert<MyColor>(invalidColorStr);
    // Behavior for invalid string depends on DEFINE_ENUM implementation.
    // It might default to the first enumerator (Red) or a specified 'Unknown' value if the macro supports that.
    // Our conceptual placeholder defaults to 'Unknown' if available, or first if not.
    std::cout << "Parsed invalid color string '" << invalidColorStr << "': "
              << iridium::convertion::convert<std::string>(defaultOrFirstColor) << std::endl;

    // Example with OperationStatus which has an 'Unknown' default
    std::string invalidStatusStr = "NonExistentStatus";
    OperationStatus status = iridium::convertion::convert<OperationStatus>(invalidStatusStr);
    std::cout << "Parsed invalid status string '" << invalidStatusStr << "': "
              << iridium::convertion::convert<std::string>(status) << std::endl; // Expected: "Unknown"

    OperationStatus goodStatus = iridium::convertion::convert<OperationStatus>("Success");
    std::cout << "Parsed valid status string 'Success': "
              << iridium::convertion::convert<std::string>(goodStatus) << std::endl; // Expected: "Success"
}
@endcode

@subsection subsec_define_enum_impact Impact of `DEFINE_ENUM`

**Simplified Logging:**
Because `DEFINE_ENUM` typically handles the creation of `iridium::convertion::convert<std::string, YourEnumType>`, enum values can be directly used in logging statements, and they will be automatically converted to their human-readable string names:
@code{.cpp}
// MyColor color = MyColor::Blue;
// LOGI << "Current color selection: " << color; // Would log "Current color selection: Blue"
@endcode

**Seamless Serialization:**
Enums defined with `DEFINE_ENUM` integrate smoothly with Iridium's serialization macros. If you use such an enum as a type for an attribute:
@code{.cpp}
// // Assuming MyColor is defined using DEFINE_ENUM
// DEFINE_ROOT_NODE_BEGIN(MyWidgetConfig)
//   DEFINE_ATTRIBUTE(MyColor, WidgetColor)
// DEFINE_ROOT_NODE_END()
//
// MyWidgetConfig config;
// config.WidgetColor = MyColor::Red;
//
// // Serializing 'config' would store "Red" for the 'widget-color' attribute.
// // Deserializing a structure where 'widget-color' is "Green" would set config.WidgetColor to MyColor::Green.
@endcode
The serialization system uses the auto-generated `convert` specializations to handle the transformation between the enum values and their string representations in the serialized data (e.g., JSON, XML).

Using `DEFINE_ENUM` thus reduces boilerplate, ensures consistency, and makes working with enumerations in Iridium much more convenient. Always refer to the specific `iridium/enum.h` or equivalent header in your version of the library for the exact capabilities and syntax of the `DEFINE_ENUM` macro.

@section sec_threading Multithreading Primitives

The Iridium library provides several primitives to aid in multithreaded application development. These include tools for direct thread management, data synchronization, and task processing with worker threads and pools.

Note: Some concrete classes like `CThread`, `CWorker`, and `CWorkerPool` reside in the `iridium::threading::implementation` namespace. While they might have `::create()` factory methods suggesting usability, this namespacing typically indicates they are foundational blocks, possibly intended to be wrapped by higher-level abstractions in some cases. However, their direct usage is documented here as per common need.

@subsection subsec_cthread Using CThread for Direct Thread Management

`iridium::threading::implementation::CThread` allows you to create and manage a dedicated thread. It executes a task defined by an object implementing the `iridium::threading::IRunnable` interface.

**`IRunnable` Interface:**
To define the work for a `CThread`, you create a class that inherits from `iridium::threading::IRunnable`. This interface requires you to implement:
- `void initialize()`: Called once after the thread starts, for setup.
- `void finalize()`: Called once before the thread joins, for cleanup.
- `void run(std::atomic<bool>& is_running)`: This is the main function executed by the thread. It should contain your task's logic and periodically check the `is_running` flag. When `is_running` becomes `false`, the `run` method should return to allow the thread to exit gracefully.

@code{.cpp}
// mainpage_example_cthread.cpp
#include "iridium/threading/implementation/thread.h" // For CThread
#include "iridium/threading/runnable.h"       // For IRunnable
#include "iridium/threading/thread.h"         // For iridium::threading::sleep, IThread (for getNameStatic)
#include <iostream>
#include <atomic>
#include <chrono> // For std::chrono::milliseconds
#include <memory> // For std::make_shared

// 1. Implement IRunnable
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
            // Simulate work by sleeping
            for(int i=0; i < 5 && is_running; ++i) { // Check is_running more frequently during "work"
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
    // 2. Create an instance of your IRunnable
    auto task = std::make_shared<MyPeriodicTask>();

    // 3. Create and manage the CThread
    // CThread::create takes (name, IRunnable::TSharedPtr)
    auto thread_obj = iridium::threading::implementation::CThread::create("MyWorker1", task);

    try {
        std::cout << "Main: Initializing thread..." << std::endl;
        thread_obj->initialize(); // Starts the thread and waits for it to be running

        std::cout << "Main: Sleeping for 3 seconds while thread runs..." << std::endl;
        iridium::threading::sleep(3000);

        std::cout << "Main: Finalizing thread..." << std::endl;
        thread_obj->finalize(); // Signals thread to stop and waits for it to join

        std::cout << "Main: Thread finished." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Main: Thread operation failed: " << e.what() << std::endl;
    }
}
@endcode
In this example, `MyPeriodicTask::run` will execute in a new thread managed by `CThread`. `CThread::initialize()` starts it, and `CThread::finalize()` signals `is_running` to `false` and joins the thread.

@subsection subsec_synchronized Using Synchronized for Data Protection

`iridium::threading::Synchronized<TMutex, bool is_tracable = false>` is a template class designed to protect shared data from concurrent access. It typically serves as a base class for classes that contain data needing protection. It uses a mutex (e.g., `std::mutex`) and a condition variable.

**Key Mechanism: `LOCK_SCOPE()`**
The `LOCK_SCOPE()` macro is the primary way to interact with `Synchronized`. When placed inside a method of a class that inherits from `Synchronized`, it creates a scoped lock (an instance of `Synchronized::Locker`). The mutex is acquired when `LOCK_SCOPE()` is encountered and automatically released when the scope is exited (due to RAII).

@code{.cpp}
// mainpage_example_synchronized.cpp
#include "iridium/threading/synchronized.h"
#include <vector>
#include <string>
#include <mutex>    // For std::mutex
#include <iostream>
#include <chrono>   // For std::chrono
// #include "iridium/threading/implementation/thread.h" // For more complex demo with actual threads

// Class whose methods will be synchronized
class SafeDataContainer : public iridium::threading::Synchronized<std::mutex> {
private:
    std::vector<std::string> items_;
    int access_count_ = 0;

public:
    void addItem(const std::string& item) {
        LOCK_SCOPE(); // Lock acquired for this scope
        items_.push_back(item);
        access_count_++;
        // std::cout << "addItem: Added '" << item << "'. Lock released upon exiting scope." << std::endl;
        // Mutex is automatically released here by _____locked_scope_____ destructor
    }

    void addItems(const std::vector<std::string>& items_to_add) {
        LOCK_SCOPE();
        for(const auto& item_to_add : items_to_add) { // Fixed variable name
            items_.push_back(item_to_add);
        }
        access_count_++;
        // std::cout << "addItems: Added multiple items. Lock released upon exiting scope." << std::endl;
    }

    std::vector<std::string> getItems() {
        LOCK_SCOPE();
        // Return by value ensures the copy is made while the lock is held.
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
        // The Synchronized::Locker's destructor calls notify_one automatically.
        // To explicitly notify (e.g. all), you would need access to the condition variable itself.
        // For this pattern, you'd typically add data (which acquires and releases a lock),
        // and the lock release notifies one waiter.
        // If you need notify_all, it's not directly exposed via LOCK_SCOPE alone for this simple pattern.
        // The Synchronized class has m_cv.notify_all() in its interrupt() method.
        // This example relies on the implicit notify_one on Locker destruction.
        LOCK_SCOPE(); // Acquire lock to ensure data consistency before potential notification
        // (no data change here, but typically you would modify data before notify)
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

    // Conceptual: A thread might wait for a condition
    // std::thread waiter([&](){
    //    std::cout << "Waiter thread: waiting for 3 items." << std::endl;
    //    container.waitUntilItemCountReaches(3, std::chrono::seconds(2));
    //    std::cout << "Waiter thread: finished waiting. Items: " << container.getItems().size() << std::endl;
    // });
    // std::thread adder([&](){
    //    iridium::threading::sleep(500);
    //    std::cout << "Adder thread: adding 'Cherry'." << std::endl;
    //    container.addItem("Cherry"); // This will notify one waiter via Locker destructor
    // });
    // adder.join();
    // waiter.join();
}
@endcode
The `Synchronized` class makes methods of `SafeDataContainer` thread-safe. The `Locker`'s destructor also calls `notify_one()` on an internal condition variable, which can be used with `Locker::wait()` methods.

@subsection subsec_cworker Using CWorker for Task Processing

`iridium::threading::implementation::CWorker<TInputItem, TOutputItem>` provides a single worker thread that processes items from an input queue and can place results in an output queue.

**`IWorker::IHandler` Interface:**
You define a handler class inheriting from `iridium::threading::IWorker<TInputItem, TOutputItem>::IHandler`. Its core method is:
- `TOutputItems handle(const TInputItems& items)`: Receives a list of items from the input queue and should return a list of output items.

@code{.cpp}
// mainpage_example_cworker.cpp
#include "iridium/threading/implementation/worker.h" // For CWorker
#include "iridium/threading/worker.h" // For IWorker::IHandler
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // For std::transform
#include <cctype>    // For toupper
#include <memory>    // For std::make_shared

// 1. Define the Handler
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
                           [](unsigned char c){ return static_cast<char>(std::toupper(c)); }); // Ensure char cast
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

        // The CWorkerRunnable processes one item at a time from the input queue,
        // calls handler (which receives a list of 1 item),
        // and handler returns a list of output items.
        // So, each pop() call will retrieve the results from one handle() call.

        // Results for "alpha"
        processed_batch = worker->pop(true); // true means wait
        for (const auto& item : processed_batch) {
            std::cout << "Main: Popped CWorker result: " << item << std::endl;
        }

        // Results for "beta"
        processed_batch = worker->pop(true);
        for (const auto& item : processed_batch) {
            std::cout << "Main: Popped CWorker result: " << item << std::endl;
        }

        // Results for "gamma"
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

@subsection subsec_cworkerpool Using CWorkerPool for Parallel Task Processing

`iridium::threading::implementation::CWorkerPool<TInputItem, TOutputItem>` manages a pool of multiple worker threads for parallel processing of items from a shared input queue to a shared output queue.

**Key Aspects:**
- Constructor takes a list of `IHandler` shared pointers (`THandlers`).
- Items pushed are distributed among worker threads.

@code{.cpp}
// mainpage_example_cworkerpool.cpp
#include "iridium/threading/implementation/worker_pool.h" // For CWorkerPool
#include "iridium/threading/worker.h" // For IWorker::IHandler
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <memory>    // For std::make_shared
#include <algorithm> // For std::transform
#include <cctype>    // For toupper

#ifndef TOUPPERSTRINGHANDLER_DEFINED_FOR_POOL_DEMO // Guard if included multiple times
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
        pool->push(all_tasks); // Push a batch. These will be added to the shared input queue.

        // Collect results. Each worker thread processes one item from the input queue at a time,
        // its handler produces a list of output items, which are added to the pool's output queue.
        // So, we expect to pop 'all_tasks.size()' lists of results.
        for (size_t i = 0; i < all_tasks.size(); ++i) {
            auto results_batch = pool->pop(true); // Wait for a batch of results from one handler call
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
The `CWorkerPool` is effective for parallelizing independent tasks. The order of results from `pop` may not strictly match the `push` order.

These primitives provide foundational tools for building concurrent and parallel applications with Iridium.
