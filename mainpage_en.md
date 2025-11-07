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

@section sec_testing Testing with Iridium

The Iridium library provides a built-in framework for writing and running unit tests, as well as for creating mock objects to isolate the code under test. This framework helps ensure the quality and reliability of code developed using Iridium.

@subsection subsec_testing_core_concepts Core Concepts

The foundation of the Iridium testing system consists of two key interfaces/classes:

-   `iridium::testing::ITest` (defined in `iridium/testing/test.h`): This is the base interface for any runnable test case. Each class representing a test must implement the `virtual void run() = 0;` method. This method contains the logic of the test itself, including calls to the code under test and assertion checks.

-   `iridium::testing::UnitTest` (defined in `iridium/testing/unit_test.h`): This class serves as the base class for your tests (usually implicitly via the `TEST` macro). It provides a set of methods for performing various assertions, such as checking equality, comparisons, truthiness of conditions, or the throwing of expected exceptions. If an assertion fails, these methods generate a special exception that signals the test failure.

@subsection subsec_testing_writing_tests Writing Tests

The primary tool for creating test cases in Iridium is the `TEST` macro.

@subsubsection subsubsec_testing_test_macro The TEST Macro

The `TEST(TestCaseName)` macro (defined in `iridium/testing/tester.h`) is used to define a test case. `TestCaseName` must be a unique identifier for your test. This macro automatically creates a class that inherits from `iridium::testing::ITest` and `iridium::testing::UnitTest`, and registers it with the testing system. You only need to provide the body for the `run()` method, which will contain the test logic.

Example:
@code{.cpp}
#include "iridium/testing/tester.h" // For TEST and ASSERT macro
#include <string>                   // For std::string in example
#include <stdexcept>                // For std::runtime_error in example
#include "iridium/convertion/convert.h" // For iridium::convertion::convert (if custom types are needed in ASSERT)

// Example custom type to demonstrate output in ASSERT
struct MyCustomType {
    int id;
    std::string value;

    // For comparison in tests
    bool operator==(const MyCustomType& other) const {
        return id == other.id && value == other.value;
    }
};

// Specialization of convert for MyCustomType, so ASSERT can print it
template<>
std::string iridium::convertion::convert(MyCustomType const &obj) {
    return "MyCustomType(id=" + iridium::convertion::convert<std::string>(obj.id) + ", value=\"" + obj.value + "\")";
}

// Example class to be tested (replace with your own)
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
    ASSERT(true); // Check for truth
    bool my_flag = false;
    ASSERT(!my_flag); // Check for falsehood via negation
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

@subsubsection subsubsec_testing_assertions The ASSERT Macro

The `iridium::testing::UnitTest` class (from which tests created by the `TEST` macro implicitly inherit) provides various methods for checking conditions. The primary way to use them in tests is the `ASSERT(...)` macro (defined in `iridium/testing/tester.h`). If an assertion fails, an exception is thrown, and the test is marked as failed.

The `ASSERT` macro is variadic (accepts a variable number of arguments) and is used as follows:

1.  **Checking a boolean condition: `ASSERT(condition)`**
    -   Used to check for truth (`ASSERT(true_condition);`) or falsehood (`ASSERT(!false_condition);`).
    -   Technically, this is a call to `ASSERT_1`, which passes `condition` to the `UnitTest::assert_()` method.
    -   Example: `ASSERT(x > 5);`

2.  **Checking for comparison: `ASSERT(value1, comparison_symbol, value2)`**
    -   Used for various types of comparisons. `comparison_symbol` is the name of a comparison method from the `UnitTest` class. `value1` represents the expected value, and `value2` the actual (received) value.
    -   When performing a comparison (`ASSERT(value1, comparison_symbol, value2)`), the expected value (`value1`) is converted to the type of the actual value (`value2`). For instance, in the template methods of `iridium::testing::UnitTest` (see `iridium/testing/unit_test.h`), a construct like `TRight left_(left);` or `static_cast<TRight>(left)` is used, where `left` is `value1` (expected), and `TRight` is the type of `value2` (actual). Thus, the actual comparison (`left_ == right`, `left_ < right`, etc.) occurs between values of the same type—the type `TRight`. For converting values to strings for display in error messages, `iridium::convertion::convert` is used.
    -   Technically, this is a call to `ASSERT_3`, which calls `UnitTest::comparison_symbol(value1, value2, "value1 symbol value2", "file:line")`.
    -   Available `comparison_symbol`s:
        -   `equal`: Checks that `value1 == value2`.
        -   `less`: Checks that `value1 < value2`.
        -   `lessEqual`: Checks that `value1 <= value2`.
        -   `greater`: Checks that `value1 > value2`.
        -   `greaterEqual`: Checks that `value1 >= value2`.
    -   Examples:
        @code{.cpp}
        ASSERT(sum, equal, 10);
        ASSERT(count, less, max_count);
        ASSERT(actual_value, greaterEqual, expected_minimum);
        @endcode
    -   Note: Although you can also write `ASSERT(value1 == value2);` (using `ASSERT_1`), using `ASSERT(value1, equal, value2);` is preferable for comparisons, as it allows the framework to potentially output the values of `value1` and `value2` more informatively upon failure.

3.  **Checking for exception throwing: `ASSERT(expression, ExpectedExceptionType)`**
    -   Technically, this is a call to `ASSERT_2`, which passes a lambda function with `expression` and `ExpectedExceptionType` to the `UnitTest::assert_<TFunction, TException>()` method.
    -   Checks that executing `expression` results in an exception of type `ExpectedExceptionType` (or its descendant) being thrown.
    -   Example: `ASSERT(myObject.methodThatThrows(), std::runtime_error);`

4.  **Unconditional test failure:**
    -   There is no direct `FAIL(message)` macro. To unconditionally fail a test, use `ASSERT(false);`. You can add a comment to explain the reason.
        @code{.cpp}
        ASSERT(false); // Test failed because condition X was not met
        @endcode

**Outputting values on errors and `iridium::convertion::convert`:**
A very important feature of the `ASSERT` macro (especially for comparisons) is how it displays values when an assertion fails. To output `value1` and `value2` (or the value from `condition` for `ASSERT_1`) in a readable format, the testing system uses `iridium::convertion::convert<std::string>(your_value)`.
If you use custom types in `ASSERT`, ensure that a specialization of `iridium::convertion::convert` to `std::string` exists for them. Otherwise, you might see uninformative output (e.g., just the type name or address). For more details on creating `convert` specializations, see section @ref sec_custom_type_conversion.

The `ASSERT` macro automatically includes the file name and line number in the error information, which helps to quickly locate the point of test failure.

@subsection subsec_testing_running_tests Running Tests

After writing tests, they need to be compiled and run. The Iridium framework simplifies this process.

@subsubsection subsubsec_testing_main The IMPLEMENT_TEST_MAIN Macro

To create an executable file that will run all defined tests, the `IMPLEMENT_TEST_MAIN()` macro (defined in `iridium/testing/tester.h`) is used. This macro generates a standard `main()` function that initializes and runs the testing framework.

Typically, you create a separate `.cpp` file (e.g., `tests_main.cpp`) that includes all your test files (or headers, if tests are defined in them) and then calls `IMPLEMENT_TEST_MAIN()`.

Example (`tests_main.cpp`):
@code{.cpp}
#include "iridium/testing/tester.h" // For IMPLEMENT_TEST_MAIN

// Include your test files here or files where TEST(...) tests are defined
// For example:
// #include "my_class_tests.cpp" 
// #include "another_module_tests.cpp"

IMPLEMENT_TEST_MAIN()
@endcode
When you compile this file along with your tests and the Iridium library, you will get an executable file that, when run, will execute all detected tests.

@subsubsection subsubsec_testing_tester Role of the Tester Class

The `iridium::testing::Tester` class (defined in `iridium/testing/tester.h`) is the central component of the testing system. It is a singleton that:
-   Registers all test cases defined using the `TEST` macro.
-   Manages the execution of tests. The `Tester::run(argc, argv, main_cpp_path)` method is called from the generated `main()` function.
-   Allows filtering of tests based on command-line arguments (this functionality may not be fully detailed here, but `Tester` provides for it).
-   Collects test results.

@subsubsection subsubsec_testing_itestrunner The ITestRunner Interface (for advanced scenarios)

