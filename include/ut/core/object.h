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

#ifndef UT_OBJECT_H
#define UT_OBJECT_H

namespace UT {

class EventLoop;

class Object {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Object(EventLoop* eventLoop);
    Object(Object* parent = nullptr);
    virtual ~Object();

    /**************************************************************************
     * Accessors / Mutators
     *************************************************************************/

    EventLoop* eventLoop() const;

private:
    /**************************************************************************
     * Members
     *************************************************************************/

    EventLoop* m_eventLoop = nullptr;
};

/******************************************************************************
 * Inline definition: Accessors / Mutators
 *****************************************************************************/

inline EventLoop* Object::eventLoop() const { return m_eventLoop; }

} // namespace UT

#endif // UT_OBJECT_H