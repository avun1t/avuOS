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

#ifndef __AVUOS_STRING_H_
#define __AVUOS_STRING_H_

#include <common/cstddef.h>
#include <common/cstring.h>

namespace DC {
	class string {
	public:
		string();
		string(const string &string);
		string(const char *string);
		~string();

		string &operator=(const char *str);
		string &operator=(const string &str);
		string &operator+=(const string &str);
		string operator+(const string &b) const;
		bool operator==(const string &str) const;
		bool operator==(const char *str) const;
		bool operator!=(const string &str) const;
		bool operator!=(const char *str) const;
		char &operator[](size_t index) const;

		size_t length() const;
		char *c_str() const;
		char *data() const;
		string substr(size_t start, size_t length) const;
		size_t find(const string &str, size_t start = 0) const;
		size_t find(const char  *tr, size_t start = 0) const;
		size_t find(const char c, size_t start = 0) const;
	private:
		size_t _size;
		size_t _length;
		char *_cstring;
	};
}

#endif // __AVUOS_STRING_H_