from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools.files import load
from conan.errors import ConanException
import os
import re

required_conan_version = ">=2.0"

class IridiumConan(ConanFile):
    name = None
    version = None
    license = "LGPL"
    author = "Vladimir Bulaev bulaev_vladimir@mail.ru"
    url = "https://github.com/volodja555/iridium"
    description = "Iridium C++ library"
    topics = ("cpp", "network", "security")
    settings = "os", "compiler", "build_type", "arch"
    package_type = "library"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "with_openssl": [True, False],
        "with_postgres": [True, False],
        "with_mysql": [True, False],
    }
    default_options = {
        "shared": True,
        "fPIC": True,
        "with_openssl": False,
        "with_postgres": False,
        "with_mysql": False,
    }

    exports_sources = "CMakeLists.txt", "source/*", "external/*"

    def set_name(self):
        source_dir = os.path.join(self.recipe_folder, "source")
        for d in os.listdir(source_dir):
            if d.endswith("-test") or d.endswith("-binary"):
                continue
            if os.path.isfile(os.path.join(source_dir, d, "version.h")):
                self.name = d
                self.output.info(f"Project name: {self.name}")
                return
        raise ConanException("Main library folder with version.h not found")

    def set_version(self):
        version_h = os.path.join(self.recipe_folder, "source", self.name, "version.h")
        if not os.path.exists(version_h):
            raise ConanException(f"version.h not found: {version_h}")
        content = load(self, version_h)
        name_upper = self.name.upper()
        major = re.search(rf"{name_upper}_VERSION_MAJOR\s* \s*([0-9]+)", content)
        minor = re.search(rf"{name_upper}_VERSION_MINOR\s* \s*([0-9]+)", content)
        patch = re.search(rf"{name_upper}_VERSION_PATCH\s* \s*([0-9]+)", content)
        if not all([major, minor, patch]):
            raise ConanException("Failed to parse version from version.h")
        self.version = f"{major.group(1)}.{minor.group(1)}.{patch.group(1)}"
        self.output.info(f"Version: {self.version}")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            del self.options.fPIC

    def requirements(self):
        if self.options.with_openssl:
            self.requires("openssl/[>=3.0 <4]")
        if self.options.with_postgres:
            self.requires("libpq/[>=14 <17]")
        if self.options.with_mysql:
            self.requires("mysql-connector-c/[>=6.1 <7]")

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"
        self.folders.generators = "build/conan"

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["CONAN_PROJECT_NAME"] = self.name
        tc.variables["CONFIG_OPENSSL"] = self.options.with_openssl
        tc.variables["CONFIG_POSTGRES"] = self.options.with_postgres
        tc.variables["CONFIG_MYSQL"] = self.options.with_mysql
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(variables={"CONAN_PROJECT_NAME": self.name})
        cmake.build()
        cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", self.name)
        self.cpp_info.set_property("cmake_target_name", self.name)
        self.cpp_info.libs = [self.name]
        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.system_libs.extend(["pthread", "dl"])
        if self.settings.os == "Windows":
            self.cpp_info.system_libs.extend(["ws2_32", "iphlpapi"])
        if self.options.with_openssl:
            self.cpp_info.defines.append("BUILD_FLAG_OPENSSL")
        if self.options.with_postgres:
            self.cpp_info.defines.append("BUILD_FLAG_POSTGRES")
        if self.options.with_mysql:
            self.cpp_info.defines.append("BUILD_FLAG_MYSQL")
