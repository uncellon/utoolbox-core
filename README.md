# Uncellon's Toolbox Core

![UToolbox Logo](logo.png)

- [Description](#description)
- [Prerequisites](#prerequisites)
- [Examples](#examples)
    - [Event with function handler based on default event loop](#event-with-function-handler-based-on-default-event-loop)
    - [Event with function handler](#event-with-function-handler)
    - [Event with method handler](#event-with-method-handler)
    - [Event with lambda handler](#event-with-lambda-handler)
- [License](#license)

## Description
Main goal of UToolbox Core is provide an easy thread-safe event-driven approach to application development.

It should be understood that this approach to application development leads to multithreading. You will have at least 2 threads: your main thread and the event handling thread (or event loop in another words). In fact, calling (or "fire" in C#) an event is adding the appropriate tasks to the event processing thread. Perhaps this approach complicates the architecture of the application, but instead you should not worry about blocking your main thread.

Every object that can handle events must inherit from `UT::Object`. This requirement is due to two reasons:

1. you don't have to worry about unsubscribing from events if the object with handler is destroyed;
2. and each `UT::Object` belongs to a specific event handling thread and is serviced accordingly by that same thread.

## Prerequisites

- C++17 or higher
- CMake >= 3.16

## Examples

### Event with function handler based on default event loop

This library implements the default event processing thread. It is based on the singleton pattern. If you don't want to manage your event loops, or you don't need more than one such thread, you can safely trust this thing.

```cpp
#include <iostream>
#include <ut/core/event.h>

bool called = false;

class EventHolder : public UT::Object {
public:
    EventHolder(UT::Object* parent = nullptr) : UT::Object(parent) { }

    void writeMessage(const std::string& message) {
        onMessageWritten(message);
    }

    UT::Event<const std::string> onMessageWritten;

}; // class EventHolder

void someUsefulFunction(const std::string message) {
    std::cout << message << std::endl;
    called = true;
}

int main(int argc, char* argv[]) {
    EventHolder eh;
    eh.onMessageWritten.addEventHandler(UT::EventLoop::getMainInstance(), someUsefulFunction);

    eh.writeMessage("Hello, event-driven World!");

    while (!called) { } // Just "spinlock" to sync output. Your main() function can execute faster than the message is displayed on the screen.

    return 0;
}
```

### Event with function handler
```cpp
#include <iostream>
#include <ut/core/event.h>

bool called = false;

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
    called = true;
}

int main(int argc, char* argv[]) {
    UT::EventLoop mainEventLoop;

    EventHolder eh(&mainEventLoop);
    eh.onMessageWritten.addEventHandler(&mainEventLoop, stdoutHandler);

    eh.writeMessage("Hello, World!");

    while (!called) { } // Just "spinlock" to sync output. Your main() function can execute faster than the message is displayed on the screen.

    return 0;
}
```

### Event with method handler
```cpp
#include <iostream>
#include <ut/core/event.h>

bool called = false;

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
        called = true;
    }

}; // class EventHandler

int main(int argc, char* argv[]) {
    // UT::EventLoop::getMainInstance() provides default singleton Event Loop
    UT::EventLoop mainEventLoop;

    EventHolder eh(&mainEventLoop);
    EventHandler ehandler(&mainEventLoop); // Or EventHandler ehandler(&eh)

    eh.onMessageWritten.addEventHandler(&ehandler, &EventHandler::messageHandler);

    eh.writeMessage("Hello, World!");

    while (!called) { } // Just "spinlock" to sync output. Your main() function can execute faster than the message is displayed on the screen.

    return 0;

}
```

### Event with lambda handler
```cpp
#include <iostream>
#include <ut/core/event.h>

bool called = false;

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
        std::cout << "Message received: " << message << std::endl;
        called = true;
    });

    eh.writeMessage("Hello, World!");

    while (!called) { } // Just "spinlock" to sync output. Your main() function can execute faster than the message is displayed on the screen.

    return 0;
}
```

## License

<img align="right" src="https://www.gnu.org/graphics/lgplv3-with-text-154x68.png">

The library is licensed under [GNU Lesser General Public License 3.0](https://www.gnu.org/licenses/lgpl-3.0.txt):

Copyright © 2023 Dmitry Plastinin

UToolbox Core is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as pubblished by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

UToolbox Core is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more details