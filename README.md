# Donat

XLM address: GDP34WXZRCSHVUDQLGKJKOBMS5LOQPHCIADZU5POEF3IICZ7XNQJ65Y6

XLM tag: 1752484

# Documentation
https://vladimir555.github.io/iridium/index.html


# Conan
conan remote add iridium --force https://registry.gitflic.ru/project/volodja/iridium/package/-/conan
conan list "iridium/*" -r=iridium
conan install --requires=iridium/{VERSION} -r=iridium --build=missing