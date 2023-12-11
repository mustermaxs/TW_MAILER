CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

TARGET = myfind
SRCS = main.cpp src/RecursiveFileHandler.cpp src/IFileHandler.cpp src/Message.cpp src/Request.cpp src/MessageHandler.cpp src/List.cpp Client/Parser/Parser.cpp src/Utils.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET) $(OBJS)