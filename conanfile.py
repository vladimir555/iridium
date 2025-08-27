from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools.files import load
from conan.errors import ConanException
import os

required_conan_version = ">=2.0"

class IridiumConan(ConanFile):
    name = "iridium"
    version = None
    license = "LGPL"
    author = "Vladimir Bulaev bulaev_vladimir@mail.ru"
    url = "https://github.com/volodja555/iridium"
    description = "Iridium C++ library"
    topics = ("cpp", "network", "security")
    settings = "os", "compiler", "build_type", "arch"
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
    exports_sources = "CMakeLists.txt", "source/*", "external/*", "version.txt"

    def set_version(self):
        version_path = os.path.join(self.recipe_folder, "version.txt")
        if not os.path.exists(version_path):
            raise ConanException("version.txt not found!")
        self.version = load(self, version_path).strip()

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
        # Generate toolchain.cmake, conanfile.cmake
        tc = CMakeToolchain(self)
        tc.variables["CONFIG_OPENSSL"]   = self.options.with_openssl
        tc.variables["CONFIG_POSTGRES"] = self.options.with_postgres
        tc.variables["CONFIG_MYSQL"]    = self.options.with_mysql
        tc.generate()

        # Generate find_package() configs
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "iridium")
        self.cpp_info.set_property("cmake_target_name", "iridium")
        self.cpp_info.libs = ["iridium"]

        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.system_libs.extend(["pthread", "dl"])
        if self.settings.os == "Windows":
            self.cpp_info.system_libs.extend(["ws2_32", "iphlpapi"])

        # pass defines
        if self.options.with_openssl:
            self.cpp_info.defines.append("BUILD_FLAG_OPENSSL")
        if self.options.with_postgres:
            self.cpp_info.defines.append("BUILD_FLAG_POSTGRES")
        if self.options.with_mysql:
            self.cpp_info.defines.append("BUILD_FLAG_MYSQL")
