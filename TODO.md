

# TW Mailer

## Recherche
+ Threads
+ Forks
+ Parallelism & Concurrency
+ Composite Pattern

## Module

### Server

#### IItem
**Methods**:
+ string toString()
+ Map toMap()


ISender : IITem
IReceiver : IITem
ISubject : IITem
IContent : IITem

#### IMessage : IItem
**Summary**:
+ beinhaltet alle Komponenten einer Mail
**Fields**:
+ ISender Sender
+ IReceiver Receiver
+ ISubject Subject
+ IContent content (die message selbst)

#### ISubject : IItem

#### ISerializer
**Summary**
+ serialisiert Message-Object, bzw. deserialisiert string und returned
**Methods**:
+ string Serialize(Message msgObj)
+ Message Deserialize(string msgString)
**Schema**
    ```
    SEND\n
    <Sender>\n
    <Receiver>\n
    <Subject (max. 80 chars)>\n
    <message (multi-line; no length restrictions)\n>
    .\n
    ```
#### ITokenParser
**Summary**:
+ erkennt Command (z.B. SEND, LIST, ...) und speichert inhalt in tokens
+ erkennt <Tags> und speichert inhalt in tokens
**Fields**:
+ Dictionary<string, string> tokens
**Methods**:
+ Map<string, string> parse(string)
**Schema**:
+ `{"Command": "SEND", "Receiver": "Max", "Sender": "Sebi"}`
+ 
#### Router
**Summary**
 + Routet Clientrequests an zuständigen Controller weiter
**Fields**:
**Methods**:
+ void HandleRequest(string request)


#### MessageController
**Summary**
+ Handles the specific requests reveiced from the router
**Methods**:
+ saveMessage(string messageSender, string, message)
+ getMessages()


#### IFileHandler
**Summary**
+ schreibt messages in files und liest sie aus
**Methods**:
+ string readFile(string path)
+ writeFile(string path, string content)

## Client

---

## Commands:
- SEND
- LIST
- READ
- DEL
- QUIT
## Protocol specs
### SEND
#### Request
```
SEND\n
<Sender>\n
<Receiver>\n
<Subject (max. 80 chars)>\n
<message (multi-line; no length restrictions)\n>
.\n
```
+ final "." ends command


#### Response
+ `OK\n`
+ `ERR\n`
+ User folder in `/var/spool/mail/$USER/<receiver>/` für receiver anlegen
    falls er noch nicht existiert
+ Content der Mail in eigenes File unter `/var/spool/mail/$USER/<receiver>/` speichern


### LIST
+ gibt die Subjects aller Nachrichten (nummeriert) aus dem Posteingang eines bestimmten Users aus


#### Request
```
LIST\n 
<Username>\n 
```

#### Reponse
```
<count of messages of the user (0 if no message or user unknown)>\n
<subject 1>\n
<subject 2>\n
…
<subject N>\n 
```

### READ
```
READ\n
<Username>\n
<Message-Number>\n
```

#### Response
```
OK\n
<complete message content (as defined in SEND)>\n
```
**or**
```
ERR\n 
```

### DEL
```
DEL\n
<Username>\n
<Message-Number>\n
```

#### Response
```
OK\n
```
or
```
ERR\n 
```
### QUIT
```
QUIT\n
```
(No Server Response)



# Additional TW MAILER PRO Features

### LOGIN
### 


ICommandHandler
+ MessageHandler
+ ListHandler
+ ...


CommandRouter/CommandMapper
-> ruft zuständigen ICommandHandler auf und delegiert
Anfrage an diesen


### Request
- enum COMMAND
- int socket-Id

### Message : IItem
- SENDER
- RECEIVER
- SUBJECT
- MESSAGE

### List
- USERNAME

Read
- USERNAME
- MESSAGE_NR

Delete
- USERNAME
- MESSAGE_NR


_SERVER_
response = router.handleRequest(socketID, buffer)
send(response.socketID, response.toString());

return controller.ControllerMethod()

---

_Controller_
Response::Send(payload)


_Response::Send_
send(payload.socketID, payload.toString())

### IRequest
**Summary**:
+ Server gibt Router socket-ID, buffer, buffer-size weiter
+ Router erstellt IRequest-Instanz
**Fields**:
+ string Command (zB "SEND")
+ int socket-ID
+ IItem payload


### IItem
**Methode**
+ public string toString()

### Response
+ IItem payload
  + IItem payload hat methode toString()
+ socket-ID

---

SERVER:
Router:
_REQUEST_
- erstellt IRequest Instanz
- ruft zuständige MessageController-Methode auf
  - gibt Nachrichten weiter an MessageController
- hat Parser instanz (deserialisiert Request)

_RESPONSE_
- ?? erhält IResponse Instanz von Controller (oder gleich als string?)
- 


MessageController
**Methods**:
- SendMessage(SendRequest)
- ListMessages(ListRequest)
- ReadMessage(ReadRequest)
- DeleteMessages(DeleteRequest)


Utils:
- IParser



CLIENT:
