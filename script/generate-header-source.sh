#!/bin/zsh

cd $(dirname "$0")
cd ../source/library

FORCE=0
if [[ "$1" == "-f" || "$1" == "--force" ]]; then
    FORCE=1
    shift
fi

UUID=`uuidgen | sed -e 's/-/_/g' | tr '[:lower:]' '[:upper:]'`
HEADER_NAME="HEADER_`echo $1 | tr '[:lower:]' '[:upper:]'`_${UUID}"

HEADER_MACRO_BEGIN="#ifndef ${HEADER_NAME}\n#define ${HEADER_NAME}\n\n\n"
HEADER_MACRO_END="\n\n\n#endif // ${HEADER_NAME}"

FILE_PATH=''
NAMESPACE_PARTS=()

for ARG in ${@:2}; do
    FILE_PATH="${FILE_PATH}${ARG}/"
    NAMESPACE_PARTS+=("${ARG}")
done

NAMESPACE_BEGIN=''
NAMESPACE_END=''

if [ ${#NAMESPACE_PARTS[@]} -gt 0 ]; then
    NAMESPACE_PATH="${(j[::])NAMESPACE_PARTS}"
    NAMESPACE_BEGIN="namespace ${NAMESPACE_PATH} {\n"
    NAMESPACE_END="\n} // ${NAMESPACE_PATH}"
fi

HEADER_PATH="${FILE_PATH}/$1.h"
SOURCE_PATH="${FILE_PATH}/$1.cpp"

if [ $FORCE -eq 0 ]; then
    if [ -f "${HEADER_PATH}" ]; then
        echo "Warning: ${HEADER_PATH} already exists. Skipping."
        return 1
    fi
    if [ -f "${SOURCE_PATH}" ]; then
        echo "Warning: ${SOURCE_PATH} already exists. Skipping."
        return 1
    fi
fi

mkdir -p "$(dirname "${HEADER_PATH}")"

# .h
print "${HEADER_MACRO_BEGIN}${NAMESPACE_BEGIN}${NAMESPACE_END}${HEADER_MACRO_END}" > ${HEADER_PATH}
echo "Created: ${HEADER_PATH}"

# .cpp (--force)
if [ $FORCE -eq 1 ] || [ ! -f "${SOURCE_PATH}" ]; then
    print "#include \"$1.h\"\n\n\n${NAMESPACE_BEGIN}${NAMESPACE_END}" > ${SOURCE_PATH}
    echo "Created: ${SOURCE_PATH}"
else
    echo "Skipped: ${SOURCE_PATH} (already exists)"
fi
