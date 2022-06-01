# UToolbox Core

![UToolbox Logo](ut-logo.svg)

- [Description](#description)
- [Examples](#examples)
    - [Event with function handler](#event-with-function-handler)
    - [Event with method handler](#event-with-method-handler)
    - [Event with lambda handler](#event-with-lambda-handler)

## Description
UToolbox Core provides an event-driven approach to application development. It's based on event loops - independent thread that process various user tasks in order. Every custom class that needs to implement events must iherit from the UT::Object class. The UT::Object constructor takes one argument, a reference to another UT::Object object. In other words, the parent object. Root UT::Object must be an event loop. Thus, all objects associated with the event loop will push tasks in this event loop. When the event loop processes a task, it calls the event handlers associated with that event. Event handler is a wrapped object that contains function, method or lambda.

## Examples

### Event with function handler
```cpp
#include <iostream>
#include <unistd.h>
#include <ut/core/event.h>
#include <ut/core/eventloop.h>
#include <ut/core/object.h>

class EventHolder : public UT::Object {
public:
    EventHolder(UT::Object* parent = nullptr) : UT::Object(parent) { }

    void writeMessage(const std::string& message) {
        onMessageWritten(message);
    }

    UT::Event<std::string> onMessageWritten;
}; // class EventHolder

void stdoutHandler(std::string message) {
    std::cout << "Message received: " << message << std::endl;
}

int main(int argc, char* argv[]) {
    UT::EventLoop mainEventLoop;

    EventHolder eh(&mainEventLoop);
    eh.onMessageWritten.addEventHandler(&mainEventLoop, stdoutHandler);

    eh.writeMessage("Hello, World!");

    sleep(1); // Delay for message output, event loop is another thread.
    return 0;
}
```

### Event with method handler
```cpp
#include <iostream>
#include <unistd.h>
#include <ut/core/event.h>
#include <ut/core/eventloop.h>
#include <ut/core/object.h>

class EventHolder : public UT::Object {
public:
    EventHolder(UT::Object* parent = nullptr) : UT::Object(parent) { }

    void writeMessage(const std::string& message) {
        onMessageWritten(message);
    }

    UT::Event<std::string> onMessageWritten;
}; // class EventHolder

class EventHandler : public UT::Object {
public:
    EventHandler(UT::Object* parent = nullptr) : UT::Object(parent) { }

    void messageHandler(std::string message) {
        std::cout << "Message received: " << message << std::endl;
    }
}; // class EventHandler

int main(int argc, char* argv[]) {
    UT::EventLoop mainEventLoop;

    EventHolder eh(&mainEventLoop);
    EventHandler ehandler(&mainEventLoop); // Or EventHandler ehandler(&eh)

    eh.onMessageWritten.addEventHandler(&ehandler, &EventHandler::messageHandler);

    eh.writeMessage("Hello, World!");

    sleep(1); // Delay for message output, event loop is another thread.
    return 0;
}
```

### Event with lambda handler
```cpp
#include <iostream>
#include <unistd.h>
#include <ut/core/event.h>
#include <ut/core/eventloop.h>
#include <ut/core/object.h>

class EventHolder : public UT::Object {
public:
    EventHolder(UT::Object* parent = nullptr) : UT::Object(parent) { }

    void writeMessage(const std::string& message) {
        onMessageWritten(message);
    }

    UT::Event<std::string> onMessageWritten;
}; // class EventHolder

int main(int argc, char* argv[]) {
    UT::EventLoop mainEventLoop;

    EventHolder eh(&mainEventLoop);

    eh.onMessageWritten.addEventHandler(&mainEventLoop, [] (std::string message) {
        std::cout << "Message received:" << message << std::endl;
    });

    eh.writeMessage("Hello, World!");

    sleep(1); // Delay for message output, event loop is another thread.
    return 0;
}
```