For more flexible control over the test execution process, there is the `iridium::testing::ITestRunner` interface (defined in `iridium/testing/test_runner.h`). This interface abstracts the way tests are run. The Iridium library provides at least two implementations of it:
-   `iridium::testing::implementation::TestRunnerRaw`: Runs tests in the same process.
-   `iridium::testing::implementation::TestRunnerFork`: Can run tests in separate processes (forks), which provides better isolation (typically on POSIX systems).

Although direct interaction with `ITestRunner` is usually not required for writing and running tests, knowing about its existence is useful for understanding the framework's architecture and for possible extensions or customization of the testing process. The `Tester` class uses an `ITestRunner` implementation to actually execute the tests.

@subsubsection subsubsec_testing_cli Command-Line Arguments

The test executable created with `IMPLEMENT_TEST_MAIN()` supports the following command-line arguments to control the testing process. Assume your test application is named `your_test_app`.

1.  **`help`**: Displays help information on usage.
    @code{.sh}
    ./your_test_app help
    @endcode
    Example output:
    @code{.txt}
    main thread: 281473424756000
    2025-06-06 10:30:56.839 I 281473424756000 
    usage:
    ./your_test_app help
    ./your_test_app list
    ./your_test_app run [ --mode=raw|serial|parallel ] [ --print-result=json ] [ --timeout=seconds ] [ include_path ] [ exclude_path ] ... [ exclude_path ]
    example:
    ./your_test_app run / 
    @endcode
    (Note: The actual thread number and timestamp in your output will differ.)

2.  **`list`**: Displays a hierarchical list (tree) of all test cases discovered by the framework. Tests are grouped by file path and then by the test name defined in the `TEST()` macro.
    @code{.sh}
    ./your_test_app list
    @endcode
    This is useful for viewing the structure of tests and their full paths, which can be used as `include_path` or `exclude_path` for the `run` command. Example output:
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
      # ... (and so on for other modules and tests)
    @endcode
    (Note: The actual thread number, timestamp, and full list of tests in your output will differ.)

3.  **`run`**: Runs the tests. This is the default command if no other is specified.
    @code{.sh}
    ./your_test_app run [options] [include_path] [exclude_path_1] [exclude_path_2] ...
    @endcode
    Standard output when running tests (without `--print-result=json`) includes logging for each test being run and its result:
    @code{.txt}
    main thread: 281473243745568
    2025-06-06 10:30:39.744 I 281473243745568 RUN  /testing/example.cpp/bool_
    2025-06-06 10:30:39.745 I 281473243745568 OK   /testing/example.cpp/bool_
    2025-06-06 10:30:39.745 I 281473243745568 RUN  /testing/example.cpp/comparing_equal
    2025-06-06 10:30:39.745 I 281473243745568 OK   /testing/example.cpp/comparing_equal
    # ... (and so on for other tests)
    # ... (in case of failure, FAILED will appear instead of OK, along with an error message)
    2025-06-06 10:30:39.747 I 281473243745568 
    passed: 8
    failed: 0
    total:  51 
    @endcode
    (Note: Actual thread numbers, timestamps, and results in your output will differ.)

    Available options for the `run` command:
    -   `--mode=MODE`: Defines the test execution mode. Possible values for `MODE`:
        -   `raw` (default): Tests are run in the same process.
        -   `serial`: Tests are run sequentially, each in a separate process.
        -   `parallel`: Tests are run in parallel in separate processes.
    -   `--print-result=FORMAT`: Controls the output format of results.
        -   If not specified, the standard text format is used (see example above).
        -   `json`: Results are output in JSON format.
    -   `--timeout=SECONDS`: Sets the maximum timeout for tests in seconds (default is 60).

    Parameters for the `run` command:
    -   `include_path` (optional): If specified, only tests whose path (as in the `list` output) starts with `include_path` are run. Defaults to `/` (all tests).
    -   `exclude_path_...` (optional): One or more paths to exclude. Tests whose path starts with one of these paths will not be run.

Examples of using the `run` command:

-   Run all tests:
    @code{.sh}
    ./your_test_app run
    @endcode
    (or just `./your_test_app`)

