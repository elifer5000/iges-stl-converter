TESTS = test3
CPPFLAGS = -I../src -g2 -Wall 


all: $(TESTS)

test2: test2.o vmath.o
	g++ $(LDFLAGS) -o test2 test2.o vmath.o

test3: test3.o vmath.o
	g++ $(LDFLAGS) -lcppunit -ldl -o test3 test3.o vmath.o

vmath.o: ../src/vmath.cpp
	g++ $(CPPFLAGS) -c $< -o $@

%.o: %.cpp
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -f test2 test3 test2.o test3.o vmath.o
