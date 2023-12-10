#include "./headers/Controller.h"
#include "./headers/Message.h"

Controller::Controller() {

}

void Controller::ReceiveMessage(Request req) {

    Message requestMessage = req.getMessage();
    std::string username = requestMessage.getReceiver();

    bool messageCreated = messageHandler.saveMessage(username, requestMessage);

    std::string resBody = "";

    if (!messageCreated) {
        resBody = "ERR\n";
    } else {
        resBody = "OK\n";
    }
    
    std::cout << resBody << std::endl;
    
    // sendResponse(req.socketId, resBody);

}

void Controller::ListMessages(Request req) {

    Message requestMessage = req.getMessage();
    std::string username = requestMessage.getReceiver();

    std::vector<Message> messages = messageHandler.getMessagesByUsername(username);

    int messagesCount = messages.size();

    std::string resBody = "";

    resBody += messagesCount + "\n";

    if (messagesCount) {
        
        for(const Message& message : messages)
        {
            resBody += message.getSubject();
        }
        
    }

    // sendResponse(req.socketId, resBody);
}

void Controller::ReadMessage(Request req) {

    Message requestMessage = req.getMessage();
    std::string username = requestMessage.getReceiver();
    int messageNumber;
    
    std::string resBody =  "";


 // sendResponse(req.socketId, resBody);
}

void Controller::DeleteMessages(Request req) {

}

void Controller::Quit() {

}
