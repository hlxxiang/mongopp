#ifndef _H_FAST_QUEUE_H_
#define _H_FAST_QUEUE_H_
#include <deque>
#include "mutex.h"

namespace util
{
    /*
    * 多生产者-单消费者队列
    */
    template<typename T, unsigned Size = 0>
    class FastQueue
    {
    public:
        typedef std::deque<T>   Cont;

        FastQueue() : _flag(0)
        {
        }
        ~FastQueue()
        {}

        /*
        * push
        */
        inline void push(const T& elem)
        {
            _mutex.lock();
            if (_flag == 0)
                _containerB.push_back(elem);
            else
                _containerA.push_back(elem);
            _mutex.unlock();
        }

        /*
        * pop
        */
        bool pop(T& elem)
        {
            if (_flag == 0)
            {
                if (!_containerA.empty())
                {
                    elem = _containerA.front();
                    _containerA.pop_front();
                    return true;
                }
                else
                {
                    _mutex.lock();
                    if (!_containerB.empty())
                    {
                        _flag = 1;
                        _mutex.unlock();

                        elem = _containerB.front();
                        _containerB.pop_front();
                        return true;
                    }
                    _mutex.unlock();
                }
            }
            else
            {
                if (!_containerB.empty())
                {
                    elem = _containerB.front();
                    _containerB.pop_front();
                    return true;
                }
                else
                {
                    _mutex.lock();
                    if (!_containerA.empty())
                    {
                        _flag = 0;
                        _mutex.unlock();

                        elem = _containerA.front();
                        _containerA.pop_front();
                        return true;
                    }
                    _mutex.unlock();
                }
            }

            return false;
        }

        /*
        * total
        */
        inline unsigned getSize()
        {
            _mutex.lock();
            unsigned int size = 0;
            size = _containerA.size() + _containerB.size();
            _mutex.unlock();
            return size;
        }

        /*
        * 移除未使用的空间
        */
        inline void reserved()
        {
            _mutex.lock();
            _containerA.shrink_to_fit();
            _containerB.shrink_to_fit();
            _mutex.unlock();
        }

        /*
        * 消费者调用
        */
        inline unsigned getSizeR()
        {
            if (_flag == 0)
                return (unsigned)_containerA.size();
            else
                return (unsigned)_containerB.size();
        }

        /*
        * 生产者调用
        */
        inline unsigned getSizeW()
        {
            _mutex.lock();
            unsigned int size = 0;
            if (_flag == 0)
                size = _containerB.size();
            else
                size = _containerA.size();
            _mutex.unlock();
            return size;
        }

        /*
        * 消费者调用
        */
        inline int getFlag(){ return _flag; }

    private:
        Cont _containerA;
        Cont _containerB;
        int _flag;         //读线程切换队列状态
        Mutex _mutex;
    };

}

#endif //_H_FAST_QUEUE_H_
