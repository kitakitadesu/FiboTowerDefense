# Example: Boss & Powerup Patterns

## Boss Multi-Phase Template

Boss inherits Enemy, uses `BossStats` struct for phase thresholds.

```cpp
// Phase advancement on HP threshold
if (shouldPhaseChange()) {
    advancePhase();
    // Phase 2: 2x attack speed (33% faster cooldown)
    // Phase 3: 3x attack speed (50% faster cooldown)
}
const int damage = getPhaseScaledDamage(baseDamage);
```

## Boss Variants (std::variant)

```cpp
struct Tank {};    // Colossus — shockwave AoE
struct DPS {};     // ThunderWorm — lightning on turrets
struct Stealth {}; // PhantomKing — invisible, confuses turrets

using BossType = std::variant<Tank, DPS, Stealth>;
```

## Powerup System — Weighted Rarity

| Type | Effect | Duration | Rarity Weight |
|------|--------|----------|---------------|
| RapidFire | 2x attack speed | 15s | 5 |
| MegaDamage | 2x damage | 15s | 6 |
| ShieldBoost | -50% incoming dmg | 20s | 7 |
| CurrencyBoost | 2x currency | 20s | 4 |
| SlowAura | -30% enemy speed | 25s | 5 |
| HealthRestore | +25% tower HP | Instant | 3 |
| TurretFreeze | Freeze all enemies | 3s | 9 |

**Spawn**: 30% chance on enemy kill via `Level::spawnRandomPowerup(position)`. Uses weighted random selection by rarity.

**Event Chain**:
```
Projectile hits Enemy → Enemy dies
  → EnemyKilledEvent(position) → Level.onEnemyKilled()
  → Level.spawnRandomPowerup(position)
  → Level.applyPowerupEffect(type)
```

**Reference**: Full details in `SESSION_SUMMARY.md` (source, to be archived)

**Related**: `context/concepts/game-architecture.md`, `context/lookup/api-docs.md`