-   Run all tests in parallel mode with a timeout of 120 seconds:
    @code{.sh}
    ./your_test_app run --mode=parallel --timeout=120
    @endcode

-   Run only tests from the `testing` module and `example.cpp` file:
    @code{.sh}
    ./your_test_app run /testing/example.cpp/
    @endcode

-   Run all tests from the `parsing` module, but exclude those in `node.cpp` within `parsing`:
    @code{.sh}
    ./your_test_app run /parsing/ /parsing/node.cpp/
    @endcode

-   Run all tests and output the result in JSON format:
    @code{.sh}
    ./your_test_app run --print-result=json
    @endcode

@subsection subsec_testing_mocking Mocking Dependencies

Mocking is the process of creating substitute objects (mocks or mock objects) to simulate the behavior of real dependencies of the component under test. This allows isolating the code under test and making tests more predictable and stable. The Iridium testing framework provides tools for creating mocks (defined in `iridium/testing/mock.h`).

@subsubsection subsubsec_testing_defining_mocks Defining Mock Classes

To create a mock for an interface or class, the `DEFINE_MOCK_CLASS(InterfaceName)` macro is used. This macro creates a new class named `InterfaceNameMock` that inherits your `InterfaceName` and `iridium::testing::Mock<InterfaceName>`.

Inside this mock class, you declare which methods of the interface you want to mock using the `DEFINE_MOCK_METHOD` (for non-const methods) and `DEFINE_MOCK_METHOD_CONST` (for const methods) macros.

Example:
@code{.cpp}
#include "iridium/testing/mock.h"
#include <string>
#include <vector>

// Assume this is the interface we want to mock
class IMyDependency {
public:
    virtual ~IMyDependency() = default;
    virtual int getValue(int key) = 0;
    virtual std::string getName() const = 0;
    virtual void processData(const std::vector<int>& data) = 0;
    // Constructor with arguments to demonstrate DEFINE_MOCK_CONSTRUCTOR
    IMyDependency(const std::string& /* initial_config */) {} 
    IMyDependency() = default; // Add a default constructor if it's also needed
};

// Define the mock class for IMyDependency
DEFINE_MOCK_CLASS(IMyDependency) {
public:
    // If the base class IMyDependency has a constructor with arguments
    // and you want to call it from the mock:
    DEFINE_MOCK_CONSTRUCTOR(IMyDependency)
    
    // Mocking interface methods
    DEFINE_MOCK_METHOD(int, getValue, (int))                  // int getValue(int key)
    DEFINE_MOCK_METHOD_CONST(std::string, getName, ())        // std::string getName() const
    DEFINE_MOCK_METHOD(void, processData, (const std::vector<int> &)) // void processData(const std::vector<int>& data)
};
@endcode
The `DEFINE_MOCK_METHOD` macros take the return type, method name, and in parentheses, the types of the method's arguments (without variable names).

@subsubsection subsubsec_testing_mock_behavior Defining Mock Behavior

After creating an instance of a mock object, you can define its behavior using the `DEFINE_MOCK_BEHAVIOR` (for non-const methods) or `DEFINE_MOCK_BEHAVIOR_CONST` (for const methods) macro. This macro allows you to assign a lambda function that will be called when the mocked method is accessed.

Example:
@code{.cpp}
#include "iridium/testing/tester.h" // For ASSERT
#include "iridium/testing/mock.h"   // For mocking macros
#include <string>
#include <vector>
#include <stdexcept> // For std::runtime_error

// For completeness, copy IMyDependency and IMyDependencyMock definitions here
// In real code, they would be in header files
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
    DEFINE_MOCK_METHOD(int, getValue, (int))
    DEFINE_MOCK_METHOD_CONST(std::string, getName, ())
    DEFINE_MOCK_METHOD(void, processData, (const std::vector<int> &))
};
// End of copied definitions


