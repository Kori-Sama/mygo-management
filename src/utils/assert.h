#ifndef __ASSERT_H
#define __ASSERT_H

#ifdef NDEBUG
#define ASSERT(expr,msg) ((void)0)
#define DEBUG_ASSERT(expr, msg) ((void)0)
#else
/// @brief Assert that the expression is true
/// @param expr The expression to assert
/// @param msg The message to print if the assertion failed
/// @note This assert will exit the program if the expression is false
#define ASSERT(expr, msg) if (!(expr)) { fprintf(stderr, "--------------------------------\n-- Assertion failed: %s\n-- File: %s\n-- Line: %d\n--------------------------------\n", msg, __FILE__, __LINE__); exit(1); }

/// @brief Assert that the expression is true
/// @param expr The expression to assert
/// @param msg The message to print if the assertion failed
/// @note This assert will only print the error message instead of exiting the program
#define DEBUG_ASSERT(expr, msg) if (!(expr)) { fprintf(stderr, "Assertion failed: %s\n", msg); }
#endif



#endif // __ASSERT_H