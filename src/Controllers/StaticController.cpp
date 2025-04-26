#include <Controllers/StaticController.hpp>

void StaticController::swaggerPage(const HttpRequestPtr& req, Callback&& callback)
{
    auto resp = HttpResponse::newRedirectionResponse("swagger/index.html");

    callback(resp);
}

void StaticController::swaggerFile(const HttpRequestPtr& req, Callback&& callback)
{
    auto resp = HttpResponse::newFileResponse("../static/swagger.json");

    callback(resp);
}

void StaticController::image(const HttpRequestPtr& req, Callback&& callback, const std::string& path)
{
    auto resp = HttpResponse::newFileResponse("../static/img/" + path);

    callback(resp);
}
