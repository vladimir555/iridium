#!/bin/zsh


cd $(dirname "$0")
cd ../source

UUID=`uuidgen | sed -e 's/-/_/g' | tr '[:lower:]' '[:upper:]'`
HEADER_NAME="HEADER_`echo $1| tr '[:lower:]' '[:upper:]'`_${UUID}"
HEADER_MACRO_BEGIN="#ifndef ${HEADER_NAME}\n#define ${HEADER_NAME}\n\n\n"
HEADER_MACRO_END="\n\n\n#endif // ${HEADER_NAME}"

NAMESPACE_BEGIN='namespace '
NAMESPACE_END='} // '
FILE_PATH=''
NS_PARTS=()

for ARG in ${@:2}; do
    NS_PARTS+=("${ARG}")
    FILE_PATH="${FILE_PATH}${ARG}/"
done;

if [ ${#NS_PARTS[@]} -gt 0 ]; then
    # Join with :: using printf (reliable across zsh versions)
    NS_PATH=$(printf '%s::' "${NS_PARTS[@]}")
    NS_PATH=${NS_PATH%::}  # remove trailing ::
    NAMESPACE_BEGIN="${NAMESPACE_BEGIN}${NS_PATH} {\n"
    NAMESPACE_END="${NAMESPACE_END}${NS_PATH}"
fi

HEADER_PATH="${FILE_PATH}/$1.h"

if [ -f "${HEADER_PATH}" ]; then
    echo "${HEADER_PATH} exists."
    return 1
fi

print "${HEADER_MACRO_BEGIN}${NAMESPACE_BEGIN}${NAMESPACE_END}${HEADER_MACRO_END}" > ${HEADER_PATH}

SOURCE_PATH="${FILE_PATH}/$1.cpp"
print "#include \"$1.h\"\n\n${NAMESPACE_BEGIN}${NAMESPACE_END}" >> ${SOURCE_PATH}
