b_plus_tree: main.o pre_processing.o b_node.o b_tree.o
	g++ -o b_plus_tree main.o pre_processing.o b_node.o b_tree.o

main.o: src/main.cpp src/headers.h
	g++ -c src/main.cpp
pre_processing.o: src/pre_processing.cpp src/pre_processing.h
	g++ -c src/pre_processing.cpp
b_node.o: src/b_node.cpp src/b_node.h
	g++ -c src/b_node.cpp
b_tree.o: src/b_tree.cpp src/b_tree.h
	g++ -c src/b_tree.cpp

clean:
	rm main.o pre_processing.o b_node.o b_tree.o