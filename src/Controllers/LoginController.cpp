#include <Controllers/LoginController.hpp>
#include <Database/DatabaseManager.hpp>

using namespace std::chrono_literals;

void LoginController::registerUser(const HttpRequestPtr& req, Callback&& callback)
{
    auto request = req->getJsonObject();

    if(!request || !request->isMember("username") || !request->isMember("password"))
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k400BadRequest);

        callback(response);
        return;
    }

    // static auto& mapper = DatabaseManager::get().getMapper<models::User>();

    // Implement your own logic...

    login(req, std::move(callback));
}

void LoginController::login(const HttpRequestPtr& req, Callback&& callback)
{
    auto request = req->getJsonObject();

    if(validateUser(request))
    {
        // Just a template - change it however you want
        auto userId = (*request)["user_id"].asInt();
        auto username = (*request)["username"].asString();
        
        auto accessToken = makeAccessToken(userId, username);
        auto refreshToken = makeRefreshToken(userId, username);

        Json::Value json;
        json["token"] = accessToken;

        auto response = HttpResponse::newHttpJsonResponse(json);

        saveRefreshToCookie(refreshToken, response);

        callback(response);
    }
    else
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
    }
}

void LoginController::refresh(const HttpRequestPtr& req, Callback&& callback)
{
    static auto refreshSecret = drogon::app().getCustomConfig()["jwt"]["refresh_secret"].asString();

    auto refreshToken = req->getCookie("refreshToken");

    if(refreshToken.empty())
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
        return;
    }

    try
    {
        auto decoded = jwt::decode(refreshToken);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256(refreshSecret))
            .with_issuer("auth0");

        verifier.verify(decoded);

        auto accessToken =
            makeAccessToken(
                decoded.get_payload_claim("user_id").as_integer(),
                decoded.get_payload_claim("username").as_string()
            );

        Json::Value json;
        json["token"] = accessToken;

        auto response = HttpResponse::newHttpJsonResponse(json);

        callback(response);
    }
    catch(const std::exception& e)
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        callback(response);
    }
}

void LoginController::logout(const HttpRequestPtr& req, Callback&& callback)
{
    auto response = HttpResponse::newHttpResponse();

    // Removing cookie
    saveRefreshToCookie("", response, 0);

    callback(response);
}

void LoginController::saveRefreshToCookie(const std::string& token, const HttpResponsePtr& resp, int maxAge)
{
    Cookie cookie("refreshToken", token);
    cookie.setHttpOnly(true);
    cookie.setSecure(true);
    cookie.setSameSite(Cookie::SameSite::kStrict);
    cookie.setPath("/refresh");
    cookie.setMaxAge(maxAge);

    resp->addCookie(cookie);
}

bool LoginController::validateUser(const std::shared_ptr<Json::Value>& json)
{
    // Implement your own validation logic
    return false;
}

std::string LoginController::makeAccessToken(int id, const std::string& username)
{
    static auto accessSecret = drogon::app().getCustomConfig()["jwt"]["access_secret"].asString();

    auto token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_expires_in(3600s) // 1 hour
        .set_payload_claim(
            "user_id",
            jwt::claim(std::to_string(id))
        )
        .set_payload_claim(
            "username",
            jwt::claim(username)
        )
        .sign(jwt::algorithm::hs256(accessSecret)); // Change the secret to something *really* secret (in config)

    return token;
}

std::string LoginController::makeRefreshToken(int id, const std::string& username)
{
    static auto refreshSecret = drogon::app().getCustomConfig()["jwt"]["refresh_secret"].asString();

    auto token = jwt::create()
        .set_issuer("auth0")
        .set_type("JWS")
        .set_expires_in(604800s) // 7 days
        .set_payload_claim(
            "user_id",
            jwt::claim(std::to_string(id))
        )
        .set_payload_claim(
            "username",
            jwt::claim(username)
        )
        .sign(jwt::algorithm::hs256(refreshSecret));

    return token;
}
