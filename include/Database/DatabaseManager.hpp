#pragma once
#include <Utils/Singleton.hpp>

#include <drogon/orm/DbClient.h>
#include <drogon/orm/Mapper.h>

#include <typeindex>
#include <unordered_map>
#include <any>

using namespace drogon;

class DatabaseManager : public Singleton<DatabaseManager>
{
public:
    orm::DbClientPtr getDbClient();

    // Stores needed mappers in cache and checks their existence (not really sure if it's useful)
    template<class T>
    orm::Mapper<T>& getMapper()
    {
        auto mapper = mappers.find(typeid(T));
        if(mapper == mappers.end())
            mappers.emplace(typeid(T), orm::Mapper<T>(dbClient));

        return std::any_cast<orm::Mapper<T>&>(mappers.at(typeid(T)));
    }

private:
    DatabaseManager();

    friend class Singleton<DatabaseManager>;

private:
    orm::DbClientPtr dbClient;

    std::unordered_map<std::type_index, std::any> mappers;
};
