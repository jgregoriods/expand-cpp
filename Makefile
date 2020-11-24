expand: main.o agent.o grid.o model.o date.o
	g++ main.o agent.o grid.o model.o date.o -o expand

main.o: main.cpp
	g++ -c main.cpp --std=c++17

agent.o: agent.cpp
	g++ -c agent.cpp --std=c++17

grid.o: grid.cpp
	g++ -c grid.cpp --std=c++17

model.o: model.cpp
	g++ -c model.cpp --std=c++17

date.o: date.cpp
	g++ -c date.cpp --std=c++17

clean:
	rm *.o expand