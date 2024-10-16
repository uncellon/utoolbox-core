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

#ifndef UT_DELEGATE_H
#define UT_DELEGATE_H

#include "abstractdelegate.h"
#include "functionwrapper.h"
#include "lambdawrapper.h"
#include "methodwrapper.h"

#include <utility>

namespace UT {

template<class TFunction>
class Delegate;

template<class TReturn, class... TArgs>
class Delegate<TReturn(TArgs...)> : public AbstractDelegate {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Delegate() = default;

    // Auto-bind function constructor
    Delegate(TReturn (*func)(TArgs...)) {
        bind(func);
    }

    // Auto-bind method constructor
    template<class TClass>
    Delegate(TClass* object, TReturn (TClass::*method)(TArgs...)) {
        bind(object, method);
    }

    // Auto-bind lambda constructor
    template<class TLambda>
    Delegate(TLambda&& lambda) {
        bind(std::move(lambda));
    }

    // Copy constructor
    Delegate(const Delegate& other) {
        mWrapper = other.mWrapper->clone();
    }

    // Move constructor
    Delegate(Delegate&& other) {
        mWrapper = other.mWrapper;
        other.mWrapper = nullptr;
    }

    ~Delegate() {
        delete mWrapper;
    }

    /**************************************************************************
     * Methods
     *************************************************************************/

    // Bind function
    void bind(TReturn (*func)(TArgs...)) {
        if (mWrapper) {
            delete mWrapper;
        }
        mWrapper = new FunctionWrapper<TReturn(TArgs...)>(func);
    }

    // Bind method
    template<class TClass>
    void bind(TClass* object, TReturn (TClass::*method)(TArgs...)) {
        if (mWrapper) {
            delete mWrapper;
        }
        mWrapper = new MethodWrapper<TClass, TReturn(TArgs...)>(object, method);
    }

    // Bind lambda
    template<class TLambda>
    void bind(TLambda&& lambda) {
        if (mWrapper) {
            delete mWrapper;
        }
        auto wrapper = new LambdaWrapper<TLambda, TReturn(TArgs...)>();
        wrapper->bind(std::move(lambda));
        mWrapper = wrapper;
    }

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    TReturn operator()(TArgs... args) const {
        if (!mWrapper) {
            return TReturn();
        }
        return mWrapper->operator()(args...);
    }

    // Copy assignment operator
    Delegate& operator=(const Delegate& other) {
        if (this == &other) {
            return *this;
        }
        mWrapper = other.mWrapper->clone();
        return *this;
    };

    // Move assignment operator
    Delegate& operator=(Delegate&& other) {
        if (this == &other) {
            return *this;
        }
        mWrapper = other.mWrapper;
        other.mWrapper = nullptr;
        return *this;
    }

    inline bool operator==(const Delegate<TReturn(TArgs...)>& other) const {
        return *mWrapper == *(other.mWrapper);
    }

    inline bool operator!=(const Delegate<TReturn(TArgs...)>& other) const {
        return *mWrapper != *(other.mWrapper);
    }

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    AbstractWrapper<TReturn(TArgs...)>* mWrapper = nullptr;

}; // class Delegate

} // namespace UT

#endif // UT_DELEGATE_H