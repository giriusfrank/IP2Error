CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -static-libgcc -static-libstdc++

skiplist.o: skiplist.cpp skiplist.h
	$(CXX) $(CXXFLAGS) -c skiplist.cpp -o skiplist.o

demo: demo.cpp skiplist.o
	$(CXX) $(CXXFLAGS) demo.cpp skiplist.o -o demo.exe

run_demo: demo
	demo.exe

test: test.cpp skiplist.o
	$(CXX) $(CXXFLAGS) test.cpp skiplist.o -o test.exe

run_test: test
	test.exe

clean:
	del /Q *.o demo.exe test.exe log.txt

rebuild: clean demo test