#include "ut/core/delegate.h"
#include "ut/core/event.h"
#include "ut/core/eventloop.h"
#include "ut/core/object.h"

#include <iostream>
#include <string>
#include <unistd.h>

void printFunction(const std::string& message) {
    std::cout << "Function called: " << message << std::endl;
}

class DerivedObject1 : public UT::Object {
public:
    DerivedObject1(UT::Object* parent = nullptr) : UT::Object(parent) { }

    UT::Event<const std::string&> onMessage;
};

class DerivedObject2 : public UT::Object {
public:
    DerivedObject2(UT::Object* parent = nullptr) : UT::Object(parent) { }

    void messageHandler(const std::string& message) {
        std::cout << "Message received: " << message << std::endl;
    }
};

class DerivedObject3 : public UT::Object {
public:
    DerivedObject3(UT::Object* parent = nullptr) : UT::Object(parent) { }

    UT::Event<void*, int, int> onMessage;
};

class DerivedObject4 : public UT::Object {
public:
    DerivedObject4(UT::Object* parent = nullptr) : UT::Object(parent) { }

    void messageHandler(void* a, int b, int c) {
        std::cout << "Message received: " << a << " " << b << " " << c << std::endl;
    }
};

int main(int argc, char* argv[]) {
    UT::EventLoop mainLoop;
    UT::EventLoop secondLoop;

    auto derivedObject1 = new DerivedObject1(&mainLoop);
    auto derivedObject2 = new DerivedObject2(&secondLoop);
    auto derivedObject22 = new DerivedObject2(derivedObject1);

    derivedObject1->onMessage.addEventHandler(derivedObject2, &DerivedObject2::messageHandler);
    derivedObject1->onMessage.addEventHandler(derivedObject22, &DerivedObject2::messageHandler);
    derivedObject1->onMessage.removeEventHandler(derivedObject2, &DerivedObject2::messageHandler);

    derivedObject1->onMessage.addEventHandler(derivedObject2, printFunction);
    derivedObject1->onMessage.removeEventHandler(derivedObject2, printFunction);
    
    derivedObject1->onMessage.addEventHandler(derivedObject2, [] (const std::string& message) {
        std::cout << "Lambda print: " << message << std::endl;
    });

    derivedObject1->onMessage("Hello!");

    sleep(1);

    delete derivedObject22;
    delete derivedObject2;

    derivedObject1->onMessage("Hey!");

    delete derivedObject1;

    DerivedObject3 obj3(&mainLoop);
    DerivedObject4 obj4(&mainLoop);

    obj3.onMessage.addEventHandler(&obj4, &DerivedObject4::messageHandler);
    obj3.onMessage((void*)"some", 1, 2);

    sleep(1);

    return 0;
}