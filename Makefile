b_plus_tree: main.o pre_processing.o
	g++ -o b_plus_tree main.o pre_processing.o

main.o: src/main.cpp src/headers.h
	g++ -c src/main.cpp
pre_processing.o: src/pre_processing.cpp src/pre_processing.h
	g++ -c src/pre_processing.cpp

clean:
	rm main.o pre_processing.o