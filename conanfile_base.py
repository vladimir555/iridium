from conan.tools.cmake import (
    CMake,
    CMakeToolchain,
    CMakeDeps,
    cmake_layout
)

from conan.tools.files import load, copy
from conan.tools.build import cross_building, check_min_cppstd
from conan.errors import ConanException, ConanInvalidConfiguration

import os
import re


class ProjectBase:
    settings = "os", "compiler", "build_type", "arch"

    options = {
        "fPIC": [True, False],
        "with_openssl": [True, False],
        "with_postgres": [True, False],
        "with_mysql": [True, False],
    }

    default_options = {
        "fPIC": True,
        "with_openssl": False,
        "with_postgres": False,
        "with_mysql": False,
    }

    exports_sources = (
        "LICENSE",
        "CMakeLists.txt",
        "source/*",
        "external/*",
        "cmake/*",
        "script/*",
    )

    def validate(self):
        cppstd = self.settings.compiler.get_safe("cppstd")

        if cppstd:
            check_min_cppstd(self, "17")

        if str(cppstd) not in ("17", "gnu17"):
            raise ConanInvalidConfiguration(
                "Only C++17 is supported"
            )

    def set_name(self):
        source_dir = os.path.join(
            self.recipe_folder,
            "source/library"
        )

        for d in os.listdir(source_dir):
            if os.path.isfile(
                os.path.join(source_dir, d, "version.h")
            ):
                self.name = d
                return

        raise ConanException(
            "Main library folder with version.h not found"
        )

    def set_version(self):
        version_h = os.path.join(
            self.recipe_folder,
            "source/library",
            self.name,
            "version.h"
        )

        content = load(self, version_h)

        name_upper = self.name.upper().replace("-", "_")

        major = re.search(
            rf"{name_upper}_VERSION_MAJOR\s*[= \t]+([0-9]+)",
            content
        )

        minor = re.search(
            rf"{name_upper}_VERSION_MINOR\s*[= \t]+([0-9]+)",
            content
        )

        patch = re.search(
            rf"{name_upper}_VERSION_PATCH\s*[= \t]+([0-9]+)",
            content
        )

        self.version = (
            f"{major.group(1)}."
            f"{minor.group(1)}."
            f"{patch.group(1)}"
        )

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def requirements(self):
        if self.options.with_openssl:
            self.requires("openssl/[>=3.0 <4]")

    def layout(self):
        cmake_layout(self)

        self.folders.source = "."
        self.folders.generators = "build/conan"

    def generate(self):
        tc = CMakeToolchain(self)

        tc.variables["CONAN_PROJECT_NAME"] = self.name

        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)

        cmake.configure(
            variables={
                "CONAN_PROJECT_NAME": self.name
            }
        )

        cmake.build()

        if (self.conf.get("user.iridium:run_tests", default=False, check_type=bool) and not cross_building(self)):
            cmake.test()

    def package(self):
        cmake = CMake(self)
        cmake.install()

        copy(self, "LICENSE", src=self.source_folder,
             dst=os.path.join(self.package_folder, "licenses"))

        copy(self, "*.cmake", src=os.path.join(self.source_folder, "cmake"),
             dst=os.path.join(self.package_folder, "lib", "cmake", self.name))

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libs = [self.name]

        self.cpp_info.set_property(
            "cmake_file_name", self.name)
        self.cpp_info.set_property(
            "cmake_target_name", f"{self.name}::{self.name}")
        self.cpp_info.set_property(
            "cmake_build_modules", [
                os.path.join("lib", "cmake", self.name, "iridium.cmake")
            ]
        )

        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.system_libs = ["pthread", "dl", "m"]
        elif self.settings.os == "Windows":
            self.cpp_info.system_libs = ["ws2_32", "iphlpapi"]

        for option_name, enabled in self.options.items():
            if option_name.startswith("with_") and bool(enabled):
                define = f"BUILD_FLAG_{option_name[5:].upper()}"
                self.cpp_info.defines.append(define)
