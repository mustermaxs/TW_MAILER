#ifndef CONTROLLER_H
#define CONTROLLER_H

class IFileHandler;

class Controller
{
    private:
        IFileHandler fileHandler;


    public:
    void SendMessage() {}
    void ListMessages() {}
    void ReadMessage() {}
    void DeleteMessages() {}



}

#endif