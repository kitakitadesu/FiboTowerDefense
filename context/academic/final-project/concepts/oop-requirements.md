# Concept: OOP Requirements

**Core Idea**: Strict adherence to Object-Oriented Programming principles as a core grading metric.

**Key Points**:
- Correct use of Class, Inheritance, and Encapsulation
- Implement ≥2 additional classes (excluding provided base classes)
- Inherit from base classes in ≥2 locations
- Runtime state modification (add/remove objects from collections)

**Quick Example**:
```cpp
class FiboTower : public Board { // Inheritance from base
public:
    void placeUnit(int r, int c) {
        if (IsValid(r, c)) SetCell(r, c, 'T'); // Encapsulation/State mod
    }
};
```

**Reference**: Final Project Criteria PDF, Page 1
