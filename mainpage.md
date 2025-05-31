/*! @mainpage Iridium Library Documentation

Welcome to the Iridium Library documentation.

@section sec_using_tests Using tests

Describe how to use the testing framework in this project.
(Content to be filled in)

The Iridium testing framework utilizes a set of macros and classes to define and run unit tests.
To create a test, you typically ...
Example:
```cpp
// Example test case
#include "iridium/testing/test.h"

DEFINE_TEST_CASE(MyFeatureTest, MySuite)
{
    ASSERT_EQ(1, 1, "Basic assertion");
}
```
To run tests, ... (build target, command, etc.)

@section sec_using_convert Using `convert` specializations

The `iridium::convertion::convert` function template provides a flexible way to transform values between different types.
This is extensively used in logging, test assertions (`ASSERT_EQ`, `ASSERT_NE`, etc., which use it for stringification), and potentially serialization.

To add support for a custom type `MyType` to be convertible (e.g., to `std::string` for logging), you need to specialize the `iridium::convertion::Convert<std::string, MyType>` struct (or other target types as needed).

Example for converting `MyType` to `std::string`:
```cpp
#include "iridium/convertion/convert.h" // For Convert and convert
#include <string>

// Your custom type
struct MyType {
    int id;
    std::string data;
};

namespace iridium { namespace convertion {
    template <>
    struct Convert<std::string, MyType> {
        static std::string convert(const MyType& value) {
            return "MyType(id: " + iridium::convertion::convert<std::string>(value.id) + ", data: " + value.data + ")";
        }
    };
}} // namespace iridium::convertion

// Now you can use:
// MyType obj = {1, "test"};
// std::string str_obj = iridium::convertion::convert<std::string>(obj);
// iridium::log::info(obj); // If logging uses convert to string
```
Ensure your specialization is in the `iridium::convertion` namespace.

@section sec_using_macros Using `DEFINE_INTERFACE` and `DEFINE_IMPLEMENTATION` macros

The Iridium library provides macros to simplify the definition of interfaces and their implementations, promoting a clean separation of concerns.

**`DEFINE_INTERFACE(InterfaceName, ...)`**

This macro is used to declare an interface.
- `InterfaceName`: The name of the interface (e.g., `ILogger`).
- `...`: A list of pure virtual functions that define the contract of the interface.

Example:
```cpp
#include "iridium/pattern/prototype.h" // Or other relevant base if needed

DEFINE_INTERFACE(IFileSystem,
    virtual bool fileExists(const std::string& path) const = 0;
    virtual std::string readFile(const std::string& path) const = 0;
    // ... other methods
);
```
This typically expands to a struct or class with the given name and pure virtual methods. It might also inherit from a common base like `iridium::pattern::prototype_i`.

**`DEFINE_IMPLEMENTATION(InterfaceName, ImplementationName, ...)`**

This macro is used to define a concrete implementation for an interface.
- `InterfaceName`: The interface this class implements.
- `ImplementationName`: The name of the concrete class (e.g., `PosixFileSystem`).
- `...`: The implementation of the interface's methods. You may also add other public/protected/private members as needed.

Example:
```cpp
#include "MyFileSystemInterface.h" // Assuming IFileSystem is defined here
#include <fstream> // For actual file operations

DEFINE_IMPLEMENTATION(IFileSystem, PosixFileSystem,
public:
    bool fileExists(const std::string& path) const override {
        std::ifstream f(path.c_str());
        return f.good();
    }

    std::string readFile(const std::string& path) const override {
        std::ifstream f(path.c_str());
        if (f.good()) {
            std::string str((std::istreambuf_iterator<char>(f)),
                             std::istreambuf_iterator<char>());
            return str;
        }
        throw std::runtime_error("Could not read file: " + path);
    }
    // ... other method implementations
);
```
The macro usually handles the inheritance (`ImplementationName : public InterfaceName`) and boilerplate.
Refer to the specific macro definitions for exact expansion details and any base classes they might automatically include (e.g., for clonability or factories).

*/
