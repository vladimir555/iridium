#!/bin/zsh

NAME=`echo $1| tr '[:lower:]' '[:upper:]'`
UUID=`uuidgen | sed -e 's/-/_/g' | tr '[:lower:]' '[:upper:]'`
echo "#ifndef HEADER_"$NAME"_"$UUID
echo "#define HEADER_"$NAME"_"$UUID
echo "\n"
namespaces_end=''
for arg in ${@:2}; do
    echo "namespace $arg {"
    namespaces_end="\n} // $arg$namespaces_end"
done;
echo $namespaces_end
echo "\n"
echo "#endif // HEADER_"$NAME"_"$UUID
