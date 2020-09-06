all:ser

ser:ser.o socket.o thread.o user.o
	g++ -std=c++11 -o ser ser.o socket.o thread.o user.o -pthread `mysql_config --libs` -I/usr/include  -lreadline 

ser.o:ser.c
	g++ -std=c++11 -c ser.c -g

socket.o:socket.c
	g++ -std=c++11 -c socket.c -g

thread.o:thread.c
	g++ -std=c++11 -c thread.c -g

user.o:user.c
	g++ -std=c++11 -c `mysql_config --cflags` user.c -g

clean:
	rm -rf *.o ser
