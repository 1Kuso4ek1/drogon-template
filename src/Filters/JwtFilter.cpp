#include <Filters/JwtFilter.hpp>

#include <drogon/HttpAppFramework.h>

void JwtFilter::doFilter(
    const HttpRequestPtr& req,
    FilterCallback&& fcb,
    FilterChainCallback&& fccb
)
{
    static auto refreshSecret = drogon::app().getCustomConfig()["jwt"]["access_secret"].asString();
    
    auto authHeader = req->getHeader("Authorization");
    
    if(authHeader.empty())
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);

        return fcb(response);
    }

    try
    {
        // Pure token with "Bearer " removed
        auto token = authHeader.substr(7);
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256("secret"))
            .with_issuer("auth0");

        verifier.verify(decoded);

        req->setParameter("user_id", decoded.get_payload_claim("user_id").as_string());
        req->setParameter("username", decoded.get_payload_claim("username").as_string());

        fccb();
    }
    catch(const std::exception& e)
    {
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(k401Unauthorized);
        
        fcb(response);
    }
}
