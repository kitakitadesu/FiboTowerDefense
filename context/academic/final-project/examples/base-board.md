# Example: Base Board Class

**Core Idea**: Immutable base class for the game board.

<critical_rule>
DO NOT MODIFY THIS FILE. Future agents must inherit from this class, not change it.
</critical_rule>

**Key Points**:
- Provides grid data storage (`std::vector<std::vector<char>>`)
- Handles cell access and validation
- Used as a base for inheritance requirements

**Quick Example**:
```cpp
class GameBoard : public Board { // Inherit from this
    // Implementation
};
```

**Files**:
- `examples/board.h`
- `examples/board.cpp`

**Reference**: User provided source (`fibo_td/Board`)
