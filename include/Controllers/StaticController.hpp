#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

class StaticController : public HttpController<StaticController>
{
public:
    void swaggerPage(const HttpRequestPtr& req, Callback&& callback);
    void swaggerFile(const HttpRequestPtr& req, Callback&& callback);

    void image(const HttpRequestPtr& req, Callback&& callback, const std::string& path);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(StaticController::swaggerPage, "/swagger", Get);
        ADD_METHOD_TO(StaticController::swaggerFile, "/swagger/swagger.json", Get);

        ADD_METHOD_TO(StaticController::image, "/img/{path}", Get);

    METHOD_LIST_END

};
