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

    options = {
        **ProjectBase.options,
        "with_openssl": [True, False],
        "with_postgres": [True, False],
        "with_mysql": [True, False],
    }

    default_options = {
        **ProjectBase.default_options,
        "with_openssl": False,
        "with_postgres": False,
        "with_mysql": False,
    }

    def requirements(self):
        if self.options.get_safe("with_openssl"):
            self.requires("openssl/[>=3.0 <4]", transitive_headers=True, transitive_libs=True)
        if self.options.get_safe("with_postgres"):
            self.requires("libpq/[>=14 <18]", transitive_headers=True, transitive_libs=True)
        if self.options.get_safe("with_mysql"):
            self.requires("libmysqlclient/[>=8.0 <10]", transitive_headers=True, transitive_libs=True)
