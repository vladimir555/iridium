# Copyright © 2019-2026 Bulaev Vladimir.
# Contacts: <bulaev_vladimir@mail.ru>
# License: https://www.gnu.org/licenses/lgpl-3.0

from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.env import VirtualRunEnv
from conan.tools.files import load, copy
from conan.tools.build import cross_building, check_min_cppstd
from conan.errors import ConanException, ConanInvalidConfiguration

import os
import re


class ProjectBase:
    settings = "os", "compiler", "build_type", "arch"

    options = {
        "fPIC": [True, False],
    }

    default_options = {
        "fPIC": True,
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

    def set_name(self):
        source_dir = os.path.join(self.recipe_folder, "source/library")

        for d in os.listdir(source_dir):
            if os.path.isfile(os.path.join(source_dir, d, "version.h")):
                self.name = d
                return

        raise ConanException("Main library folder with version.h not found")

    def set_version(self):
        version_h_path = os.path.join(
            self.recipe_folder,
            "source/library",
            self.name,
            "version.h"
        )

        version_h = load(self, version_h_path)
        name_upper = self.name.upper().replace("-", "_")
        major = re.search(rf"{name_upper}_VERSION_MAJOR\s*[= \t]+([0-9]+)", version_h)
        minor = re.search(rf"{name_upper}_VERSION_MINOR\s*[= \t]+([0-9]+)", version_h)
        patch = re.search(rf"{name_upper}_VERSION_PATCH\s*[= \t]+([0-9]+)", version_h)

        self.version = (
            f"{major.group(1)}."
            f"{minor.group(1)}."
            f"{patch.group(1)}"
        )

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

        self.folders.source = "."
        self.folders.generators = "build/conan"

    def generate(self):
        tc = CMakeToolchain(self)

        tc.variables["CONAN_PROJECT_NAME"] = self.name

        for option_name in self.__class__.options:
            if option_name.startswith("with_") and self.options.get_safe(option_name):
                tc.variables[f"IRIDIUM_BUILD_FLAG_{option_name[5:].upper()}"] = self.options.get_safe(option_name, False)

        conan_directory = os.environ.get(
            "CONAN_HOME",
            os.path.join(os.path.expanduser("~"), ".conan2"),
        )
        cache_directory = self.conf.get(
            "user.iridium:cache_directory",
            default=os.path.join(conan_directory, "cache_directory"),
        )
        tc.variables["IRIDIUM_BUILD_CACHE_DIRECTORY"] = cache_directory

        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()
        env = VirtualRunEnv(self)
        env.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure( variables={ "CONAN_PROJECT_NAME": self.name } )
        cmake.build()

        if (self.conf.get("user.iridium:run_tests", default=False, check_type=bool) and not cross_building(self)):
            try:
                cmake.test(env="conanrun")
            finally:
                log_path = os.path.join(self.build_folder, "Testing", "Temporary", "LastTest.log")

                if os.path.isfile(log_path):
                    self.output.info(load(self, log_path))

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

        self.cpp_info.set_property("cmake_file_name", self.name)
        self.cpp_info.set_property("cmake_target_name", f"{self.name}::{self.name}")
        self.cpp_info.set_property("cmake_build_modules",
            [ os.path.join("lib", "cmake", self.name, "iridium.cmake") ]
        )

        if self.settings.os in ["Linux", "FreeBSD"]:
            self.cpp_info.system_libs = ["pthread", "dl", "m"]
        elif self.settings.os == "Windows":
            self.cpp_info.system_libs = ["ws2_32", "iphlpapi"]
