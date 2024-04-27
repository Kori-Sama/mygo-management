#ifndef __MIDDLEWARE_H
#define __MIDDLEWARE_H

#include "http/context.h"

void middleware1(Context& ctx) {
    ctx.header("Test", "middleware1");
}

#endif // __MIDDLEWARE_H