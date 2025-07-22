#include "Filters/JwtFilter.hpp"

#include "Utils/JwtUtils.hpp"

namespace Filters
{

void JwtFilter::doFilter(
    const HttpRequestPtr& req,
    FilterCallback&& fcb,
    FilterChainCallback&& fccb
)
{
    static auto accessSecret = app().getCustomConfig()["jwt"]["access_secret"].asString();

    const auto authHeader = req->getHeader("Authorization");
    const auto sessionJwt = req->getSession()->get<std::string>("jwtAccess");

    auto token = authHeader.empty() ? sessionJwt : authHeader.substr(7);

    if(authHeader.empty() && sessionJwt.empty())
    {
        if(tryRefresh(req))
            token = req->getSession()->get<std::string>("jwtAccess");
        else
        {
            const auto response = HttpResponse::newHttpResponse();
            response->setStatusCode(k401Unauthorized);

            fcb(response);

            return;
        }
    }

    try
    {
        const auto decoded = jwt::decode(token);
        const auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256(accessSecret))
            .with_issuer("auth0");

        verifier.verify(decoded);

        req->setParameter("user_id", decoded.get_payload_claim("user_id").as_string());
        req->setParameter("username", decoded.get_payload_claim("username").as_string());

        fccb();
    }
    catch(...)
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        fcb(response);
    }
}

bool JwtFilter::tryRefresh(const HttpRequestPtr& req)
{
    static auto refreshSecret = app().getCustomConfig()["jwt"]["refresh_secret"].asString();

    const auto refreshToken = req->getCookie("refreshToken");

    if(refreshToken.empty())
        return false;

    try
    {
        const auto decoded = jwt::decode(refreshToken);
        const auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256(refreshSecret))
                .with_issuer("auth0");

        verifier.verify(decoded);

        const auto userId = decoded.get_payload_claim("user_id").as_string();
        const auto username = decoded.get_payload_claim("username").as_string();

        req->getSession()->insert(
            "jwtAccess",
            Utils::makeAccessToken(std::stoi(userId), username)
        );

        return true;
    }
    catch(...)
    {
        return false;
    }
}

}
