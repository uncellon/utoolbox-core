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

#ifndef UT_LAMBDA_WRAPPER_H
#define UT_LAMBDA_WRAPPER_H

#include "abstractwrapper.h"

namespace UT {

template<class TLambda, class TFunction>
class LambdaWrapper;

template<class TLambda, class TReturn, class... TArgs>
class LambdaWrapper<TLambda, TReturn(TArgs...)> : public AbstractWrapper<TReturn(TArgs...)> {
    using TLWrapper = LambdaWrapper<TLambda, TReturn(TArgs...)>;
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    LambdaWrapper() = default;

    // Copy constructor
    LambdaWrapper(const LambdaWrapper& other) { 
        m_lambda = new TLambda(*(other.m_lambda)); 
    }

    // Move constructor
    LambdaWrapper(LambdaWrapper&& other) 
    : m_lambda(other.m_lambda) {
        other.m_lambda = nullptr;
    }

    virtual ~LambdaWrapper() {
        delete m_lambda;
    }

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual TLWrapper* clone() override { 
        return new TLWrapper(*this); 
    }

    void bind(TLambda&& lambda) { 
        m_lambda = new TLambda(lambda);
    }

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    virtual TReturn operator()(TArgs... args) override {
        return m_lambda->operator()(args...);
    }

    // Copy assignment operator
    LambdaWrapper& operator=(const LambdaWrapper& other) {
        if (this == &other) {
            return *this;
        }
        m_lambda = other.m_lambda;
        return *this;
    }

    // Move assignment operator
    LambdaWrapper& operator=(LambdaWrapper&& other) {
        if (this == &other) {
            return *this;
        }
        m_lambda = other.m_lambda;
        other.m_lambda = nullptr;
        return *this;
    }

protected:
    /**************************************************************************
     * Method (Protected)
     *************************************************************************/

    virtual bool isEquals(const AbstractWrapper<TReturn(TArgs...)>& other) const override {
        const TLWrapper* otherPtr = dynamic_cast<const TLWrapper*>(&other);
        return otherPtr != nullptr && m_lambda == otherPtr->m_lambda;
    }

    /**************************************************************************
     * Members
     *************************************************************************/

    TLambda* m_lambda = nullptr;
};

} // namespace UT

#endif // UT_LAMBDA_WRAPPER_H