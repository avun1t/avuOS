/*
	This file is part of avuOS.
	avuOS is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	avuOS is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with avuOS.  If not, see <https://www.gnu.org/licenses/>.
	Copyright (c) avun1t 2022. All rights reserved.
*/

#ifndef AVUOS_UNIQUE_PTR_HPP
#define AVUOS_UNIQUE_PTR_HPP

/**
 * @file  unique_ptr.hpp
 * @brief unique_ptr is a fake implementation to use in place of a C++11 std::unique_ptr when compiling on an older compiler.
 *
 * @see http://www.cplusplus.com/reference/memory/unique_ptr/
 *
 * Copyright (c) 2014-2019 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#pragma once

#include <common/cstddef.h>
#include <common/utility.h>

// can be replaced by other error mechanism
#define SHARED_ASSERT(x) /*ASSERT(x)*/ //TODO: Fix

namespace DC {
/**
 * @brief fake implementation to use in place of a C++11 std::move() when compiling on an older compiler.
 *
 * @see http://www.cplusplus.com/reference/utility/move/
*/
	template<typename T>
	inline T &move(T &v)
	{
		return v;
	}

/**
 * @brief minimal implementation of unique pointer, a subset of the C++11 std::unique_ptr or boost::unique_ptr.
 *
 * unique_ptr is a smart pointer retaining ownership of an object through a provided pointer,
 * without the overhead of the shared_ptr reference counter.
 * It does not allow sharing this ownership (but moving it is possible)
 * and destroys the object when the pointer is destroyed or reset.
 *
 * This implementation is a fake one, to use when compiling on an older compiler that does not have the move semantic.
 * As such, it cannot offer any guaranty that the pointer is not copied!
 * It is still usable in production on such a compiler if you also compile
 * your program on a recent compiler to verify that it does not violate this guaranty.
 */
	template<class T>
	class unique_ptr {
	public:
		/// The type of the managed object, aliased as member type
		typedef T element_type;

		/// @brief Default constructor
		unique_ptr(void) throw() : px(NULL) // never throws
		{}

		/// @brief Constructor with the provided pointer to manage
		explicit unique_ptr(T *p) throw() : px(p) // never throws
		{}

		/// @brief Copy constructor to convert from another pointer type
		/* TODO MSVC error C2248: 'unique_ptr<B>::px' : unique_ptr<A> cannot access private member declared in class 'unique_ptr<B>'
		template <class U>
		unique_ptr(const unique_ptr<U>& ptr) throw() : // never throws
			px(static_cast<typename unique_ptr<T>::element_type*>(ptr.px))
		{
			const_cast<unique_ptr<U>&>(ptr).px = NULL; // const-cast to force ownership transfer!
		}
		*/

		/// @brief Copy constructor (used by the copy-and-swap idiom)
		unique_ptr(const unique_ptr &ptr) throw() : px(ptr.px) // never throws
		{
			const_cast<unique_ptr &>(ptr).px = NULL; // const-cast to force ownership transfer!
		}

		/// @brief Assignment operator using the copy-and-swap idiom (copy constructor and swap method)
		unique_ptr &operator=(unique_ptr ptr) throw() // never throws
		{
			swap(ptr);
			return *this;
		}

		/// @brief the destructor releases its ownership and destroy the object
		inline ~unique_ptr(void) throw() // never throws
		{
			destroy();
		}

		/// @brief this reset releases its ownership and destroy the object
		inline void reset(void) throw() // never throws
		{
			destroy();
		}

		/// @brief this reset release its ownership and re-acquire another one
		void reset(T *p) throw() // never throws
		{
			SHARED_ASSERT((NULL == p) || (px != p)); // auto-reset not allowed
			destroy();
			px = p;
		}

		/// @brief Swap method for the copy-and-swap idiom (copy constructor and swap method)
		void swap(unique_ptr &lhs) throw() // never throws
		{
			DC::swap(px, lhs.px);
		}

		/// @brief release the ownership of the px pointer without destroying the object!
		inline void release(void) throw() // never throws
		{
			px = NULL;
		}

		// reference counter operations :
		inline operator bool() const throw() // never throws
		{
			return (NULL != px); // TODO nullptr
		}

		// underlying pointer operations :
		inline T &operator*() const throw() // never throws
		{
			SHARED_ASSERT(NULL != px);
			return *px;
		}

		inline T *operator->() const throw() // never throws
		{
			SHARED_ASSERT(NULL != px);
			return px;
		}

		inline T *get(void) const throw() // never throws
		{
			// no assert, can return NULL
			return px;
		}

	private:
		/// @brief release the ownership of the px pointer and destroy the object
		inline void destroy(void) throw() // never throws
		{
			delete px;
			px = NULL;
		}

		/// @brief hack: const-cast release the ownership of the px pointer without destroying the object!
		inline void release(void) const throw() // never throws
		{
			px = NULL;
		}

	private:
		T *px; //!< Native pointer
	};


// comparaison operators
	template<class T, class U>
	inline bool operator==(const unique_ptr<T> &l, const unique_ptr<U> &r) throw() // never throws
	{
		return (l.get() == r.get());
	}

	template<class T, class U>
	inline bool operator!=(const unique_ptr<T> &l, const unique_ptr<U> &r) throw() // never throws
	{
		return (l.get() != r.get());
	}

	template<class T, class U>
	inline bool operator<=(const unique_ptr<T> &l, const unique_ptr<U> &r) throw() // never throws
	{
		return (l.get() <= r.get());
	}

	template<class T, class U>
	inline bool operator<(const unique_ptr<T> &l, const unique_ptr<U> &r) throw() // never throws
	{
		return (l.get() < r.get());
	}

	template<class T, class U>
	inline bool operator>=(const unique_ptr<T> &l, const unique_ptr<U> &r) throw() // never throws
	{
		return (l.get() >= r.get());
	}

	template<class T, class U>
	inline bool operator>(const unique_ptr<T> &l, const unique_ptr<U> &r) throw() // never throws
	{
		return (l.get() > r.get());
	}
}

#endif // AVUOS_UNIQUE_PTR_HPP