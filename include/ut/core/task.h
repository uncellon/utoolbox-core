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

#ifndef UT_TASK_H
#define UT_TASK_H

#include "abstractevent.h"
#include "delegate.h"

#include <future>

namespace UT {

class Object;

/******************************************************************************
 *
 * AbstractTask class
 *
 *****************************************************************************/

class AbstractTask {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    virtual ~AbstractTask() = default;

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual void execute() = 0;

    /**************************************************************************
     * Accessors / Mutators
     *************************************************************************/

    virtual AbstractDelegate* delegate() const = 0;

    inline void setSender(AbstractEvent* sender) { m_sender = sender; }
    inline AbstractEvent* sender() const { return m_sender; }

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    AbstractEvent* m_sender = nullptr;
}; // class AbstractTask



/******************************************************************************
 *
 * Task template helper
 *
 *****************************************************************************/

template<class TFunction>
class Task;



/******************************************************************************
 *
 * Task (without return and arguments)
 *
 *****************************************************************************/

template<> 
class Task<void()> : public AbstractTask {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Task(Delegate<void()>* delegate) : m_delegate(delegate) { }

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual inline void execute() override { (*m_delegate)(); }

    /**************************************************************************
     * Accessors / Mutators
     *************************************************************************/

    virtual AbstractDelegate* delegate() const override { return m_delegate; }

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    Delegate<void()>* m_delegate;
}; // class Task<void()>



/******************************************************************************
 *
 * Task (without return and with arguments)
 *
 *****************************************************************************/

template<class... TArgs>
class Task<void(TArgs...)> : public AbstractTask {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Task(Delegate<void(TArgs...)>* delegate, TArgs... args)
    : m_delegate(delegate), m_args(std::tuple<TArgs...>(args...)) { }

    /**************************************************************************
     * Methods
     *************************************************************************/

    // virtual void execute() override { (*m_delegate)(std::get<TArgs...>(m_args)); }
    virtual void execute() override { std::apply(*m_delegate, m_args); }

    /**************************************************************************
     * Accessors / Mutators
     *************************************************************************/

    virtual AbstractDelegate* delegate() const override { return m_delegate; }

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    Delegate<void(TArgs...)>* m_delegate;
    std::tuple<TArgs...> m_args;
}; // class Task<void(TArgs...)>



/******************************************************************************
 *
 * Task (with return and without arguments)
 *
 *****************************************************************************/

template<class TReturn>
class Task<TReturn()> : public AbstractTask {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Task(Delegate<TReturn()>* delegate) : m_delegate(delegate) { }

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual inline void execute() override { m_promise.set_value((*m_delegate)()); }

    /**************************************************************************
     * Accessors / Mutators
     *************************************************************************/

    virtual AbstractDelegate* delegate() const override { return m_delegate; }
    std::future<TReturn> future() { return m_promise.get_future(); }

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    Delegate<TReturn()>* m_delegate;
    std::promise<TReturn> m_promise;
}; // class Task<TReturn()>



/******************************************************************************
 *
 * Task (with return and with arguments)
 *
 *****************************************************************************/

template<class TReturn, class... TArgs>
class Task<TReturn(TArgs...)> : public AbstractTask {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Task(Delegate<TReturn(TArgs...)>* delegate, TArgs... args) 
    : m_delegate(delegate), m_args(std::tuple<TArgs...>(args...)) { }

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual inline void execute() override { m_promise.set_value(std::apply(*m_delegate, m_args)); }

    /**************************************************************************
     * Accessors / Mutators
     *************************************************************************/

    virtual AbstractDelegate* delegate() const override { return m_delegate; }
    std::future<TReturn> future() { return m_promise.get_future(); }

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    Delegate<TReturn(TArgs...)>* m_delegate;
    std::tuple<TArgs...> m_args;
    std::promise<TReturn> m_promise;
}; // class Task<TReturn(TArgs...)>

} // namespace UT

#endif // UT_TASK_H