CXX = g++
CXXFLAGS = -std=c++17 -Wall -g


SRV_TARGET = ./bin/twmailer 
CLIENT_TARGET = ./bin/client

SRV_SRCS = Server/server.cpp 
CLIENT_SRC = Client/client.cpp

SRV_OBJS = $(SRV_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRC:.cpp=.o)

all: $(SRV_TARGET) $(CLIENT_TARGET)

$(SRV_TARGET): $(SRV_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%/Server/%.o: %/Server/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
%/Client/%.o: %/Client/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(SRV_TARGET) $(OBJS)
	rm -f $(CLIENT_TARGET) $(OBJS)

