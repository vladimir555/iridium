# Copyright © 2019 Bulaev Vladimir.
# Contacts: <bulaev_vladimir@mail.ru>
# License: https://www.gnu.org/licenses/lgpl-3.0

from conan import ConanFile
from conanfile_base import ProjectBase


class IridiumConan(ProjectBase, ConanFile):
    license = "LGPL-3.0-only"
    author = "Vladimir Bulaev <bulaev_vladimir@mail.ru>"
    url = "https://github.com/volodja555/iridium"
    description = "Iridium C++ library"
    topics = ("cpp", "network", "security")
    package_type = "shared-library"
    exports = ("conanfile_base.py",)

# from conan import ConanFile
# from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
# from conan.tools.files import load, copy
# from conan.tools.build import cross_building, check_min_cppstd
# from conan.errors import ConanException, ConanInvalidConfiguration
# import os
# import re


# class IridiumConan(ConanFile):
#     name = None
#     version = None
#     license = ("LGPL-3.0-only")
#     author = "Vladimir Bulaev <bulaev_vladimir@mail.ru>"
#     url = "https://github.com/volodja555/iridium"
#     description = "Iridium C++ library"
#     topics = ("cpp", "network", "security")
#     settings = "os", "compiler", "build_type", "arch"
#     package_type = "shared-library"
#     options = {
#         "fPIC": [True, False],
#         "with_openssl": [True, False],
#         "with_postgres": [True, False],
#         "with_mysql": [True, False],
#     }
#     default_options = {
#         "fPIC": True,
#         "with_openssl": False,
#         "with_postgres": False,
#         "with_mysql": False,
#     }

#     exports_sources = \
#         "LICENSE", \
#         "CMakeLists.txt", \
#         "source/*", \
#         "external/*", \
#         "cmake/*", \
#         "script/iridium-update-project-version.sh", \
#         "conan_helper/*"

#     def validate(self):
#         cppstd = self.settings.compiler.get_safe("cppstd")

#         if cppstd:
#             check_min_cppstd(self, "17")

#         if str(cppstd) not in ("17", "gnu17"):
#             raise ConanInvalidConfiguration("Only C++17 is supported")

#     def set_name(self):
#         source_dir = os.path.join(self.recipe_folder, "source/library")
#         for d in os.listdir(source_dir):
#             if os.path.isfile(os.path.join(source_dir, d, "version.h")):
#                 self.name = d
#                 self.output.info(f"Project name: {self.name}")
#                 return
#         raise ConanException("Main library folder with version.h not found")

#     def set_version(self):
#         version_h = os.path.join(self.recipe_folder, "source/library", self.name, "version.h")

#         if not os.path.exists(version_h):
#             raise ConanException(f"version.h not found: {version_h}")

#         content = load(self, version_h)
#         name_upper = self.name.upper().replace("-", "_")
#         major = re.search(rf"{name_upper}_VERSION_MAJOR\s*[= \t]+([0-9]+)", content)
#         minor = re.search(rf"{name_upper}_VERSION_MINOR\s*[= \t]+([0-9]+)", content)
#         patch = re.search(rf"{name_upper}_VERSION_PATCH\s*[= \t]+([0-9]+)", content)

#         if not all([major, minor, patch]):
#             raise ConanException("Failed to parse version from version.h")

#         self.version = f"{major.group(1)}.{minor.group(1)}.{patch.group(1)}"
#         self.output.info(f"Version: {self.version}")

#     def config_options(self):
#         if self.settings.os == "Windows":
#             self.options.rm_safe("fPIC")

#     def configure(self):
#         if self.options.get_safe("shared"):
#             self.options.rm_safe("fPIC")

#     def requirements(self):
#         if self.options.with_openssl:
#             self.requires("openssl/[>=3.0 <4]", transitive_headers=True, transitive_libs=True)
#         if self.options.with_postgres:
#             self.requires("libpq/[>=14 <17]", transitive_headers=True, transitive_libs=True)
#         if self.options.with_mysql:
#             self.requires("mysql-connector-c/[>=6.1 <7]", transitive_headers=True, transitive_libs=True)

#     def layout(self):
#         cmake_layout(self)
#         self.folders.source = "."
#         self.folders.generators = "build/conan"

#     def generate(self):
#         tc = CMakeToolchain(self)
#         tc.variables["CONAN_PROJECT_NAME"] = self.name
#         tc.variables["CONFIG_OPENSSL"] = self.options.with_openssl
#         tc.variables["CONFIG_POSTGRES"] = self.options.with_postgres
#         tc.variables["CONFIG_MYSQL"] = self.options.with_mysql
#         tc.variables["BUILD_TESTING"] = self.conf.get("user.iridium:run_tests", default=False, check_type=bool)
#         tc.generate()

#         deps = CMakeDeps(self)
#         deps.generate()

#     def build(self):
#         cmake = CMake(self)
#         cmake.configure(variables={"CONAN_PROJECT_NAME": self.name})
#         cmake.build()

#         run_tests = self.conf.get("user.iridium:run_tests", default=False, check_type=bool)
#         if run_tests and not cross_building(self):
#             cmake.test()

#     def package(self):
#         cmake = CMake(self)
#         cmake.install()

#         copy(self, "LICENSE", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))

#     def package_info(self):
#         self.cpp_info.set_property("cmake_file_name", self.name)
#         self.cpp_info.set_property("cmake_target_name", f"{self.name}::{self.name}")
#         self.cpp_info.set_property("cmake_config_version_compat", "SameMinorVersion")

#         self.cpp_info.libs = [self.name]

#         if self.settings.os in ["Linux", "FreeBSD"]:
#             self.cpp_info.system_libs = ["pthread", "dl", "m"]
#         elif self.settings.os == "Windows":
#             self.cpp_info.system_libs = ["ws2_32", "iphlpapi"]

#         # Флаги компиляции для опциональных зависимостей
#         if self.options.with_openssl:
#             self.cpp_info.defines.append("BUILD_FLAG_OPENSSL")
#         if self.options.with_postgres:
#             self.cpp_info.defines.append("BUILD_FLAG_POSTGRES")
#         if self.options.with_mysql:
#             self.cpp_info.defines.append("BUILD_FLAG_MYSQL")

#         # Прозрачное пробрасывание зависимостей потребителям
#         if self.options.with_openssl:
#             self.cpp_info.requires.append("openssl::openssl")
#         if self.options.with_postgres:
#             self.cpp_info.requires.append("libpq::libpq")
#         if self.options.with_mysql:
#             self.cpp_info.requires.append("mysql-connector-c::mysqlclient")
