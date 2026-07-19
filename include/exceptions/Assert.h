#pragma once
#include "log/Logger.h"
#include <cstdlib>
#include <format>
#include <source_location>

// Programmer errors / broken invariants — "this should never happen if the
// code is correct." Distinct from Result<T,E> (recoverable/expected) and
// EngineException (unrecoverable but *expected-to-be-possible* engine
// failures like a missing config file).
//
// ENGINE_ASSERT is intentionally NOT compiled out in release (NDEBUG).
// A broken invariant that's silently skipped in a shipping build doesn't
// go away — it just resurfaces later as a corrupted-state crash with no
// diagnostic trail. It logs full context, then aborts immediately: no
// attempt is made to continue past a known-broken invariant.
//
// ENGINE_ASSERT_DEBUG is for checks too expensive to pay for every frame
// in a shipping build (e.g. O(n) container-consistency checks) — those
// compile out under NDEBUG. Prefer ENGINE_ASSERT unless you have a
// specific, measured reason to reach for this one.
#define ENGINE_ASSERT(cond)                                                     \
    do                                                                          \
    {                                                                           \
        if (!(cond))                                                            \
        {                                                                       \
            AssertDetail::Fail(#cond, std::source_location::current());         \
        }                                                                       \
    } while (0)

#define ENGINE_ASSERT_MSG(cond, msg)                                            \
    do                                                                          \
    {                                                                           \
        if (!(cond))                                                            \
        {                                                                       \
            AssertDetail::Fail(#cond, (msg), std::source_location::current());  \
        }                                                                       \
    } while (0)

#ifdef NDEBUG
#define ENGINE_ASSERT_DEBUG(cond) \
    do                            \
    {                             \
    } while (0)
#else
#define ENGINE_ASSERT_DEBUG(cond) ENGINE_ASSERT(cond)
#endif

namespace AssertDetail
{
    [[noreturn]] inline void Fail(const char *expr, std::source_location loc)
    {
        LOG_FATAL(std::format("Assertion failed: {} at {}:{} ({})",
                               expr, loc.file_name(), loc.line(), loc.function_name()));
        std::abort();
    }

    [[noreturn]] inline void Fail(const char *expr, const std::string &msg, std::source_location loc)
    {
        LOG_FATAL(std::format("Assertion failed: {} — {} at {}:{} ({})",
                               expr, msg, loc.file_name(), loc.line(), loc.function_name()));
        std::abort();
    }
}