#pragma once
#include <drogon/HttpFilter.h>

using namespace drogon;

namespace Filters
{

class JwtFilter final : public HttpFilter<JwtFilter>
{
public:
    void doFilter(
        const HttpRequestPtr& req,
        FilterCallback&& fcb,
        FilterChainCallback&& fccb
    ) override;

};

}
