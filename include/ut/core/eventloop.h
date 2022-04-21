/******************************************************************************
 * 
 * Copyright (C) 2021 Dmitry Plastinin
 * Contact: uncellon@yandex.ru, uncellon@gmail.com, uncellon@mail.ru
 * 
 * This file is part of the UToolbox Core library.
 * 
 * UToolbox Core is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as pubblished by the
 * Free Software Foundation, either version 3 of the License, or (at your 
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

#include "delegate.h"
#include "object.h"
#include "task.h"

#include <queue>
#include <thread>

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

    void pushTask(AbstractTask* task);

protected:
    /**************************************************************************
     * Methods (Protected)
     *************************************************************************/

    void loop();

    /**************************************************************************
     * Members
     *************************************************************************/

    bool m_running = false;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::queue<AbstractTask *> m_tasks;
    std::thread* m_threadLoop = nullptr;
};

} // namespace UT

#endif // UT_EVENT_LOOP_H