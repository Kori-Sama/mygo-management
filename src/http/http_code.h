#ifndef __HTTP_CODE_H
#define __HTTP_CODE_H

namespace http {
    enum HttpCode {
        OK = 200,
        CREATED = 201,
        NO_CONTENT = 204,
        FOUND = 302,
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        I_AM_TEAPOT = 418,
        INTERNAL_SERVER_ERROR = 500
    };
}

#endif // __HTTP_CODE_H