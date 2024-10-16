/******************************************************************************
 *
 * Copyright (C) 2023 Dmitry Plastinin
 * Contact: uncellon@yandex.ru, uncellon@gmail.com, uncellon@mail.ru
 *
 * This file is part of the UToolbox Core library.
 *
 * UToolbox Core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as pubblished by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * UToolbox Core is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more
 * details
 *
 * You should have received a copy of the GNU Lesset General Public License
 * along with UToolbox Core. If not, see <https://www.gnu.org/licenses/>.
 *
 *****************************************************************************/

#include "eventdispatcher.h"
#include "eventloop.h"

namespace UT {

/******************************************************************************
 * Constructors / Destructors
 *****************************************************************************/

Object::Object(EventLoop* eventLoop)
: mEventLoop(eventLoop) { }

Object::Object(Object* parent) {
    if (!parent) {
        mEventLoop = EventLoop::getMainInstance();
        return;
    }
    if (!parent->eventLoop()) {
        mEventLoop = EventLoop::getMainInstance();
        return;
    }
    mEventLoop = parent->eventLoop();
}

Object::~Object() {
    EventDispatcher::getInstance()->clean(this);
}

} // namespace UT