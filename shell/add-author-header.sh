#!/bin/bash


head_=`head -n1 $1`
if [ "$head_" == "// Copyright © 2019 Bulaev Vladimir." ]; then
    exit
fi

header="// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0
"
file=`echo "$header" | cat - $1`
echo "$file" > $1
