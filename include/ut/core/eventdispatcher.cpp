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

#include "abstractevent.h"
#include "eventdispatcher.h"

namespace UT {

std::mutex EventDispatcher::mInstanceMutex;
EventDispatcher* EventDispatcher::mInstance = nullptr;

EventDispatcher* EventDispatcher::getInstance() {
    std::unique_lock lock(mInstanceMutex);
    if (!mInstance) {
        mInstance = new EventDispatcher();
    }
    return mInstance;
}

void EventDispatcher::registerAttachment(AbstractEvent* event, Object* object, AbstractDelegate* delegate) {
    std::unique_lock lock(mVectorMutex);

    mAttachments.emplace_back(DispatcherInfo { event, object, delegate });
}

void EventDispatcher::removeAttachment(AbstractEvent* event, AbstractDelegate* delegate) {
    std::unique_lock lock(mVectorMutex);

    for (size_t i = 0; i < mAttachments.size(); ++i) {
        if (mAttachments[i].event != event
            || delegate != mAttachments[i].delegate) {
            continue;
        }
        mAttachments.erase(mAttachments.begin() + i);
        return;
    }
}

void EventDispatcher::eventDestroyed(AbstractEvent* event) {
    std::unique_lock lock(mVectorMutex);

    for (size_t i = 0; i < mAttachments.size(); ++i) {
        if (mAttachments[i].event != event) {
            continue;
        }
        mAttachments.erase(mAttachments.begin() + i);
        --i;
    }
}

void EventDispatcher::clean(Object* object) {
    std::unique_lock lock(mVectorMutex);

    for (size_t i = 0; i < mAttachments.size(); ++i) {
        if (mAttachments[i].object != object) {
            continue;
        }
        mAttachments[i].event->removeEventHandler(mAttachments[i].delegate);
        mAttachments.erase(mAttachments.begin() + i);
        --i;
    }
}

bool EventDispatcher::attachmentValid(AbstractEvent* event, AbstractDelegate* delegate) {
    std::unique_lock lock(mVectorMutex);

    for (size_t i = 0; i < mAttachments.size(); ++i) {
        if (mAttachments[i].event == event
            && mAttachments[i].delegate == delegate) {
            return true;
        }
    }
    return false;
}

} // namespace UT