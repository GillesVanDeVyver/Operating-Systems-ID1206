echo Performs 10 million entries on 4 threads
gcc -o list list.c -lpthread
gcc -o clist clist.c -lpthread
gcc -o slist slist.c -lpthread
echo running sequential list
./list 10000000 4
echo running concurrent list
./clist 10000000 4
echo running slist with spinlock
./slist 10000000 4

