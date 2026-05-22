#pragma once

#include <string>

/// Named entity interface. All entities with display names implement this.
class IDisplayName {
public:
    virtual std::string getDisplayName() const = 0;
    virtual ~IDisplayName() = default;
};
