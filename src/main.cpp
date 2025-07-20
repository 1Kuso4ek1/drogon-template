#include <drogon/drogon.h>

using namespace drogon;

int main(const int argc, char* argv[])
{
    auto configPath = "../config/config-template.json";

    if(argc > 1)
        configPath = argv[1];

    app()
        .loadConfigFile(configPath)
        .registerPostHandlingAdvice( // Needed only for swagger, remove it in production
            [](const HttpRequestPtr& req, const HttpResponsePtr& resp) {
                resp->addHeader("Access-Control-Allow-Origin", "*");
            }
        )
        .run();
}
