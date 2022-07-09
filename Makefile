.default: all

all: solve

clean:
	rm -f solve *.o

solve: main.o Node.o NodeList.o PathPlanner.o Types.o
	g++ -std=c++14 -g -O -o $@ $^

%.o: %.cpp
	g++ -std=c++14 -g -O -c $^
