#!/bin/bash

make -s re
./ft_nm binary/* > test1
./ft_nm -r binary/* >> test1
./ft_nm -u binary/* >> test1
./ft_nm -ru binary/* >> test1
nm binary/* > test2
nm -r binary/* >> test2
nm -u binary/* >> test2
nm -ru binary/* >> test2
#cat test1 | wc
diff test1 test2
rm -rf test1 test2
make -s fclean
