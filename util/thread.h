#ifndef _H_THREAD_H_
#define _H_THREAD_H_
#include <thread>
#include <functional>

namespace util
{
    class Thread
    {
    public:
        using ThreadID = unsigned long;
        using PROC = std::function<void(void)>;

        Thread(void);
        ~Thread(void);

        bool start(PROC t, const std::string& name = "thread");
        bool stop();
    protected:
        std::thread::id getId();
        void handle();
    private:
        std::thread _thread;
        std::string _name;
        PROC _proc;
    };
}
#endif //_H_THREAD_H_