CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic
SOURCES = main.cpp Config.cpp RequestGenerator.cpp WebServer.cpp LoadBalancer.cpp
OBJECTS = $(SOURCES:.cpp=.o)

.PHONY: all clean docs test
all: loadbalancer

loadbalancer: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(OBJECTS:.o=.d)

docs:
	doxygen Doxyfile

test: loadbalancer
	$(CXX) $(CXXFLAGS) -I. tests/test.cpp Config.cpp WebServer.cpp -o tests/test_runner
	./tests/test_runner
	python3 tests/integration.py

clean:
	rm -f loadbalancer $(OBJECTS) $(OBJECTS:.o=.d) tests/test_runner
