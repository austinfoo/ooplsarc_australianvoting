INCDIRS:=/Users/dmielke/Documents/oopl/trees/googletest/googletest/include
LIBDIRS:=/Users/dmielke/Documents/oopl/trees/googletest/googletest/make

FILES :=                                \
    .travis.yml                         \
    dijkstra-tests/EID-RunDijkstra.in   \
    dijkstra-tests/EID-RunDijkstra.out  \
    dijkstra-tests/EID-TestDijkstra.c++ \
    dijkstra-tests/EID-TestDijkstra.out \
    Dijkstra.c++                        \
    Dijkstra.h                          \
    Dijkstra.log                        \
    html                                \
    RunDijkstra.c++                     \
    RunDijkstra.in                      \
    RunDijkstra.out                     \
    TestDijkstra.c++                    \
    TestDijkstra.out                    \
    DijkstraBundle.c++

# Call gcc and gcov differently on Darwin
ifeq ($(shell uname), Darwin)
  CXX      := g++
  GCOV     := gcov
  VALGRIND := echo Valgrind not available on Darwin
else
  CXX      := g++-4.8
  GCOV     := gcov-4.8
  VALGRIND := valgrind
endif

CXXFLAGS   := -pedantic -std=c++11 -Wall -I$(INCDIRS)
LDFLAGS    := -lgtest -lgtest_main -pthread -L$(LIBDIRS)
GCOVFLAGS  := -fprofile-arcs -ftest-coverage
GPROF      := gprof
GPROFFLAGS := -pg

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f RunDijkstra
	rm -f RunDijkstra.tmp
	rm -f TestDijkstra
	rm -f TestDijkstra.tmp
	rm -f DijkstraBundle

config:
	git config -l

bundle:
	cat Dijkstra.h Dijkstra.c++ RunDijkstra.c++ | sed -e "s/#include \"Dijkstra.h\"//g" > DijkstraBundle.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) DijkstraBundle.c++ -o DijkstraBundle

scrub:
	make  clean
	rm -f  Dijkstra.log
	rm -rf dijkstra-tests
	rm -rf html
	rm -rf latex

status:
	make clean
	@echo
	git branch
	git remote -v
	git status

test: RunDijkstra.tmp TestDijkstra.tmp

RunDijkstra: Dijkstra.h Dijkstra.c++ RunDijkstra.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) Dijkstra.c++ RunDijkstra.c++ -o RunDijkstra

RunDijkstra.tmp: RunDijkstra
	./RunDijkstra < RunDijkstra.in > RunDijkstra.tmp
	diff RunDijkstra.tmp RunDijkstra.out

TestDijkstra: Dijkstra.h Dijkstra.c++ TestDijkstra.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) Dijkstra.c++ TestDijkstra.c++ -o TestDijkstra $(LDFLAGS)

TestDijkstra.tmp: TestDijkstra
	./TestDijkstra                                                    >  TestDijkstra.tmp 2>&1
	$(VALGRIND) ./TestDijkstra                                        >> TestDijkstra.tmp
	$(GCOV) -b Dijkstra.c++     | grep -A 5 "File 'Dijkstra.c++'"     >> TestDijkstra.tmp
	$(GCOV) -b TestDijkstra.c++ | grep -A 5 "File 'TestDijkstra.c++'" >> TestDijkstra.tmp
	cat TestDijkstra.tmp
