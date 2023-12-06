CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

TARGET = myfind
SRCS = main.cpp FileHandler/RecursiveFileHandler.cpp FileHandler/IFileHandler.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)