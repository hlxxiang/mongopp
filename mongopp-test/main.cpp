#include "mongo_asyn_mgr.h"
int main(int argc, char const *argv[])
{
    auto conn1 = "conn1";
    db::MongoWrapper::initMongodb();
    db::MongoAsynMgr::getSingleton()->start();
    db::MongoAsynMgr::getSingleton().addConnect(conn1, db::ReadMode::k_primary, "127.0.0.1", "replsetName", "dbName", "user", "pwd");

    //异步
    auto oper = SmartPtr<db::MongoAsynOper>(new db::MongoAsynOper([](const db::ResultMap& ret) {
        for (auto&& iter = ret.begin(); iter != ret.end(); ++iter){
            auto operId = iter->second->id;
            auto code = iter->second->code;
            auto count = iter->second->count;
        }
    }));
    auto operId1 = oper->count("testDB", "{hello:'world'}");
    auto operId2 = oper->count("testDB", "{world:'hello'}");
    auto operId3 = oper->count("testDB", "{hello:'world'}");
    db::MongoAsynMgr::getSingleton().runOper("conn1", oper);

    //ͬ同步
    auto conn2 = SmartPtr<db::MongoWrapper>(new db::MongoWrapper);
    conn2->initConnection("127.0.0.1", db::ReadMode::k_primary, "", "dbName", "user", "pwd");

    db::MongoWrapper::autoCursor cursor;
    auto error = conn2->selectMany(cursor, "testDB", "{'name':'name'}", "{'hello':'world'}","{'age':-1}");
    if (db::E_SUCCESS == error)
    {
        for (auto&& it = cursor->begin(); it != cursor->end(); it++){
            for (auto&& element : *it){

            }
        }
    }
    else
    {

    }
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        db::MongoAsynMgr::getSingleton().onTimer();
    }
    db::MongoWrapper::shutdown();
    return 0;
}
