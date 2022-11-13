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

#ifndef __AVUOS_UTILITY_H_
#define __AVUOS_UTILITY_H_

#include <common/type_traits.h>

namespace DC {
	template <typename T>
	typename remove_reference<T>::type &&move(T &&arg)
	{
		return static_cast<typename remove_reference<T>::type &&>(arg);
	}

	template<typename T> void swap(T &t1, T& t2)
	{
		T temp = DC::move(t1);
		t1 = DC::move(t2);
		t2 = DC::move(temp);
	}
}

#endif // __AVUOS_UTILITY_H_