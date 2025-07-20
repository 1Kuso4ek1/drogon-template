#include <Filters/JwtFilter.hpp>

#include <drogon/HttpAppFramework.h>

#include <jwt-cpp/jwt.h>

namespace Filters
{

void JwtFilter::doFilter(
    const HttpRequestPtr& req,
    FilterCallback&& fcb,
    FilterChainCallback&& fccb
)
{
    static auto refreshSecret = app().getCustomConfig()["jwt"]["access_secret"].asString();

    const auto authHeader = req->getHeader("Authorization");
    
    if(authHeader.empty())
    {
        const auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        return fcb(response);
    }

    try
    {
        // Pure token with "Bearer " removed
        const auto token = authHeader.substr(7);
        const auto decoded = jwt::decode(token);
        const auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256("secret"))
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

}
