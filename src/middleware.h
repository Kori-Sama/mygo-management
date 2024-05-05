#ifndef __MIDDLEWARE_H
#define __MIDDLEWARE_H

#include "http/context.h"
#include "utils/string_utils.h"
#include "utils/base64.h"
#include "tool.h"

#define TOKEN_PREFIX "Bearer "
#define SECRET_KEY "MyGO!!!!! is a bang team with five girls, Anno, Tomori, Soyo, Taki and Rena"
#define LOGIN_URL "http://localhost:3000/login"

void logger_middleware(Context& ctx) {
    std::cout << "[" << ctx.req.method << "]       " << ctx.req.url << "          " << ctx.req.ip << ":" << ctx.req.port << std::endl;
}

void cors_middleware(Context& ctx) {
    auto origin = ctx.req.header["Origin"];
    if (!origin.empty()) {
        ctx.header("Access-Control-Allow-Origin", "*")
            .header("Access-Control-Allow-Headers", "Origin, Content-Type, Accept, Authorization")
            .header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS")
            .header("Access-Control-Allow-Credentials", "true");
    }

    if (ctx.req.method == "OPTIONS") {
        ctx.code(HttpCode::NO_CONTENT);
        ctx.block();
        return;
    }
}

/// @brief Make a JWT middleware 
/// @param exclude_urls The urls to exclude from the middleware
/// @return The middleware function
HandleFunc make_jwt_middleware(std::vector<std::string>&& exclude_urls) {
    return [exclude_urls](Context& ctx) {
        for (auto& url : exclude_urls) {
            if (ctx.req.url == url) {
                return;
            }
        }

        std::string token = ctx.req.header["Authorization"];
        if (!utils::starts_with(token, TOKEN_PREFIX)) {
            ctx.redirect(LOGIN_URL);
            ctx.block();
            return;
        }

        auto tokens = utils::split(token, ".");
        if (tokens.size() != 3) {
            ctx.redirect(LOGIN_URL);
            ctx.block();
            return;
        }
        token = tokens[1];
        auto secret = tokens[2];

        auto bytes = utils::base64_decode(token);
        std::string jwt(bytes.begin(), bytes.end());
        auto jwt_info = json_object_to_map(jwt.c_str());
        if (!jwt_info) {
            ctx.redirect(LOGIN_URL);
            ctx.block();
            return;
        }

        int exp = std::stoi(jwt_info.value()["exp"]);
        if (exp < time(nullptr)) {
            ctx.redirect(LOGIN_URL);
            ctx.block();
            return;
        }

        if (jwt_info.value()["Role"] != "Admin") {
            ctx.code(HttpCode::FORBIDDEN).send("Forbidden, only Admin can access");
            ctx.block();
            return;
        }

        };

}

#endif // __MIDDLEWARE_H