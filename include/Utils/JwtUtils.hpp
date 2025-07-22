#pragma once
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpResponse.h>

#include <jwt-cpp/jwt.h>

using namespace drogon;
using namespace std::chrono_literals;

namespace Utils
{

inline void saveRefreshToCookie(const std::string& token, const HttpResponsePtr& resp, int maxAge = 604800) // 7 days
{
    Cookie cookie("refreshToken", token);
    cookie.setHttpOnly(true);
    cookie.setSecure(true);
    cookie.setSameSite(Cookie::SameSite::kStrict);
    cookie.setPath("/refresh");
    cookie.setMaxAge(maxAge);

    resp->addCookie(cookie);
}

inline std::string makeAccessToken(const int id, const std::string& username)
{
    static auto accessSecret = app().getCustomConfig()["jwt"]["access_secret"].asString();

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
        .sign(jwt::algorithm::hs256(accessSecret));

    return token;
}

inline std::string makeRefreshToken(const int id, const std::string& username)
{
    static auto refreshSecret = app().getCustomConfig()["jwt"]["refresh_secret"].asString();

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

}
