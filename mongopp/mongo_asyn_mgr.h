#ifndef _H_MONGO_ASYN_MGR_H_
#define _H_MONGO_ASYN_MGR_H_

#include "define.h"

#include <unordered_map>

#include "mongo_wrapper.h"
#include "mongo_asyn_oper.h"

#include "../util/singleton.h"
#include "../util/thread.h"

namespace db
{
    using MongOperQueue = util::FastQueue<std::pair<std::string, MongoAsynOperPtr>>;

    class MongoAsynMgr
        : public util::Singleton<MongoAsynMgr>
    {
    public:
        using MongoWrapperMap = std::unordered_map<std::string, MongoWrapperPtr>;
        using MongoWrapperMapPair = std::pair<std::string, MongoWrapperPtr>;

        MongoAsynMgr();
        ~MongoAsynMgr();

        bool addConnect(const std::string& dbKey, ReadMode read_mode, const std::string &host_name, const std::string& replset, const std::string &db_name, const std::string &username, const std::string &password);
        void delConnect(const std::string& dbKey);
        MongoWrapperPtr findMongoWrapper(const std::string& dbKey);
        bool start();
        void stop();
        void runOper(const std::string& dbKey, MongoAsynOperPtr oper);
        void execute();
        void onTimer();
    private:
        MongoWrapperMap _warpper_map;

        MongOperQueue _operQueue;
        MongOperQueue _resultQueue;

        bool _loop;
        int _sleep;

        util::Thread _thread;
    };
}
#endif //_H_MONGO_ASYN_MGR_H_