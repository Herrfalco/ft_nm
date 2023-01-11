#!/bin/bash

OPTS=("  " "-p" "-r" "-u" "-pr" "-pu" "-ru" "-pru")

make -s re
> test1
> test2
echo "Options:"
for OP in "${OPTS[@]}"; do
	echo -n "\"${OP}\" -> "
	./ft_nm "${OP}" ./binary/* > test1 2> /dev/null
	nm "${OP}" ./binary/* > test2 2> /dev/null
	diff test1 test2 > /dev/null
	if [ "$?" -eq 0 ]; then
		echo "OK";
	else
		echo "KO";
	fi
done
echo "From a.out:"
mv ft_nm a.out
./a.out > test1 2> /dev/null
nm > test2 2> /dev/null
diff test1 test2 > /dev/null
if [ "$?" -eq 0 ]; then
	echo "OK";
else
	echo "KO";
fi
rm -rf test1 test2 a.out
make -s fclean
