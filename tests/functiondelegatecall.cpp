#include <ut/core/delegate.h>

using namespace UT;

bool result = false;

void setResult(bool value) {
    result = value;
}

int main(int argc, char *argv[]) {
    Delegate<void(bool)> delegate(setResult);
    delegate(true);
    if (!result) {
        return 1;
    }
    return 0;
}