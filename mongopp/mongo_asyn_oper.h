#ifndef _H_MONGO_ASYN_H_
#define _H_MONGO_ASYN_H_

#include "define.h"
#include <vector>
#include <functional>
#include <stdint.h>

#include "../util/thread.h"
#include "../util/fast_queue.h"

#include "mongo_wrapper.h"

namespace db
{
    using MongoOperID = uint32_t;
    struct MongoDataOper
    {
        enum Option
        {
            op_count = 1,
            op_selectOne,
            op_selectMany,
            op_insertOne,
            op_insertMany,
            op_updateOne,
            op_updateMany,
            op_deleteOne,
            op_deleteMany,
            op_drop,
        };

        MongoDataOper(Option o, MongoOperID id) :
            op(o), id(id), limit(0), skip(0), upsert(false)
        {

        }

        Option op;
        MongoOperID id;
        std::string table;
        std::string query;
        std::string data;
        std::string fields;
        std::string sort;
        int limit;    //select
        int skip;     //select
        bool upsert;   //update
    };
    using BsonArray = std::vector<bsoncxx::document::view>;
    struct MongoResult_T
    {
        MongoResult_T(MongoOperID mongoId, int err, uint64_t c = 0) :
            id(mongoId), code(err), count(c)
        {

        }

        MongoOperID id;
        int code;        //update,insert,delete
        uint64_t count;    //count
        BsonArray bsonArr;  //select
    };
    using MongoDataOperPtr = SmartPtr<MongoDataOper>;
    using MongoDataOperArray = std::vector<MongoDataOperPtr>;
    using MongoResult_TPtr = SmartPtr<MongoResult_T>;
    using ResultMap = std::unordered_map<MongoOperID, MongoResult_TPtr>;
    using ResultPair = std::pair<MongoOperID, MongoResult_TPtr>;
    using MongoOperHandle = std::function<void(const ResultMap&)>;


    class MongoAsynOper
    {
    public:
        MongoAsynOper(MongoOperHandle handle);
        ~MongoAsynOper();

        MongoOperID getOperId();

        MongoOperID count(const std::string &table, const std::string &query);

        MongoOperID selectOne(const std::string &table, const std::string &query, const std::string &fields);

        MongoOperID selectMany(const std::string &table, const std::string &query, const std::string &fields, const std::string &sort, int limit = 0, int skip = 0);

        MongoOperID insertOne(const std::string &table, const std::string &data);

        MongoOperID insertMany(const std::string &table, const std::string &data);

        MongoOperID updateOne(const std::string &table, const std::string& query, const std::string& data);

        MongoOperID updateMany(const std::string &table, const std::string& query, const std::string& data);

        MongoOperID deleteOne(const std::string &table, const std::string& query);

        MongoOperID deleteMany(const std::string &table, const std::string& query);

        MongoOperID drop(const std::string &table);


        void onTimer();
        void onHandle(MongoWrapperPtr conn);
    private:
        int onHandleCount(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);
        int onHandleSelectOne(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);
        int onHandleSelectMany(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);
        int onHandleInsertOne(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);
        int onHandleInsertMany(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);
        int onHandleUpdateOne(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);
        int onHandleUpdateMany(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);
        int onHandleDeleteOne(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);
        int onHandleDeleteMany(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);
        int onHandleDrop(MongoWrapperPtr conn, MongoDataOperPtr dataPtr);

    private:
        MongoOperID _operId;
        MongoDataOperArray _operArray;
        ResultMap _resultArr;
        MongoOperHandle _handle;
        int _result;
    };

    using MongoAsynOperPtr = SmartPtr<MongoAsynOper>;
}
#endif //_H_MONGO_ASYN_H_