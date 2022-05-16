/******************************************************************************
 * 
 * Copyright (C) 2022 Dmitry Plastinin
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

#ifndef UT_EVENT_DISPATCHER_H
#define UT_EVENT_DISPATCHER_H

#include <shared_mutex>
#include <vector>

namespace UT {

class AbstractDelegate;
class AbstractEvent;
class Object;

class EventDispatcher {
public:
    /**************************************************************************
     * Methods 
     *************************************************************************/

    static EventDispatcher* getInstance();

    void registerAttachment(AbstractEvent* event, Object* object, AbstractDelegate* delegate);
    void removeAttachment(AbstractEvent* event, AbstractDelegate* delegate);

    void eventDestroyed(AbstractEvent* event);
    void clean(Object* object);

    bool attachmentValid(AbstractEvent* event, AbstractDelegate* delegate);

    inline void lockAttachments() { m_mutex.lock_shared(); }
    inline void unlockAttachments() { m_mutex.unlock_shared(); }

protected:
    struct DispatcherInfo {
        AbstractEvent* event;
        Object* object;
        AbstractDelegate* delegate;
    };

    /**************************************************************************
     * Members
     *************************************************************************/

    static std::shared_mutex m_mutex;
    static EventDispatcher* m_instance;

    std::mutex m_vectorMutex;
    std::vector<DispatcherInfo> m_attachments;

private:
    /**************************************************************************
     * Constructors / Destructors (Private)
     *************************************************************************/

    EventDispatcher() = default;    
}; // class EventDispatcher

} // Hlk

#endif // UT_EVENT_DISPATCHER_H