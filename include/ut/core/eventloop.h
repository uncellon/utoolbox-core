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

#ifndef UT_EVENT_LOOP_H
#define UT_EVENT_LOOP_H

#include "object.h"
#include "task.h"

#include <queue>

namespace UT {

class EventLoop : public Object {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    EventLoop();
    ~EventLoop();

    /**************************************************************************
     * Methods
     *************************************************************************/

    static EventLoop* getMainInstance();

    void pushTask(AbstractTask* task);

protected:
    /**************************************************************************
     * Methods (Protected)
     *************************************************************************/

    void loop();

    /**************************************************************************
     * Members
     *************************************************************************/

    static EventLoop* mMainInstance;
    static std::mutex mInstanceMutex;

    bool mRunning = false;
    std::condition_variable mCv;
    std::mutex mMutex;
    std::queue<AbstractTask *> mTasks;
    std::thread* mThreadLoop = nullptr;

}; // class EventLoop

} // namespace UT

#endif // UT_EVENT_LOOP_H