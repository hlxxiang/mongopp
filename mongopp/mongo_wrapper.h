#ifndef MONGOWRAPPER_H_
#define MONGOWRAPPER_H_

#include "define.h"

#include <deque>
#include <string>
#include <time.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>



namespace db {

    enum MongoCode {
        E_SUCCESS = 0,
        E_CONNECTION_FAILS,
        E_INVALID_ARGS,
        E_INVALID_NAMESPACE,
        E_AUTH_FAILS,
        E_INVALID_RESULT,
        E_DB_EXCEPTION,
        E_EXIST,
        E_TIMEOUT
    };

    enum DBOpertionCode
    {
        OPERTION_UPDATE = 0,
        OPERTION_INSERT,
        OPERTION_REMOVE,
        OPERTION_REMOVEALL
    };

    enum ReadMode
    {
        k_primary = 0,
        k_primary_preferred,
        k_secondary,
        k_secondary_preferred,
        k_nearest
    };

    struct DBOption_T
    {
        DBOption_T(int option, const std::string &table,
            const std::string query, const std::string date)
            :_upsert(false), _multi(false), _raw(false)
        {
            _option = option;

            _table = table;
            _query = query;
            _data = date;
        }

        int _option;

        std::string _table;
        std::string _query;
        std::string _data;
        bool _upsert;
        bool _multi;
        bool _raw;
    };

    using DBOptionPtr = SmartPtr<DBOption_T>;

    class MongoWrapper {
    public:
        using BSONValueList = std::vector<bsoncxx::document::value>;
        using autoCursor = SmartPtr<mongocxx::v_noabi::cursor>;

        MongoWrapper();
        ~MongoWrapper();

        static bool initMongodb();
        static void shutdown();
        MongoCode initConnection(const std::string &host_name, ReadMode read_mode, const std::string& replset, const std::string &db_name, const std::string &username, const std::string &password);
        MongoCode resetConnect();

        /*
        * ͳ�Ƹ���
        */
        MongoCode count(unsigned long long &result, const std::string &ns, const std::string &query);

        /*
        * ��������
        */
        MongoCode selectOne(bsoncxx::v_noabi::stdx::optional<bsoncxx::document::value> &result, const std::string &ns, const std::string &query, const std::string &filter);

        /*
        * ����
        */
        MongoCode selectMany(autoCursor &result, const std::string &ns, const std::string &query, const std::string &filter, const std::string& sort, std::int64_t limit = 0, std::int64_t skip = 0);

        /*
        * ���뵥��
        */
        MongoCode insertOne(const std::string &ns, const std::string &data);

        /*
        * ���뵥��
        */
        MongoCode insertMany(const std::string &ns, const std::string &data);

        /*
        * ���µ���
        */
        MongoCode updateOne(const std::string &ns, const std::string& query, const std::string& data, bool upsert = true);

        /*
        * ���¶���
        */
        MongoCode updateMany(const std::string &ns, const std::string& query, const std::string& data, bool upsert = true);

        /*
        * ɾ������
        */
        MongoCode deleteOne(const std::string &ns, const std::string& query);

        /*
        * ɾ������
        */
        MongoCode deleteMany(const std::string &ns, const std::string& query);

        /*
        * ɾ��
        */
        MongoCode drop(const std::string &ns);
    private:

        MongoCode ping();


        ReadMode        _read_mode;
        std::string     _url;
        std::uint16_t   _port;
        std::string     _replset;
        std::string     _dbName;
        std::string     _username;
        std::string     _password;
        mongocxx::client* _conn;
        mongocxx::database _db;
    };

    using MongoWrapperPtr = SmartPtr<MongoWrapper>;
}
#endif /* MONGOWRAPPER_H_ */
