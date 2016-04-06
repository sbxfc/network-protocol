mkdir bin
gcc src/main.c -o bin/icmp_smurf 
cp bin/icmp_smurf /usr/bin
rm bin/icmp_smurf
