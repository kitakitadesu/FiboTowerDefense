# Concept: FIBO TD Game Architecture

**Core Idea**: Event-driven tower defense with state machine game flow, multi-phase boss fights, and encapsulated entity design.

**Key Points**:
- **Game States**: Playing, Paused, Won, Lost — transitions via tower HP (→ Lost) or all waves + boss defeated + final level (→ Won)
- **Event-Driven**: entt::dispatcher decouples systems — EnemyKilledEvent(position) triggers powerup spawn at death location
- **Encapsulation**: All entity fields private with getters. 100% no public data exposure across 8+ classes
- **Boss Phases**: 3 phases per boss — Phase 1 (100-66% HP), Phase 2 (66-33%, 2x speed), Phase 3 (33-0%, 3x speed)
- **Boss Variants**: Colossus (tank/AoE), ThunderWorm (DPS/lightning), PhantomKing (stealth/confuse)

**Game Flow**:
```
Game::update():
  if Playing:
    level.update()
    if tower destroyed → Lost
    if waves complete + boss dead + final level → Won
```

**Reference**: Full session at `SESSION_SUMMARY.md` (source, to be archived)

**Related**: `context/examples/boss-powerup.md`, `context/lookup/api-docs.md`