// Class that uses IMyDependency
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
    IMyDependencyMock mockDep; // Create a mock instance

    // Define behavior for getValue
    DEFINE_MOCK_BEHAVIOR(int, getValue, mockDep, (int key)) {
        // This is a lambda function: [=](int key_param) -> int { ... }
        // Parameter names in the lambda can be anything, but types must match those declared in DEFINE_MOCK_METHOD
        if (key == 1) return 100;
        if (key == 42) return 420;
        return -1;
    };

    // Define behavior for getName (const method)
    DEFINE_MOCK_BEHAVIOR_CONST(std::string, getName, mockDep, ()) {
        // Lambda for a method with no arguments: [=]() -> std::string { ... }
        return "MockedName";
    };
    
    // Define behavior for processData (void method)
    std::vector<int> received_data;
    DEFINE_MOCK_BEHAVIOR(void, processData, mockDep, (const std::vector<int> &data)) {
        // Lambda for a void method: [=](const std::vector<int>& data_param) -> void { ... }
        received_data = data; // Copy data for verification
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
    
    // Check exception throwing from the main class, not the mock
    ASSERT(mainObj.fetchValue(-1), std::runtime_error);
}
@endcode
In the lambda function defining the behavior, you can access the arguments with which the mocked method was called and return an appropriate value or perform necessary actions. The argument types in the lambda must match the types specified in `DEFINE_MOCK_METHOD`.

Using `DEFINE_MOCK_CONSTRUCTOR` is necessary if your original interface/class has constructors with parameters that you want to call when creating the mock object (e.g., if the mock inherits from a class rather than a pure interface, and the base class constructor needs to be called).

@subsubsection subsubsec_testing_mock_auto_creation Automatic Mock Object Creation and Provisioning via `create()`

The Iridium framework provides a mechanism that allows static `create()` methods to return mock objects instead of real ones during testing. This is particularly useful when the class under test creates its dependencies internally. Understanding this mechanism requires looking at several macros and user conventions.

**Key Macros and Their Interaction:**

1.  **`DEFINE_MOCK_CREATE` (Preprocessor Symbol):**
    *   This is a **preprocessor symbol** that users should define for their test builds. This can be done, for example, by adding `#define DEFINE_MOCK_CREATE` at the top of a test file (before including Iridium headers like `iridium/smart_ptr.h`) or via a compiler option (e.g., `-DDEFINE_MOCK_CREATE`).
    *   The Iridium macro `DEFINE_CREATE(TClass)` (located in `iridium/smart_ptr.h`) uses the preprocessor directive `#ifdef DEFINE_MOCK_CREATE` to conditionally compile the static `TClass::create(...)` method.

2.  **`DEFINE_CREATE(TClass)` (Iridium Macro from `iridium/smart_ptr.h`):**
    *   This macro generates the static `TClass::create(...)` method.
    *   Due to the `#ifdef DEFINE_MOCK_CREATE` check within `DEFINE_CREATE`:
        *   If `DEFINE_MOCK_CREATE` is defined when `DEFINE_CREATE(TClass)` is processed, the resulting `TClass::create(...)` method will call `iridium::testing::Mock<TClass>::create(...)`.
        *   If `DEFINE_MOCK_CREATE` is *not* defined, then `TClass::create(...)` will call `std::make_shared<TClass>(...)` to create a real object.

3.  **`iridium::testing::Mock<TClass>::create(...)` (from `iridium/testing/mock.h`):**
    *   This method is called when `TClass::create(...)` has been generated in "mock mode" (due to `DEFINE_MOCK_CREATE`).
    *   It checks an internal static flag, `Mock<TClass>::m_is_mocked`.
    *   If `m_is_mocked == true` (this flag is set when an instance of a mock class, e.g., `TClassMock`, is created), then `Mock<TClass>::create(...)` attempts to return a previously registered instance of `TClassMock`. If no suitable mocks are available, an exception is thrown.
    *   If `m_is_mocked == false`, then `Mock<TClass>::create(...)` attempts to create a real object via `std::make_shared<TClass>(...)`. **Important:** If `TClass` is an abstract interface, this attempt will result in a compile-time error.

4.  **`DEFINE_IMPLEMENTATION(TClass)` (from `iridium/smart_ptr.h`):**
    *   This Iridium macro is used for concrete classes. It internally calls `DEFINE_CREATE(TClass)`.
    *   Consequently, the static `create()` method for classes defined with `DEFINE_IMPLEMENTATION` will automatically support switching between real and mock creation based on the presence of `DEFINE_MOCK_CREATE`.

