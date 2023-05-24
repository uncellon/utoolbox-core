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

#ifndef UT_FUNCTION_WRAPPER_H
#define UT_FUNCTION_WRAPPER_H

#include "abstractwrapper.h"

namespace UT {

template<class TFunction>
class FunctionWrapper;

template<class TReturn, class... TArgs>
class FunctionWrapper<TReturn(TArgs...)> : public AbstractWrapper<TReturn(TArgs...)> {
    using TFWrapper = FunctionWrapper<TReturn(TArgs...)>;
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    FunctionWrapper() = default;

    // Auto-bind constructor
    FunctionWrapper(TReturn (*func)(TArgs...)) {
        bind(func);
    }

    // Copy constructor
    FunctionWrapper(const FunctionWrapper& other) 
    : mFunc(other.mFunc) { }

    // Move constructor
    FunctionWrapper(FunctionWrapper&& other) 
    : mFunc(other.mFunc) {
        other.mFunc = nullptr;
    }

    virtual ~FunctionWrapper() = default;

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual TFWrapper* clone() override { 
        return new TFWrapper(*this); 
    }

    void bind(TReturn (*func)(TArgs...)) {
        mFunc = func;
    }

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    virtual TReturn operator()(TArgs... args) override {
        return (*mFunc)(args...);
    }

    // Copy assignment operator
    FunctionWrapper& operator=(const FunctionWrapper& other) {
        if (this == &other) {
            return *this;
        }
        mFunc = other.mFunc;
        return *this;
    }

    // Move assignment operator
    FunctionWrapper& operator=(FunctionWrapper&& other) {
        if (this == &other) {
            return *this;
        }
        mFunc = other.mFunc;
        other.mFunc = nullptr;
        return *this;
    }

protected:
    /**************************************************************************
     * Method (Protected)
     *************************************************************************/

    virtual bool isEquals(const AbstractWrapper<TReturn(TArgs...)>& other) const override {
        const TFWrapper* otherPtr = dynamic_cast<const TFWrapper*>(&other);
        return otherPtr != nullptr && mFunc == otherPtr->mFunc;
    }

    /**************************************************************************
     * Members
     *************************************************************************/

    TReturn (*mFunc)(TArgs...) = nullptr;
    
}; // class FunctionWrapper

} // namespace UT

#endif // UT_FUNCTION_WRAPPER_H