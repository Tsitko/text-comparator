#pragma once

#include <chrono>
#include <iostream>

class LogDuration {
public:
    using Clock = std::chrono::steady_clock;
    
    LogDuration(const std::string& name, std::ostream& out): name_(name), out_(out) {
    }

    ~LogDuration() {
        using namespace std::chrono;
        using namespace std::literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        out_ << name_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }

private:
    const Clock::time_point start_time_ = Clock::now();
    const std::string name_;
	std::ostream& out_;
};

#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x, y) LogDuration UNIQUE_VAR_NAME_PROFILE(x, y) 