#include "mongo_asyn_oper.h"
#include "log.h"

namespace db
{
    MongoAsynOper::MongoAsynOper(MongoOperHandle handle)
        : _operId(0)
        , _result(0)
        , _handle(handle)
    {
    }

    MongoAsynOper::~MongoAsynOper()
    {
    }

    MongoOperID MongoAsynOper::getOperId()
    {
        return ++_operId;
    }

    MongoOperID MongoAsynOper::count(const std::string &table, const std::string &query)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_count, getOperId()));
        oper->table = table;
        oper->query = query;
        _operArray.push_back(oper);
        return oper->id;
    }

    MongoOperID MongoAsynOper::selectOne(const std::string &table, const std::string &query, const std::string &fields)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_selectOne, getOperId()));
        oper->table = table;
        oper->query = query;
        oper->fields = fields;
        _operArray.push_back(oper);
        return oper->id;
    }

    MongoOperID MongoAsynOper::selectMany(const std::string &table, const std::string &query, const std::string &fields, const std::string &sort, int limit, int skip)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_selectMany, getOperId()));
        oper->table = table;
        oper->query = query;
        oper->fields = fields;
        oper->sort = sort;
        oper->limit = limit;
        oper->skip = skip;
        _operArray.push_back(oper);
        return oper->id;
    }

    MongoOperID MongoAsynOper::insertOne(const std::string &table, const std::string &data)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_insertOne, getOperId()));
        oper->table = table;
        oper->data = data;
        _operArray.push_back(oper);
        return oper->id;
    }

    MongoOperID MongoAsynOper::insertMany(const std::string &table, const std::string &data)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_insertMany, getOperId()));
        oper->table = table;
        oper->data = data;
        _operArray.push_back(oper);
        return oper->id;
    }

    MongoOperID MongoAsynOper::updateOne(const std::string &table, const std::string& query, const std::string& data)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_updateOne, getOperId()));
        oper->table = table;
        oper->query = query;
        oper->data = data;
        _operArray.push_back(oper);
        return oper->id;
    }

    MongoOperID MongoAsynOper::updateMany(const std::string &table, const std::string& query, const std::string& data)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_updateMany, getOperId()));
        oper->table = table;
        oper->query = query;
        oper->data = data;
        _operArray.push_back(oper);
        return oper->id;
    }

    MongoOperID MongoAsynOper::deleteOne(const std::string &table, const std::string& query)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_deleteOne, getOperId()));
        oper->table = table;
        oper->query = query;
        _operArray.push_back(oper);
        return oper->id;
    }

    MongoOperID MongoAsynOper::deleteMany(const std::string &table, const std::string& query)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_deleteMany, getOperId()));
        oper->table = table;
        oper->query = query;
        _operArray.push_back(oper);
        return oper->id;
    }

    MongoOperID MongoAsynOper::drop(const std::string &table)
    {
        MongoDataOperPtr oper(new MongoDataOper(MongoDataOper::op_drop, getOperId()));
        oper->table = table;
        _operArray.push_back(oper);
        return oper->id;
    }

    int MongoAsynOper::onHandleCount(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        unsigned long long ret = 0;
        error = conn->count(ret, dataPtr->table, dataPtr->query);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error, ret));
        _resultArr.insert(ResultPair(result->id, result));
        return error;
    }

    int MongoAsynOper::onHandleSelectOne(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        bsoncxx::v_noabi::stdx::optional<bsoncxx::document::value> bsonObj;
        error = conn->selectOne(bsonObj, dataPtr->table, dataPtr->query, dataPtr->fields);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error));
        _resultArr.insert(ResultPair(result->id, result));
        if (E_SUCCESS == error)
        {
            result->bsonArr.push_back(bsonObj.get());
        }
        return error;
    }

    int MongoAsynOper::onHandleSelectMany(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        MongoWrapper::MongoWrapper::autoCursor cursor;
        error = conn->selectMany(cursor, dataPtr->table, dataPtr->query, dataPtr->fields, dataPtr->sort, dataPtr->limit, dataPtr->skip);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error));
        _resultArr.insert(ResultPair(result->id, result));
        if (MongoCode::E_SUCCESS == error)
        {
            for (auto&& doc : *cursor.get())
            {
                result->bsonArr.push_back(doc);
            }
        }
        return error;
    }

    int MongoAsynOper::onHandleInsertOne(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        error = conn->insertOne(dataPtr->table, dataPtr->data);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error));
        _resultArr.insert(ResultPair(result->id, result));
        return error;
    }

    int MongoAsynOper::onHandleInsertMany(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        error = conn->insertMany(dataPtr->table, dataPtr->data);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error));
        _resultArr.insert(ResultPair(result->id, result));
        return error;
    }

    int MongoAsynOper::onHandleUpdateOne(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        error = conn->updateOne(dataPtr->table, dataPtr->query, dataPtr->data);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error));
        _resultArr.insert(ResultPair(result->id, result));
        return error;
    }

    int MongoAsynOper::onHandleUpdateMany(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        error = conn->updateMany(dataPtr->table, dataPtr->query, dataPtr->data);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error));
        _resultArr.insert(ResultPair(result->id, result));

        return error;
    }
    
    int MongoAsynOper::onHandleDeleteOne(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        error = conn->deleteOne(dataPtr->table, dataPtr->query);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error));
        _resultArr.insert(ResultPair(result->id, result));
        return error;
    }

    int MongoAsynOper::onHandleDeleteMany(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        error = conn->deleteMany(dataPtr->table, dataPtr->query);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error));
        _resultArr.insert(ResultPair(result->id, result));
        return error;
    }

    int MongoAsynOper::onHandleDrop(MongoWrapperPtr conn, MongoDataOperPtr dataPtr)
    {
        db::MongoCode error = E_SUCCESS;
        error = conn->drop(dataPtr->table);
        MongoResult_TPtr result(new MongoResult_T(dataPtr->id, error));
        _resultArr.insert(ResultPair(result->id, result));
        return error;
    }

    void MongoAsynOper::onTimer()
    {
        if (_handle)
        {
            _handle(_resultArr);
        }
        else
        {
            LOG_ERROR("mongoOper callback is null");
        }
    }

    void MongoAsynOper::onHandle(MongoWrapperPtr conn)
    {
        for (size_t i = 0; i < _operArray.size(); ++i)
        {
            MongoDataOperPtr dataPtr = _operArray[i];
            if (dataPtr->op == MongoDataOper::op_count)
                onHandleCount(conn, dataPtr);
            else if (dataPtr->op == MongoDataOper::op_selectOne)
                onHandleSelectOne(conn, dataPtr);
            else if (dataPtr->op == MongoDataOper::op_selectMany)
                onHandleSelectMany(conn, dataPtr);
            else if (dataPtr->op == MongoDataOper::op_insertOne)
                onHandleInsertOne(conn, dataPtr);
            else if (dataPtr->op == MongoDataOper::op_insertMany)
                onHandleUpdateMany(conn, dataPtr);
            else if (dataPtr->op == MongoDataOper::op_updateOne)
                onHandleUpdateOne(conn, dataPtr);
            else if (dataPtr->op == MongoDataOper::op_updateMany)
                onHandleUpdateMany(conn, dataPtr);
            else if (dataPtr->op == MongoDataOper::op_deleteOne)
                onHandleDeleteOne(conn, dataPtr);
            else if (dataPtr->op == MongoDataOper::op_deleteMany)
                onHandleDeleteMany(conn, dataPtr);
            else if (dataPtr->op == MongoDataOper::op_drop)
                onHandleDrop(conn, dataPtr);
            else
                assert(0);
        }
    }
}