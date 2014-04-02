all: main.cpp
	$(CXX) $(CXXFLAGS) -Wall -Wextra -g3 -o main main.cpp -lboost_regex $(LDFLAGS)
