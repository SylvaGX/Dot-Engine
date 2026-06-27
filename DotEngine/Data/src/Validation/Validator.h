#pragma once

#include <functional>
#include <string>
#include <vector>

namespace DotData {

    struct ValidationResult {
        bool        valid = true;
        std::string error;

        static ValidationResult Ok()                        { return {true,  {}}; }
        static ValidationResult Fail(std::string msg)       { return {false, std::move(msg)}; }
    };

    // Rule-based validator. Attach typed rules; call Validate() after deserialization.
    //
    // Example:
    //   Validator<WindowConfig> v;
    //   v.AddRule([](const WindowConfig& c) -> ValidationResult {
    //       if (c.width == 0) return ValidationResult::Fail("width must be > 0");
    //       return ValidationResult::Ok();
    //   });
    //   auto result = v.Validate(cfg);
    template<typename T>
    class Validator {
    public:
        using Rule = std::function<ValidationResult(const T&)>;

        void AddRule(Rule rule) {
            m_Rules.push_back(std::move(rule));
        }

        // Runs all rules in order; returns first failure, or Ok.
        ValidationResult Validate(const T& obj) const {

            // TODO: Improve this method. It can be multi-threaded for more efficient (probably...)
            for (const auto& rule : m_Rules) {
                auto result = rule(obj);
                if (!result.valid)
                    return result;
            }
            return ValidationResult::Ok();
        }

    private:
        std::vector<Rule> m_Rules;
    };

} // namespace DotData
