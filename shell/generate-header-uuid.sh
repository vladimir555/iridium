NAME=`echo $1| tr '[:lower:]' '[:upper:]'`
UUID=`uuidgen | sed -e 's/-/_/g' | tr '[:lower:]' '[:upper:]'`
echo "#ifndef HEADER_"$NAME"_"$UUID
echo "#define HEADER_"$NAME"_"$UUID
echo "#endif // HEADER_"$NAME"_"$UUID
