CXX = g++
CXXFLAGS = -std=c++17 -Wall -g


SRV_TARGET = ./bin/twmailer-server
CLIENT_TARGET = ./bin/twmailer-client
LIBS = -lldap -llber

SRV_SRCS = Server/server.cpp src/Router.cpp src/Message.cpp src/MessageHandler.cpp src/Utils.cpp src/Controller.cpp src/Request.cpp src/ConnectionConfig.cpp src/IMessage.cpp src/SocketServer.cpp src/ISocketHandler.cpp src/IFileHandler.cpp src/FileHandler.cpp src/LdapHandler.cpp src/LoginMessage.cpp src/Response.cpp src/LdapHandler.cpp
CLIENT_SRC = Client/client.cpp Client/Parser/Parser.cpp src/Utils.cpp src/ConnectionConfig.cpp src/IMessage.cpp src/SocketClient.cpp src/ISocketHandler.cpp src/IFileHandler.cpp src/FileHandler.cpp

SRV_OBJS = $(SRV_SRCS:.cpp=.o)
CLIENT_OBJS = $(CLIENT_SRC:.cpp=.o)

all: $(SRV_TARGET) $(CLIENT_TARGET)

$(SRV_TARGET): $(SRV_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%/Server/%.o: %/Server/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
%/Client/%.o: %/Client/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(SRV_TARGET) $(CLIENT_TARGET) $(SRV_OBJS) $(CLIENT_OBJS)