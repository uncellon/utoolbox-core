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

#ifndef UT_EVENT_H
#define UT_EVENT_H

#include "abstractevent.h"
#include "delegate.h"
#include "eventdispatcher.h"
#include "eventloop.h"
#include "object.h"
#include "task.h"

#include <mutex>
#include <vector>

namespace UT {

template <class... TArgs>
class Event 
: public AbstractEvent {
    using TDelegate = Delegate<void(TArgs...)>;
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Event() = default;

    Event(const Event& other) {
        for (size_t i = 0; i < other.m_handlers.size(); ++i) {
            m_handlers.emplace_back((other->m_handlers)[i]);
        }
    }

    Event(Event&& other) {
        m_handlers = other.m_handlers;
        other.m_handlers.clear();
    }

    ~Event() {
        std::unique_lock lock(m_mutex);

        EventDispatcher::getInstance()->eventDestroyed(this);

        for (const auto& eventHandlerInfo : m_handlers) {
            delete eventHandlerInfo.delegate;
        }
        m_handlers.clear();
    }

    /**************************************************************************
     * Methods
     *************************************************************************/

    // Add function event handler
    void addEventHandler(Object* context, void (*func)(TArgs...)) {
        std::unique_lock lock(m_mutex);

        auto delegate = new TDelegate(func);

        if (indexOfHandler(delegate) != -1) {
            delete delegate;
            return;
        }

        m_handlers.emplace_back( EventHandlerInfo { delegate, context->eventLoop() });
        EventDispatcher::getInstance()->registerAttachment(this, context, delegate);
    }

    // Add method event handler
    template<class TObject>
    void addEventHandler(TObject* context, void (TObject::*method)(TArgs...)) {
        std::unique_lock lock(m_mutex);

        auto delegate = new TDelegate();
        delegate->bind(context, method);

        if (indexOfHandler(delegate) != -1) {
            delete delegate;
            return;
        }

        m_handlers.emplace_back( EventHandlerInfo { delegate, context->eventLoop() });
        EventDispatcher::getInstance()->registerAttachment(this, context, delegate);
    }

    // Add lambda event handler
    template<class TLambda>
    void addEventHandler(Object* context, TLambda&& lambda) {
        std::unique_lock lock(m_mutex);

        auto delegate = new TDelegate();
        delegate->bind(std::move(lambda));

        if (indexOfHandler(delegate) != -1) {
            delete delegate;
            return;
        }

        m_handlers.emplace_back( EventHandlerInfo { delegate, context->eventLoop() } );
        EventDispatcher::getInstance()->registerAttachment(this, context, delegate);
    }


    // Remove function event handler
    void removeEventHandler(Object* context, void (*func)(TArgs...)) {
        std::unique_lock lock(m_mutex);
        TDelegate delegate(func);
        unsafeRemoveEventHandler(&delegate);
    }

    // Remove method event handler
    template<class TObject>
    void removeEventHandler(TObject* object, void (TObject::*method)(TArgs...)) {
        std::unique_lock lock(m_mutex);
        TDelegate delegate(object, method);
        unsafeRemoveEventHandler(&delegate);
    }

    // Remove lambda event handler
    template<class TLambda>
    void removeEventHandler(Object* context, TLambda&& lambda) {
        std::unique_lock lock(m_mutex);
        TDelegate delegate(std::move(lambda));
        unsafeRemoveEventHandler(&delegate);
    }

    // Method for EventDispatcher
    virtual void removeEventHandler(AbstractDelegate* delegate) override {
        std::unique_lock lock(m_mutex);
        for (size_t i = 0; i < m_handlers.size(); ++i) {
            if (m_handlers[i].delegate != delegate) {
                continue;
            }
            delete m_handlers[i].delegate;
            m_handlers.erase(m_handlers.begin() + i);
            --i;
        }
    }

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    void operator()(TArgs... params, bool async = false) {
        std::unique_lock lock(m_mutex);

        for (size_t i = 0; i < m_handlers.size(); ++i) {
            auto task = new Task<void(TArgs&&...)>(m_handlers[i].delegate, params...);
            task->setSender(this);
            m_handlers[i].eventLoop->pushTask(task);
        }
    }

protected:
    struct EventHandlerInfo {
        TDelegate* delegate;
        EventLoop* eventLoop;
    };

    /**************************************************************************
     * Methods (Protected)
     *************************************************************************/

    inline int indexOfHandler(TDelegate* delegate) {
        for (size_t i = 0; i < m_handlers.size(); ++i) {
            if ( *(m_handlers[i].delegate) != *delegate ) {
                continue;
            }
            return i;
        }
        return -1;
    }

    inline void unsafeRemoveEventHandler(TDelegate* delegate) {
        int index = indexOfHandler(delegate);
        if (index == -1) {
            return;
        }
        EventDispatcher::getInstance()->removeAttachment(this, m_handlers[index].delegate);
        delete m_handlers[index].delegate;
        m_handlers.erase(m_handlers.begin() + index);
    }

    /**************************************************************************
     * Members
     *************************************************************************/

    std::mutex m_mutex;
    std::vector<EventHandlerInfo> m_handlers;
};

} // namespace UT

#endif // UT_EVENT_H
