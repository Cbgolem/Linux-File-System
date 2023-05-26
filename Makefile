project4: sdisk.o filesys.o block.o shell.o table.o main4.o
	g++ sdisk.o filesys.o block.o shell.o table.o main4.o -o project4
	
project3: sdisk.o filesys.o block.o shell.o main3.o
	g++ sdisk.o filesys.o block.o shell.o main3.o -o project3
	
project2: sdisk.o main2.o filesys.o block.o
	g++ sdisk.o main2.o filesys.o block.o -o project2

project1: sdisk.o main.o 
	g++ sdisk.o main.o  -o project1

block.o: block.cpp
	g++ -c block.cpp
	
sdisk.o: sdisk.cpp
	g++ -c sdisk.cpp

main.o: main.cpp
	g++ -c main.cpp
	
main2.o: main2.cpp
	g++ -c main2.cpp
	
main3.o: main3.cpp
	g++ -c main3.cpp

main4.o: main4.cpp
	g++ -c main4.cpp
	
filesys.o: filesys.cpp
	g++ -c filesys.cpp

shell.o: shell.cpp
	g++ -c shell.cpp

table.o: table.cpp
	g++ -c table.cpp

clean:
	rm *.o
	
