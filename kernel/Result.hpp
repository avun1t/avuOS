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

#ifndef __AVUOS_RESULT_HPP_
#define __AVUOS_RESULT_HPP_

#include <common/shared_ptr.hpp>

class Result {
public:
	static const int Success = 0;

	Result(int code);

	bool is_success() const;
	bool is_error() const;
	int code() const;
private:
	int _code;
};

template<typename T>
class ResultRet {
public:
	ResultRet(int error) : _result(error) {};
	ResultRet(Result error) : _result(error) {};
	ResultRet(T ret) : _ret(ret), _result(0) {};
	bool is_error() const { return _result.is_error(); }
	int code() const { return _result.code(); }
	T value() const
	{
		ASSERT(!is_error());
		return _ret;
	};
private:
	Result _result;
	T _ret;
};

#endif // __AVUOS_RESULT_HPP_