5.  **`DEFINE_INTERFACE(Interface)` (from `iridium/smart_ptr.h`):**
    *   This Iridium macro is used for defining interfaces. It defines typedefs for smart pointers and a virtual destructor.
    *   **Important:** `DEFINE_INTERFACE` does **not** use `DEFINE_CREATE`. Thus, interfaces defined solely with `DEFINE_INTERFACE` do not automatically get a static `create()` method that is managed by `DEFINE_MOCK_CREATE`.

**Usage Scenarios:**

*   **Mocking a Concrete Class:** If you have a concrete class `CMyImpl` defined with `DEFINE_IMPLEMENTATION(CMyImpl)`, and you define `DEFINE_MOCK_CREATE` in your test setup, then calls to `CMyImpl::create(...)` will automatically be routed to `iridium::testing::Mock<CMyImpl>::create(...)`. If you've instantiated a `CMyImplMock`, it will then be provided.

*   **Mocking via an Interface:** If you want `IMyInterface::create(...)` to return a mock, and `IMyInterface` is defined only with `DEFINE_INTERFACE`, you must **manually implement** the static `IMyInterface::create(...)` method. In your implementation, you can use `DEFINE_MOCK_CREATE` (or another flag of your choosing) to decide whether to call `std::make_shared<CMyRealImpl>(...)` or `iridium::testing::Mock<IMyInterface>::create(...)`.

**Example (Mocking a concrete implementation, similar to `iridium-test/testing/example.cpp`):**

@code{.cpp}
// --- Start of Code Example ---

// Include Iridium headers. Order can be important if DEFINE_MOCK_CREATE affects them.
// DEFINE_MOCK_CREATE should be defined before smart_ptr.h, where DEFINE_CREATE resides.
// For this example, we'll place DEFINE_MOCK_CREATE directly in the "test file".

// File: IDataService.h
#pragma once
#include "iridium/smart_ptr.h" // For DEFINE_INTERFACE
#include <string>
#include <memory>

class IDataService {
public:
    DEFINE_INTERFACE(IDataService); // Does not create IDataService::create()
    virtual std::string fetchData(int id) = 0;
};

// File: CDataServiceImpl.h
#pragma once
#include "IDataService.h"
#include "iridium/smart_ptr.h" // For DEFINE_IMPLEMENTATION

class CDataServiceImpl : public IDataService {
    std::string m_serviceName;
public:
    // Constructor for the real implementation
    CDataServiceImpl(const std::string& name) : m_serviceName(name) {}

    // DEFINE_IMPLEMENTATION includes DEFINE_CREATE, which will respect DEFINE_MOCK_CREATE
    DEFINE_IMPLEMENTATION(CDataServiceImpl);

    std::string fetchData(int id) override {
        return "Real data for id " + std::to_string(id) + " from " + m_serviceName;
    }
};

// File: CDataServiceImplMock.h (or directly in the test .cpp)
#pragma once
#include "CDataServiceImpl.h" // We are mocking the concrete class
#include "iridium/testing/mock.h"

DEFINE_MOCK_CLASS(CDataServiceImpl) { // Creates CDataServiceImplMock
public:
    // Since CDataServiceImpl has a constructor with parameters,
    // the mock must call it via DEFINE_MOCK_CONSTRUCTOR.
    DEFINE_MOCK_CONSTRUCTOR(CDataServiceImpl)

    DEFINE_MOCK_METHOD(std::string, fetchData, (int));
};

// File: DataConsumer.h
#pragma once
#include "CDataServiceImpl.h" // Depends on the concrete implementation to call CDataServiceImpl::create()
#include <memory>
#include <string>

class DataConsumer {
    std::shared_ptr<CDataServiceImpl> m_dataService; // Uses the concrete class
public:
    DataConsumer() {
        // Call CDataServiceImpl::create(), which will be governed by DEFINE_MOCK_CREATE
        m_dataService = CDataServiceImpl::create("MyRealService");
    }

    std::string processData(int recordId) {
        if (!m_dataService) return "Error: Service not created";
        return "Consumed: " + m_dataService->fetchData(recordId);
    }
};

