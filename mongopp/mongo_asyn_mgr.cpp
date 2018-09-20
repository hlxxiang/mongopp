#include "mongo_asyn_mgr.h"

#include "log.h"

namespace db
{
    MongoAsynMgr::MongoAsynMgr()
        : _loop(false)
        , _sleep(0)
    {

    }

    MongoAsynMgr::~MongoAsynMgr()
    {
        stop();
    }

    bool MongoAsynMgr::addConnect(const std::string& dbKey, ReadMode read_mode, const std::string &host_name, const std::string& replset, const std::string &db_name, const std::string &username, const std::string &password)
    {
        MongoWrapperPtr pWarpper(new MongoWrapper());
        if (MongoCode::E_SUCCESS == pWarpper->initConnection(host_name, static_cast<ReadMode>(read_mode), replset, db_name, username, password))
        {
            assert(_warpper_map.find(dbKey) == _warpper_map.end());
            _warpper_map.insert(MongoWrapperMapPair(dbKey, pWarpper));
            return true;
        }

        return false;
    }

    void MongoAsynMgr::delConnect(const std::string & dbKey)
    {
        auto it = _warpper_map.find(dbKey);
        if (it != _warpper_map.end())
        {
            _warpper_map.erase(it);
        }
    }

    MongoWrapperPtr MongoAsynMgr::findMongoWrapper(const std::string& dbKey)
    {
        auto iter = _warpper_map.find(dbKey);
        if (iter != _warpper_map.end())
            return iter->second;

        return MongoWrapperPtr();
    }

    bool MongoAsynMgr::start()
    {
        if (_loop) 
        {
            return false;
        }
        _loop = true;
        _sleep = 10;
        auto bStart = _thread.start([this]() {
            while (_loop)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                if (_operQueue.getSize() > 0)
                {
                    execute();
                }
            }
            execute();
        }, "mongoThread");
        return bStart;
    }

    void MongoAsynMgr::stop()
    {
        _loop = false;
        _thread.stop();
        _warpper_map.clear();
    }

    void MongoAsynMgr::runOper(const std::string& dbKey, MongoAsynOperPtr oper)
    {
        _operQueue.push(std::pair<std::string, MongoAsynOperPtr>(dbKey, oper));
    }

    void MongoAsynMgr::execute()
    {
        int size = _operQueue.getSize();
        while (size > 0)
        {
            std::pair<std::string, MongoAsynOperPtr> oper;
            _operQueue.pop(oper);
            auto it = findMongoWrapper(oper.first);
            if (it.get())
            {
                if (size > DB_MAX_QUEUE)
                {
                    LOG_WARNING("MongoAsyn::read warning: data is most(%d)!", size);
                }
                size--;
                oper.second->onHandle(it);
                _resultQueue.push(oper);
            }
            else
            {
                LOG_ERROR("Not found %s mongo", oper.first.c_str());
            }
        }
    }

    void MongoAsynMgr::onTimer()
    {
        int size = _resultQueue.getSize();
        while (size > 0)
        {
            std::pair<std::string, MongoAsynOperPtr> oper;
            _resultQueue.pop(oper);
            oper.second->onTimer();
            --size;
        }
    }
}