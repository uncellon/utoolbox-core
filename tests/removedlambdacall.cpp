#include <ut/core/event.h>
#include <iostream>
#include <unistd.h>

using namespace UT;
using namespace std;

unsigned int counter = 0;

class A : public Object {
public:
    A(EventLoop* loop) : Object(loop) { }

    void fireEvent() { onTriggered(); }

    Event<> onTriggered;
};

class B : public Object {
public:
    B(EventLoop* loop) : Object(loop) { }

    void increaseCounter() { ++counter; }
};

int main(int argc, char* argv[]) {
    EventLoop mainLoop;

    auto a = new A(&mainLoop);
    auto b = new B(&mainLoop);

    a->fireEvent();
    a->onTriggered.addEventHandler(b, [b] () {
        b->increaseCounter();
    });
    a->fireEvent();

    sleep(1);

    delete b;
    a->fireEvent();

    if (counter != 1) {
        std::cout << "Counter is " << counter << std::endl;
        return 1;
    }

    delete a;
    return 0;
}