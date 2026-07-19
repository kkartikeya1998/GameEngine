#pragma once
#include <variant>
#include <optional>
#include "exceptions/Assert.h"

// Minimal Result<T,E> for recoverable/expected failures. Not a monad, no
// map/and_then chaining — just enough to replace "throw on a miss" and
// "silently return nullptr" with an explicit, inspectable outcome.
//
//   Exceptions  -> "the engine cannot continue"        (exceptions/EngineExceptions.h)
//   Result<T,E> -> "something went wrong, we continue"  (this file)
//   ENGINE_ASSERT -> "the programmer broke an assumption" (exceptions/Assert.h)
//
// Pulling a value/error out the wrong way (value() on a failed Result,
// error() on a successful one) is itself a programmer error — that's a bug
// in the calling code, not a recoverable condition — so it goes through
// ENGINE_ASSERT rather than silently returning garbage.
template <typename T, typename E>
class Result
{
public:
    static Result Ok(T value) { return Result(std::move(value)); }
    static Result Err(E error) { return Result(std::move(error), Tag{}); }

    bool ok() const { return std::holds_alternative<T>(data_); }
    explicit operator bool() const { return ok(); }

    const T &value() const &
    {
        ENGINE_ASSERT_MSG(ok(), "Result::value() called on an Err result");
        return std::get<T>(data_);
    }
    T &value() &
    {
        ENGINE_ASSERT_MSG(ok(), "Result::value() called on an Err result");
        return std::get<T>(data_);
    }
    T &&value() &&
    {
        ENGINE_ASSERT_MSG(ok(), "Result::value() called on an Err result");
        return std::get<T>(std::move(data_));
    }

    const E &error() const
    {
        ENGINE_ASSERT_MSG(!ok(), "Result::error() called on an Ok result");
        return std::get<E>(data_);
    }

    T valueOr(T fallback) const { return ok() ? std::get<T>(data_) : std::move(fallback); }

private:
    struct Tag
    {
    };
    explicit Result(T value) : data_(std::move(value)) {}
    Result(E error, Tag) : data_(std::move(error)) {}

    std::variant<T, E> data_;
};

// Specialization for T = void — operations that either succeed or fail with
// no payload on success (e.g. EntityFactory::populate).
template <typename E>
class Result<void, E>
{
public:
    static Result Ok() { return Result(); }
    static Result Err(E error) { return Result(std::move(error)); }

    bool ok() const { return !error_.has_value(); }
    explicit operator bool() const { return ok(); }

    const E &error() const
    {
        ENGINE_ASSERT_MSG(!ok(), "Result::error() called on an Ok result");
        return *error_;
    }

private:
    Result() = default;
    explicit Result(E error) : error_(std::move(error)) {}
    std::optional<E> error_;
};