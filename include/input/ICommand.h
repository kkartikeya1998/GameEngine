#pragma once

// A command is "what a bound key means" for one particular state's
// context type. Templated so each state can have its own vocabulary
// (a menu doesn't need Direction/sprint/jump, gameplay doesn't need
// NavigateUp/Confirm) without one bloated shared struct.
template <typename Context>
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute(Context& ctx) const = 0;
};