CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

TARGET = myfind
SRCS = main.cpp FileScanner.cpp IFileScanner.cpp utils.cpp IPrinter.cpp Printer.cpp CommandLineParser.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)
