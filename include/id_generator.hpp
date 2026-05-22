#pragma once

/// Simple thread-unsafe incrementing ID generator.
class IdGenerator {
public:
    static int getNextId() {
        static int s_nextId = 1;
        return s_nextId++;
    }
};
