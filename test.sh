#!/bin/bash

make -s re
./ft_nm binary/* > test1
nm binary/* > test2
diff test1 test2
rm -rf test1 test2
make -s fclean
