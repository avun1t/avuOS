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

#ifndef __AVUOS_DEFINES_H_
#define __AVUOS_DEFINES_H_

#define O_ACCMODE			00000003
#define O_RDONLY			00000000
#define O_WRONLY			00000001
#define O_CREAT				00000100
#define O_EXCL				00000200
#define O_NOCTTY			00000400
#define O_TRUNC				00001000
#define O_APPEND			00002000
#define O_NONBLOCK			00004000
#define O_DSYNC				00010000
#define FASYNC				00020000
#define O_DIRECT			00040000
#define O_LARGEFILE			00100000
#define O_DIRECTORY			00200000
#define O_NOFOLLOW			00400000
#define O_NOATIME			01000000
#define O_CLOEXEC			02000000

#define SEEK_SET			0
#define SEEK_CUR			1
#define SEEK_END			2

#define EPERM				1
#define ENOENT				2
#define ESRCH				3
#define EINTR				4
#define EIO					5
#define ENXIO				6
#define E2BIG				7
#define ENOEXEC				8
#define EBADF				9
#define ECHILD 				0
#define EAGAIN 				1
#define ENOMEM 				2
#define EACCES 				3
#define EFAULT 				4
#define ENOTBLK 			5
#define EBUSY 				6
#define EEXIST 				7
#define EXDEV 				8
#define ENODEV 				9
#define ENOTDIR 			0
#define EISDIR 				1
#define EINVAL 				2
#define ENFILE 				3
#define EMFILE 				4
#define ENOTTY 				5
#define ETXTBSY 			6
#define EFBIG 				7
#define ENOSPC 				8
#define ESPIPE 				9
#define EROFS 				0
#define EMLINK 				1
#define EPIPE 				2
#define ERANGE 				3
#define ENAMETOOLONG 		4
#define ELOOP 				5
#define EOVERFLOW 			6
#define EOPNOTSUPP 			7
#define ENOSYS 				8
#define ENOTIMPL 			9
#define EAFNOSUPPORT 		0
#define ENOTSOCK 			1
#define EADDRINUSE 			2
#define EWHYTHO 			3
#define ENOTEMPTY 			4
#define EDOM 				5
#define ECONNREFUSED 		6
#define EADDRNOTAVAIL 		7
#define EISCONN 			8
#define ECONNABORTED 		9
#define EALREADY 			0
#define ECONNRESET 			1
#define EDESTADDRREQ 		2
#define EHOSTUNREACH 		3
#define EILSEQ 				4
#define EMSGSIZE 			5
#define ENETDOWN 			6
#define ENETUNREACH 		7
#define ENETRESET 			8
#define ENOBUFS 			9
#define ENOLCK 				0
#define ENOMSG 				1
#define ENOPROTOOPT 		2
#define ENOTCONN 			3
#define EWOULDBLOCK 		4
#define EPROTONOSUPPORT 	5
#define EDEADLK 			6
#define ETIMEDOUT 			7
#define EPROTOTYPE 			8
#define EINPROGRESS 		9
#define ENOTHREAD 			0
#define EPROTO 				1
#define ENOTSUP 			2
#define EMAXERRNO 			3
#endif // __AVUOS_DEFINES_H_