// File: test_main.cpp (or your test file)
// THIS IS KEY: DEFINE_MOCK_CREATE should be defined BEFORE including
// iridium/smart_ptr.h if it's to affect DEFINE_CREATE within it.
// For simplicity here, we assume CDataServiceImpl.h (which pulls in smart_ptr.h)
// will be processed by the compiler after this define.
#define DEFINE_MOCK_CREATE

#include "iridium/testing/tester.h"
// #include "DataConsumer.h"       // Conceptually included above
// #include "CDataServiceImplMock.h"

TEST(DataConsumer_UsesMockService) {
    // 1. Create an instance of the CDataServiceImplMock mock.
    //    This action sets Mock<CDataServiceImpl>::m_is_mocked = true;
    //    and registers this mockService in the list of available mocks.
    //    Pass constructor arguments as for the real CDataServiceImpl.
    CDataServiceImplMock mockService("MockedServiceInstance");

    // 2. Define behavior for the fetchData mock method
    DEFINE_MOCK_BEHAVIOR(std::string, fetchData, mockService, (int id)) {
        if (id == 101) {
            return "mocked_payload_for_101";
        }
        return "generic_mock_payload";
    };

    // 3. Create DataConsumer. Its constructor will call CDataServiceImpl::create("MyRealService").
    //    Because DEFINE_MOCK_CREATE is active, CDataServiceImpl::create() was generated
    //    to call iridium::testing::Mock<CDataServiceImpl>::create(...).
    //    Since mockService (of type CDataServiceImplMock) was created and registered,
    //    it will be returned. (The "MyRealService" argument would be used by the mock's constructor
    //    if it accepts it, or ignored if Mock<T>::create decides so for mocks).
    DataConsumer consumer;

    // 4. Verify that DataConsumer received and uses the mocked data
    ASSERT(consumer.processData(101), equal, "Consumed: mocked_payload_for_101");
    ASSERT(consumer.processData(200), equal, "Consumed: generic_mock_payload");
}
// --- End of Code Example ---
@endcode

**Summary for Interfaces:**

If you want to get a mock for an interface `IExample` via a call to `IExample::create()`, then, because `DEFINE_INTERFACE` does not provide `create()`, you must write this method yourself. For example:

@code{.cpp}
// In IExample.h or IExample.cpp
// #include "CRealExampleImpl.h" // Your real implementation
// #include "iridium/testing/mock.h" // For Mock<IExample>::create()

// std::shared_ptr<IExample> IExample::create() { // Assuming you declared static create() in IExample
// #ifdef USER_TEST_FLAG_FOR_IEXAMPLE_MOCK // User-defined flag
//     return iridium::testing::Mock<IExample>::create();
// #else
//     return std::make_shared<CRealExampleImpl>();
// #endif
// }
@endcode
In this scenario, `DEFINE_MOCK_CREATE` is not directly used by Iridium for `IExample::create()`, but you could use it (or a similar flag like `USER_TEST_FLAG_FOR_IEXAMPLE_MOCK`) in your own implementation of `IExample::create()`.

This detailed information should correctly reflect the mocking mechanism when using `create()` in Iridium.

@subsection subsec_testing_sequences Testing Call Sequences

Sometimes, it's important not only *which* methods of a mock object are called, but also the *order* in which they are called. The Iridium mocking framework provides tools to define and verify call sequences.

***Note:** This functionality may be incomplete or under development. The macros present in the code may not align with a full implementation of the `MockSequence` class.*

@subsubsection subsubsec_testing_defining_sequences Defining a Sequence (DEFINE_MOCK_SEQUENCE)

The `DEFINE_MOCK_SEQUENCE(name)` macro is used to create a sequence object.
-   `name`: The name you give to this sequence (a variable `sequence_<name>` will be created).

@subsubsection subsubsec_testing_sequence_expectations Expectations in a Sequence (DEFINE_MOCK_SEQUENCE_EXPECTATION)

After defining a sequence object, you add expected calls to it using the `DEFINE_MOCK_SEQUENCE_EXPECTATION(sequence_name, mock, method)` macro.
-   `sequence_name`: The name of the previously defined sequence.
-   `mock`: The mock object.
-   `method`: The name of the mocked method that is expected to be called.

In the current implementation, this macro does not allow specifying the expected arguments for the method call.
```