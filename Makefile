b_plus_tree: main.o read_file.o random.o
	g++ -o b_plus_tree main.o read_file.o random.o

main.o: src/main.cpp src/headers.h
	g++ -c src/main.cpp
random.o: src/random.cpp src/random.h
	g++ -c src/random.cpp
read_file.o: src/read_file.cpp src/headers.h
	g++ -c src/read_file.cpp

clean:
	rm main.o random.o read_file.o