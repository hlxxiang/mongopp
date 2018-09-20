#ifndef _H_DEFINE_H_
#define _H_DEFINE_H_

#include <boost/shared_ptr.hpp>
#include <unordered_map>

#if defined WIN32
#define NOMINMAX 1
#include <WinSock2.h>
#include <windows.h>

#define snprintf _snprintf

#else

#define snprintf snprintf

#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>

#endif

#define SmartPtr boost::shared_ptr
#define SmartPtrCast boost::dynamic_pointer_cast

#define DB_MAX_QUEUE 1024

#endif //_H_DEFINE_H_