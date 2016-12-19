#!/bin/bash
sudo updatedb

SEARCH=$(locate BIOS-bochs-latest)

IFS=$'\n' read -ra OUTPUT <<< "$SEARCH"
ROMIMAGE="${OUTPUT[0]}"

SEARCH=$(locate VGABIOS-lgpl-latest)

IFS=$'\n' read -ra OUTPUT <<< "$SEARCH"
VGAROMIMAGE="${OUTPUT[0]}"

cp .bochsrc ./.bochsrc.tmp

sed -e "s~^romimage: file=.*$~romimage: file=\"$ROMIMAGE\"~" \
    -e "s~^vgaromimage: file=.*$~vgaromimage: file=\"$VGAROMIMAGE\"~" .bochsrc.tmp > .bochsrc

rm .bochsrc.tmp
