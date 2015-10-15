INCDIRS:=/Users/dmielke/Documents/oopl/trees/googletest/googletest/include
LIBDIRS:=/Users/dmielke/Documents/oopl/trees/googletest/googletest/make

FILES :=                                        \
    .travis.yml                                 \
    dijkstra-tests/EID-RunAustralianVoting.in   \
    dijkstra-tests/EID-RunAustralianVoting.out  \
    dijkstra-tests/EID-TestAustralianVoting.c++ \
    dijkstra-tests/EID-TestAustralianVoting.out \
    AustralianVoting.c++                        \
    AustralianVoting.h                          \
    AustralianVoting.log                        \
    html                                        \
    RunAustralianVoting.c++                     \
    RunAustralianVoting.in                      \
    RunAustralianVoting.out                     \
    TestAustralianVoting.c++                    \
    TestAustralianVoting.out                    \
    AustralianVotingBundle.c++

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
	rm -f RunAustralianVoting
	rm -f RunAustralianVoting.tmp
	rm -f TestAustralianVoting
	rm -f TestAustralianVoting.tmp
	rm -f AustralianVotingBundle

config:
	git config -l

bundle:
	cat AustralianVoting.h AustralianVoting.c++ RunAustralianVoting.c++ | sed -e "s/#include \"AustralianVoting.h\"//g" > AustralianVotingBundle.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) AustralianVotingBundle.c++ -o AustralianVotingBundle

scrub:
	make  clean
	rm -f  AustralianVoting.log
	rm -rf dijkstra-tests
	rm -rf html
	rm -rf latex

status:
	make clean
	@echo
	git branch
	git remote -v
	git status

test: RunAustralianVoting.tmp TestAustralianVoting.tmp

RunAustralianVoting: AustralianVoting.h AustralianVoting.c++ RunAustralianVoting.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) AustralianVoting.c++ RunAustralianVoting.c++ -o RunAustralianVoting

RunAustralianVoting.tmp: RunAustralianVoting
	./RunAustralianVoting < RunAustralianVoting.in > RunAustralianVoting.tmp
	diff RunAustralianVoting.tmp RunAustralianVoting.out

TestAustralianVoting: AustralianVoting.h AustralianVoting.c++ TestAustralianVoting.c++
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) AustralianVoting.c++ TestAustralianVoting.c++ -o TestAustralianVoting $(LDFLAGS)

TestAustralianVoting.tmp: TestAustralianVoting
	./TestAustralianVoting                                                            >  TestAustralianVoting.tmp 2>&1
	$(VALGRIND) ./TestAustralianVoti        ng                                        >> TestAustralianVoting.tmp
	$(GCOV) -b AustralianVoting.c++     | grep -A 5 "File 'AustralianVoting.c++'"     >> TestAustralianVoting.tmp
	$(GCOV) -b TestAustralianVoting.c++ | grep -A 5 "File 'TestAustralianVoting.c++'" >> TestAustralianVoting.tmp
	cat TestAustralianVoting.tmp
