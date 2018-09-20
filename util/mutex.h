#ifndef _H_MUTEX_H_H_
#define _H_MUTEX_H_H_
#include <stdio.h>

namespace util
{
    class Mutex
    {
    public:
        Mutex()
        {
#ifdef WINDOWS
            InitializeCriticalSection(&_lock);
#else
            pthread_mutex_init(&_lock, 0);
#endif
        }
        ~Mutex()
        {
#ifdef WINDOWS
            DeleteCriticalSection(&_lock);
#else
            pthread_mutex_destroy(&_lock);
#endif
        }
        void lock()
        {
#ifdef WIN32
            EnterCriticalSection(&_lock);
#else
            pthread_mutex_lock(&_lock);
#endif
        }
        void unlock()
        {
#ifdef WIN32
            LeaveCriticalSection(&_lock);
#else
            pthread_mutex_unlock(&_lock);
#endif
        }
    private:
#ifdef WINDOWS
        CRITICAL_SECTION _lock;
#else
        pthread_mutex_t _lock;
#endif
    };
}
#endif //_H_MUTEX_H_H_