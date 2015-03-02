gcc test.c our_queue.c -pthread
set -e
for i in {1..100}
do
   ./a.out
    if [ $? -eq 139 ]; then
        echo "Segfault Oh no!!"
        exit 1
    fi

done
