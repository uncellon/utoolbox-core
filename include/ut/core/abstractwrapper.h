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

#ifndef UT_ABSTRACT_WRAPPER_H
#define UT_ABSTRACT_WRAPPER_H

namespace UT {

template<class TFunction>
class AbstractWrapper;

template<class TReturn, class... TArgs>
class AbstractWrapper<TReturn(TArgs...)> {
    using TWrapper = AbstractWrapper<TReturn(TArgs...)>;
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    virtual ~AbstractWrapper() = default;

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual TWrapper* clone() = 0;    
    virtual TReturn operator()(TArgs...) = 0;

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    inline bool operator==(const TWrapper& other) const {
        return isEquals(other);
    }

    bool operator!=(const TWrapper& other) const {
        return !(*this == other);
    }

protected:
    /**************************************************************************
     * Methods (Protected)
     *************************************************************************/

    virtual bool isEquals(const TWrapper& other) const = 0;    
}; // class AbstractWrapper

} // namespace UT

#endif // UT_ABSTRACT_WRAPPER_H