#!/bin/bash

OPTS=("  " "-p" "-r" "-u" "-pr" "-pu" "-ru" "-pru")

make -s re
> test1
> test2
echo "Options:"
for OP in "${OPTS[@]}"; do
	echo -n "\"${OP}\" -> "
	./ft_nm "${OP}" ./binary/* >> test1 2> /dev/null
	nm "${OP}" ./binary/* >> test2 2> /dev/null
	diff test1 test2 > /dev/null
	if [ "$?" -eq 0 ]; then
		echo "OK";
	else
		echo "KO";
	fi
done
rm -rf test1 test2
make -s fclean
