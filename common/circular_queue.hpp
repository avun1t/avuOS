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

#ifndef __AVUOS_CIRCULAR_QUEUE_HPP_
#define __AVUOS_CIRCULAR_QUEUE_HPP_

#include "cstddef.h"
#include "utility.h"
#include "stdlib.h"

namespace DC {
	template<typename T>
	class circular_queue {
	public:
		circular_queue(size_t size) : _storage(new T[size]), _capacity(size), _size(0)
		{}

		~circular_queue()
		{
			for (auto i = 0; i < _capacity; i++)
				_storage[i].~T();
			
			delete _storage;
		}

		bool push(const T& elem)
		{
			if (size() == _capacity) return false;
			if (_front == -1) _front = 0;
			_back = (++_back) % _capacity;
			_storage[_back] = DC::move(elem);
			_size++;
			return true;
		}

		T &pop_front()
		{
			T &ret = _storage[_front];
			if (_front == _back) {
				_front = -1;
				_back = -1;
			} else {
				_front = (++_front) % _capacity;
			}

			_size--;
			return ret;
		}

		T &pop_back()
		{
			T &ret = _storage[_back];
			if (_front == _back) {
				_front = -1;
				_back = -1;
			} else if (_back == 0) {
				_back = _capacity - 1;
			} else {
				_back--;
			}
			_size--;
			return ret;
		}

		T &front() const
		{
			return _storage[_front];
		}

		T &back() const
		{
			return _storage[_back];
		}

		bool empty() const
		{
			return _size == 0;
		}

		size_t size() const
		{
			return _size;
		}

	private:
		T *_storage;
		size_t _capacity;
		size_t _size = 0;
		size_t _front = -1;
		size_t _back = -1;
	};
}

#endif // __AVUOS_CIRCULAR_QUEUE_HPP_