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

EventLoop* EventLoop::mMainInstance = nullptr;
std::mutex EventLoop::mInstanceMutex;

/******************************************************************************
 * Constructors / Destructors
 *****************************************************************************/

EventLoop::EventLoop()
: Object(this) {
    mRunning = true;
    mThreadLoop = new std::thread(&EventLoop::loop, this);
}

EventLoop::~EventLoop() {
    {
        std::unique_lock lock(mMutex);
        mRunning = false;
        mCv.notify_one();
    }
    mThreadLoop->join();
}

/******************************************************************************
 * Methods
 *****************************************************************************/

EventLoop* EventLoop::getMainInstance() {
    std::unique_lock lock(mInstanceMutex);
    if (!mMainInstance) {
        mMainInstance = new EventLoop();
    }
    return mMainInstance;
}

void EventLoop::pushTask(AbstractTask* task) {
    std::unique_lock lock(mMutex);
    mTasks.emplace(task);
    mCv.notify_one();
}

/******************************************************************************
 * Methods (Protected)
 *****************************************************************************/

void EventLoop::loop() {
    auto eventDispatcher = EventDispatcher::getInstance();
    std::unique_lock lock(mMutex);

    while (mRunning) {
        if (mTasks.empty()) {
            mCv.wait(lock);
            continue;
        }

        auto task = mTasks.front();
        mTasks.pop();
        lock.unlock();

        eventDispatcher->lockAttachments();
        if (eventDispatcher->attachmentValid(task->sender(), task->delegate())) {
            task->execute();
        }
        delete task;
        eventDispatcher->unlockAttachments();

        lock.lock();
    }
}

} // namespace UT