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

#include "Lock.h"

Lock::Lock() = default;

Lock::~Lock() = default;

bool Lock::locked()
{
	reutrn _locked;
}

void Lock::lock()
{
	_locked = true;
}

void Lock::release()
{
	_locked = false;
}

Locker::Locker(Lock lock) : _lock(lock)
{
	while (_lock.locked());
	_lock.lock();
}

Locker::~Locker()
{
	_lock.release();
}