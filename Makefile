b_plus_tree: main.o read_file.o
	g++ -o b_plus_tree main.o read_file.o

main.o: src/main.cpp src/headers.h
	g++ -c src/main.cpp
read_file.o: src/read_file.cpp src/headers.h
	g++ -c src/read_file.cpp

clean:
	rm main.o read_file.o