## mongopp
mongocxx封装，支持集群、异步和同步

### 使用前需要下载安装
    mongo-c-driver
    mongo-cxx-driver
    boost

### 注：当前mongo-cxx-driver使用BOOST构建，也可以不需要BOOST库，则需要C++17支持。不建议使用C++17，VS很难支持。

## build linux
  ### C bson
    cd mongo-c-driver/src/libbson
    mkdir build
    cd build
    cmake ..
    make && make install
  ### build mongo-c-driver
    cd mongo-c-driver
    mkdir build
    cd build
    cmake ..
    make && make install
  ### build bsoncxx
    cd mongo-cxx-driver/src
    mkdir build
    cd build
    cmake ..
    make && make install
  ### build mongo-cxx-driver
    cd mongo-cxx-driver
    cmake -DCMAKE_BUILD_TYPE=Release -DBSONCXX_POLY_USE_BOOST=1 -DBOOST_ROOT=boost_root -DCMAKE_INSTALL_PREFIX=/usr/local
## Windows
  ### C bson
    cd mongo-c-driver/src/libbson
    mkdir build
    cd build
    cmake .. 创建VS工程 
  ### mongo-c-driver
    cd mongo-c-driver
    mkdir build
    cd build
    cmake .. 创建VS工程 
  ### bsoncxx
    cd mongo-cxx-driver/src
    mkdir build
    cd build
    cmake .. 创建VS工程 
  ### mongo-cxx-driver
    cd mongo-cxx-driver
    mkdir build
    cd build
    cmake -G "Visual Studio 15 2017" "-DENABLE_SSL=WINDOWS" "-DENABLE_SASL=SSPI" "-DCMAKE_INSTALL_PREFIX=mongo-c-driver_include" "-DCMAKE_PREFIX_PATH=mongo-c-driver_lib" "-DCMAKE_BUILD_TYPE=Release"

    第三方库目录
    3rd/include/boost
    3rd/include/bsoncxx
    3rd/include/libbson
    3rd/include/libmongoc
    3rd/include/mongocxx

    3rd/lib/debug|release
    3rd/lib/debug|release
    3rd/lib/debug|release
    3rd/lib/debug|release
    3rd/lib/debug|release

  ### mongopp
    mkdir build
    cd build
    cmake -G "Visual Studio 15 2017" .. 或 cmake -G "Visual Studio 15 2017 Win64" ..
    
## 使用
    初始化mongodb
    db::MongoWrapper::initMongodb();
    启动mongo线程
    db::MongoAsynMgr::getSingleton()->start();
    添加一条连接
    db::MongoAsynMgr::getSingleton().addConnect(conn1, db::ReadMode::k_primary, "127.0.0.1", "replsetName", "dbName", "user", "pwd");
  
    三条异步查询，开始执行的opernId和最终执行结果的operId一一对应
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
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        db::MongoAsynMgr::getSingleton().onTimer();//回调与此同一条线程
    }
    //ͬ同步
    auto conn2 = SmartPtr<db::MongoWrapper>(new db::MongoWrapper);
    conn2->initConnection("127.0.0.1", db::ReadMode::k_primary, "", "dbName",     "user", "pwd");

    db::MongoWrapper::autoCursor cursor;
    auto error = conn2->selectMany(cursor, "testDB", "{'name':'name'}", " {'hello':'world'}","{'age':-1}");
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
