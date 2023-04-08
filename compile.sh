#!/usr/bin/env bash

COMPILE_SETTINGS=$1

if [ -z "${COMPILE_SETTINGS}" ];
then
    COMPILE_SETTINGS="A"
fi

#can't take inputs in $2 or $3 with a | in them, since that's the sed delimiter
function replace() { 
    sed -i '' 's|'$2'|'$3'|' $1
}

git submodule update --init --recursive
touch temp.txt

echo "$(cat build_settings.txt)\n$COMPILE_SETTINGS\n" > temp.txt

python3.11 VulkanEngine/build.py < temp.txt
rm temp.txt