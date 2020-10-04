
touch val.txt
make
valgrind ./Cater.out 2>val.txt
va=$(tail -n 1 val.txt)
