#include "ut/core/event.h"
#include "ut/core/delegate.h"
#include "ut/core/eventloop.h"
#include "ut/core/object.h"

#include <iostream>
#include <string>
#include <unistd.h>

void printFunction(const std::string &message) {
    std::cout << "Function called: " << message << std::endl;
}

class MethodHolder : public UT::Object {
public:
    void printMethod(const std::string &message) {
        std::cout << "Method called: " << message << std::endl;
    }
};

class EventHolder {
public:
    UT::Event<const std::string &> onCall;
};

class DerivedObject1 : public UT::Object {
public:
    DerivedObject1(UT::Object* parent = nullptr) : UT::Object(parent) { }

    UT::Event<const std::string &> onMessage;
};

class DerivedObject2 : public UT::Object {
public:
    DerivedObject2(UT::Object* parent = nullptr) : UT::Object(parent) { }

    void messageHandler(const std::string &a) {
        std::cout << "Message received: " << a << std::endl;
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
        std::cout << "Message received: " << a << b << c << std::endl;
    }
};

int main(int argc, char *argv[]) {
    // UT::Delegate<void(const std::string &)> nd1, nd2;
    // nd1.bind(printFunction);
    // nd1("Calling first delegate");


    // MethodHolder mh;
    // nd1.bind(&mh, &MethodHolder::printMethod);
    // nd1("Calling first delegate");

    // nd1.bind([&mh] (const std::string &message) {
    //     std::cout << "Lambda called: " << message << std::endl;
    // });
    // nd1("Calling first delegate");

    // nd2 = nd1;
    // nd2("Calling second delegate");

    // EventHolder eh;
    // eh.onCall.addEventHandler([] (const std::string &message) {
    //     std::cout << "Lambda called: " << message << std::endl;
    // });
    // eh.onCall.addEventHandler(printFunction);
    // eh.onCall.addEventHandler(&mh, &MethodHolder::printMethod);

    // eh.onCall("Calling onCall(...) event");

    // eh.onCall.removeEventHandler(printFunction);
    // eh.onCall.removeEventHandler(&mh, &MethodHolder::printMethod);

    // eh.onCall("Calling onCall(...) event after removed some event handlers");




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
    obj3.onMessage((void*)"dsad", 1, 2);

    sleep(1);



    return 0;
}