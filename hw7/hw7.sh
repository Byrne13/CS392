gcc -o server server.c
gcc -o client client.c

./server 2000 &
SERVER=$!

sleep 3

./client 2000

kill $SERVER
