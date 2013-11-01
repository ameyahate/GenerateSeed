CXX = g++ 
CXXFLAGS = -Wall -g

main: main.o SimpleSource.o SimpleNode.o CalcSeed.o

	$(CXX) $(CXXFLAGS) -o genSeeds main.o SimpleSource.o SimpleNode.o CalcSeed.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

SimpleSource.o: SimpleSource.h SimpleSource.cpp
	$(CXX) $(CXXFLAGS) -c SimpleSource.cpp

SimpleNode.o: SimpleNode.h SimpleNode.cpp
	$(CXX) $(CXXFLAGS) -c SimpleNode.cpp

CalcSeed.o: CalcSeed.h CalcSeed.cpp
	$(CXX) $(CXXFLAGS) -c CalcSeed.cpp

clean:
	rm -rf *o genSeeds
