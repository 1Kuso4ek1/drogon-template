#include "Controllers/StaticController.hpp"

namespace Controllers
{

void StaticController::swaggerPage(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newRedirectionResponse("swagger/index.html"));
}

void StaticController::swaggerFile(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newFileResponse("../static/swagger.json"));
}

}
