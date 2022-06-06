#include <ut/core/event.h>
#include <iostream>
#include <unistd.h>

using namespace UT;
using namespace std;

unsigned int counter = 0;

class A : public Object {
public:
    A(Object* parent = nullptr) : Object(parent) { }

    void fireEvent() { onTriggered(); }
    
    Event<> onTriggered;
};

class B : public Object {
public:
    B(Object* parent = nullptr) : Object(parent) { }

    void triggerHandler() { ++counter; }
};

int main(int argc, char* argv[]) {
    // Create objects with default event loop
    auto a = new A();
    auto b = new B();

    a->fireEvent();
    a->onTriggered.addEventHandler(b, &B::triggerHandler);
    a->fireEvent();

    sleep(1);
    
    a->onTriggered.removeEventHandler(b, &B::triggerHandler);

    delete b;
    a->fireEvent();

    if (counter != 1) {
        return 1;
    }
    
    delete a;
    return 0;
}