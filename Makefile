args = -lX11 -std=c++20 -O3

CXX = gcc # Flag for implicit rules
CXXFLAGS = -g -I ./ $(args) -std=c++20 # Flag for implicit rules. Turn on debug info

objects = main.o

build: $(objects)
	g++ $(objects) $(args) -o ./Build/main

rebuild: clean build

# These files compile via implicit rules
# Syntax - targets ...: target-pattern: prereq-patterns ...
# In the case of the first target, foo.o, the target-pattern matches foo.o and sets the "stem" to be "foo".
# It then replaces the '%' in prereq-patterns with that stem
$(objects): %.o: %.cpp

clean:
	rm -f *.o all
