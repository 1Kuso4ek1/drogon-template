#include <Database/DatabaseManager.hpp>

#include <drogon/HttpAppFramework.h>

DatabaseManager::DatabaseManager()
{
    dbClient = drogon::app().getDbClient();
}

orm::DbClientPtr DatabaseManager::getDbClient()
{
    return dbClient;
}
