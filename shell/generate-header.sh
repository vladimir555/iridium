#!/bin/zsh


cd $(dirname "$0")
cd ../source

UUID=`uuidgen | sed -e 's/-/_/g' | tr '[:lower:]' '[:upper:]'`
HEADER_NAME="HEADER_`echo $1| tr '[:lower:]' '[:upper:]'`_${UUID}"
HEADER_MACRO_BEGIN="#ifndef ${HEADER_NAME}\n#define ${HEADER_NAME}\n\n"
HEADER_MACRO_END="\n\n\n#endif // ${HEADER_NAME}"

NAMESPACE_BEGIN=''
NAMESPACE_END=''
FILE_PATH=''
for ARG in ${@:2}; do
    NAMESPACE_BEGIN="${NAMESPACE_BEGIN}\nnamespace ${ARG} {"
    NAMESPACE_END="\n} // ${ARG}${NAMESPACE_END}"
    FILE_PATH="${FILE_PATH}${ARG}/"
done;
HEADER_PATH="${FILE_PATH}/$1.h"

echo "${HEADER_MACRO_BEGIN}${NAMESPACE_BEGIN}${NAMESPACE_END}${HEADER_MACRO_END}" > ${HEADER_PATH}
