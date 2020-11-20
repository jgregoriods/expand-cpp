expand: main.o agent.o grid.o model.o
	g++ main.o agent.o grid.o model.o -o expand

main.o: main.cpp
	g++ -c main.cpp

agent.o: agent.cpp
	g++ -c agent.cpp

grid.o: grid.cpp
	g++ -c grid.cpp

model.o: model.cpp
	g++ -c model.cpp

clean:
	rm *.o expand