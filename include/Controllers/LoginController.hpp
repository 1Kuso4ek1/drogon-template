#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class LoginController final : public HttpController<LoginController>
{
public:
    void registerUser(const HttpRequestPtr& req, Callback&& callback);
    void login(const HttpRequestPtr& req, Callback&& callback);

    static void logout(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(LoginController::registerUser, "/register", Post);
        ADD_METHOD_TO(LoginController::login, "/login", Post);

        ADD_METHOD_TO(logout, "/logout", Post);

    METHOD_LIST_END

private:
    bool validateUser(const std::shared_ptr<Json::Value>& json);
};

}
