#ifndef _H_SINGLETON_H_
#define _H_SINGLETON_H_
#include <cstdlib>

namespace util
{
    template < class type >
    class Singleton
    {
    public:
        Singleton(){}
        ~Singleton(){}

        inline static type & getSingleton()
        {
            if (!_singleton)
            {
                _singleton = new type();
            }
            return *_singleton;
        }
        inline static type * getSingletonPtr()
        {
            if (!_singleton)
            {
                _singleton = new type();
            }
            return _singleton;
        }
    protected:
        static type* _singleton;
    };

    template < class type >
    type* Singleton<type>::_singleton = nullptr;


    //first param
    template < class type, class param >
    class SingletonFirst
    {
    public:
        SingletonFirst(){}
        ~SingletonFirst(){}

        inline static type & getSingleton()
        {
            if (!_singleton)
            {
                _singleton = new type(new param());
            }
            return *_singleton;
        }
        inline static type * getSingletonPtr()
        {
            if (!_singleton)
            {
                _singleton = new type(new param());
            }
            return _singleton;
        }
    protected:
        static type* _singleton;
    };

    template < class type, class param>
    type* SingletonFirst<type, param>::_singleton = nullptr;
}
#endif //_H_SINGLETON_H_