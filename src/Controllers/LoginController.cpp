#include "Controllers/LoginController.hpp"

#include "Utils/JwtUtils.hpp"

#include <drogon/orm/Mapper.h>

using namespace std::chrono_literals;

namespace Controllers
{

void LoginController::registerUser(const HttpRequestPtr& req, Callback&& callback)
{
    if(const auto request = req->getJsonObject();
        !request || !request->isMember("username") || !request->isMember("password"))
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k400BadRequest);

        callback(response);
        return;
    }

    // Implement your own logic...
    // auto dbClient = app().getDbClient();
    // auto& mapper = orm::Mapper<models::User>(dbClient);

    login(req, std::move(callback));
}

void LoginController::login(const HttpRequestPtr& req, Callback&& callback)
{
    if(const auto request = req->getJsonObject();
        validateUser(request))
    {
        // Just a template - change it however you want
        const auto userId = (*request)["user_id"].asInt();
        const auto username = (*request)["username"].asString();
        
        const auto accessToken = Utils::makeAccessToken(userId, username);
        const auto refreshToken = Utils::makeRefreshToken(userId, username);

        req->getSession()->insert("jwtAccess", access);

        Json::Value json;
        json["token"] = accessToken; // Optional, since we're storing access in session

        const auto response = HttpResponse::newHttpJsonResponse(json);

        Utils::saveRefreshToCookie(refreshToken, response);

        callback(response);
    }
    else
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
    }
}

void LoginController::logout(const HttpRequestPtr& req, Callback&& callback)
{
    const auto response = HttpResponse::newHttpResponse();

    // Removing cookie
    Utils::saveRefreshToCookie("", response, 0);

    // Clearing session
    req->getSession()->erase("jwtAccess");

    callback(response);
}

bool LoginController::validateUser(const std::shared_ptr<Json::Value>& json)
{
    // Implement your own validation logic
    return true;
}

}
