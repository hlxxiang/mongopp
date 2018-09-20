#if defined LINUX
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <pthread.h>
#endif

#include "log.h"
#include "thread.h"

namespace util
{
    Thread::Thread()
    {

    }

    Thread::~Thread()
    {

    }

    bool Thread::start(PROC t, const std::string& name)
    {
        _name = name;
        _proc = t;
        _thread = std::thread(std::bind(&Thread::handle, this));
        LOG_DEBUG("thread(name:%s) is start!", _name.c_str());
        return true;
    }

    bool Thread::stop()
    {
        if (_thread.joinable())
        {
            _thread.join();
            LOG_DEBUG("thread(name:%s) is stop!", _name.c_str());
            return true;
        }
        return false;
    }

    std::thread::id Thread::getId()
    {
        return _thread.get_id();
    }

    void Thread::handle()
    {
#ifdef LINUX
        sigset_t new_mask;
        sigfillset(&new_mask);
        sigset_t old_mask;

        pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);
#endif
        _proc();
    }
}
