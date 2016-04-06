mkdir bin
gcc src/main.c -o bin/icmp_flooder 
cp bin/icmp_flooder /usr/bin
rm bin/icmp_flooder
