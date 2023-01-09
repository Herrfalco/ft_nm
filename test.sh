#!/bin/bash

make -s re
./ft_nm binary/* > test1
./ft_nm -p binary/* > test1
./ft_nm -r binary/* >> test1
./ft_nm -u binary/* >> test1
./ft_nm -pr binary/* >> test1
./ft_nm -pu binary/* >> test1
./ft_nm -ru binary/* >> test1
./ft_nm -pru binary/* >> test1
nm binary/* > test2
nm -p binary/* > test2
nm -r binary/* >> test2
nm -u binary/* >> test2
nm -pr binary/* >> test2
nm -pu binary/* >> test2
nm -ru binary/* >> test2
nm -pru binary/* >> test2
cat test1 | wc
diff test1 test2
rm -rf test1 test2
make -s fclean
