#ifndef __MIDDLEWARE_H
#define __MIDDLEWARE_H

#include "http/context.h"
#include "utils/string_utils.h"
#include "utils/base64.h"
#include "tool.h"

#define TOKEN_PREFIX "Bearer "
#define SECRET_KEY "MyGO!!!!! is a bang team with five girls, Anno, Tomori, Soyo, Taki and Rena"
#define LOGIN_URL "/login"

void logger_middleware(Context& ctx) {
    std::cout << "[" << ctx.req.method << "]       " << ctx.req.url << "          " << ctx.req.ip << ":" << ctx.req.port << std::endl;
}

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

        int exp = std::stoi(jwt_info["exp"]);
        if (exp < time(nullptr)) {
            ctx.redirect(LOGIN_URL);
            ctx.block();
            return;
        }

        if (jwt_info["role"] != "Admin") {
            ctx.code(HttpCode::FORBIDDEN).send("Forbidden, only Admin can access");
            ctx.block();
            return;
        }

        };

}

#endif // __MIDDLEWARE_H