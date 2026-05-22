#pragma once

/// Entity identity interface. All game entities with unique IDs implement this.
class IIdentifier {
public:
    virtual int getId() const = 0;
    virtual ~IIdentifier() = default;
};
