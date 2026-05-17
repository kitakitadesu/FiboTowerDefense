# FIBO TD API Reference

This document contains all the functions defined in the codebase along with their usage counts to help identify heavily used APIs and potential dead code.

## Boss

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `public:
    const std::variant<Tank, DPS, Stealth>&` | `getBossType` | `` | 1 |
| `int` | `getCurrentPhase` | `` | 1 |
| `int` | `getPhaseScaledDamage` | `int baseDamage` | 2 |
| `bool` | `isPhaseThree` | `` | 1 |
| `bool` | `isPhaseTwo` | `` | 1 |

## Cell

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `draw` | `` | 14 |
| `const CellData&` | `getCellData` | `` | 14 |
| `CellData&` | `getCellData` | `` | 14 |
| `int` | `getX` | `` | 2 |
| `int` | `getY` | `` | 2 |
| `void` | `setCellData` | `CellData data` | 6 |
| `void` | `setPosition` | `int x, int y` | 2 |

## Enemy

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `applyEffect` | `const EffectData& effect` | 4 |
| `void` | `draw` | `` | 14 |
| `const std::vector<EffectData>&` | `getActiveEffects` | `` | 1 |
| `int` | `getDamage` | `` | 4 |
| `std::string` | `getDisplayName` | `` | 7 |
| `const EnemyData&` | `getEnemyData` | `` | 2 |
| `int` | `getHp` | `` | 9 |
| `int` | `getId` | `` | 15 |
| `int` | `getMaxHp` | `` | 4 |
| `const raylib::Vector2&` | `getPosition` | `` | 7 |
| `int` | `getReward` | `` | 2 |
| `float` | `getSpeed` | `` | 2 |
| `float` | `getSpeedMultiplier` | `` | 1 |
| `const std::vector<raylib::Vector2>&` | `getWaypoints` | `` | 1 |
| `bool` | `isFlying` | `` | 7 |
| `bool` | `isFrozen` | `` | 1 |
| `bool` | `isVisibleToTurret` | `` | 3 |
| `void` | `move` | `float dt` | 16 |
| `void` | `setFrozen` | `bool frozen` | 3 |
| `void` | `setSpeedMultiplier` | `float multiplier` | 3 |
| `void` | `takeDamage` | `int amount` | 4 |
| `void` | `update` | `float dt` | 25 |

## Game

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `addLevel` | `std::shared_ptr<Level> level` | 3 |
| `GameState` | `getGameState` | `` | 1 |
| `const std::vector<std::shared_ptr<Level>>&` | `getLevels` | `` | 1 |
| `void` | `handleGameOver` | `` | 3 |
| `void` | `handleGameWon` | `` | 3 |
| `bool` | `isBossDefeated` | `` | 1 |
| `bool` | `isRunning` | `` | 2 |
| `void` | `loadLevel` | `int levelIndex` | 3 |
| `void` | `nextLevel` | `` | 2 |
| `void` | `quit` | `` | 3 |
| `void` | `render` | `` | 6 |
| `void` | `setBossDefeated` | `bool defeated` | 1 |
| `void` | `setGameState` | `GameState state` | 1 |
| `void` | `start` | `` | 8 |
| `void` | `update` | `float dt` | 25 |

## Grid

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `InitCells` | `` | 3 |
| `size_t` | `colCount` | `` | 3 |
| `const Cell*` | `getCell` | `int r, int c` | 7 |
| `Cell*` | `getCell` | `int r, int c` | 7 |
| `const std::vector<std::vector<Cell>>&` | `getCells` | `` | 7 |
| `std::vector<std::vector<Cell>>&` | `getCells` | `` | 7 |
| `bool` | `placeTurret` | `int r, int c, std::shared_ptr<Turret> turret` | 2 |
| `bool` | `removeTurret` | `int r, int c` | 5 |
| `size_t` | `rowCount` | `` | 5 |
| `bool` | `setCellData` | `int r, int c, CellData data` | 6 |

## IDisplayName

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `std::string` | `getDisplayName` | `` | 7 |

## IIdentifier

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `int` | `getId` | `` | 15 |
| `return` | `getId` | `) == other.getId(` | 15 |
| `return` | `getId` | `) != other.getId(` | 15 |

## ISprite

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `draw` | `` | 14 |

## IdGenerator

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `int` | `getNextId` | `` | 4 |

## Level

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `addCurrency` | `int amount` | 3 |
| `void` | `addEnemy` | `std::shared_ptr<Enemy> enemy` | 3 |
| `void` | `addProjectile` | `std::shared_ptr<Projectile> projectile` | 3 |
| `void` | `addWave` | `std::shared_ptr<WaveManager> wave` | 2 |
| `void` | `applyPowerupEffect` | `PowerupType type` | 3 |
| `entt::dispatcher&` | `events` | `` | 1 |
| `const std::vector<std::shared_ptr<Powerup>>&` | `getActivePowerups` | `` | 1 |
| `int` | `getCurrency` | `` | 1 |
| `float` | `getCurrencyMultiplier` | `` | 1 |
| `Difficulty` | `getDifficulty` | `` | 1 |
| `const std::vector<std::shared_ptr<Enemy>>&` | `getEnemies` | `` | 2 |
| `Grid&` | `getGrid` | `` | 2 |
| `const Grid&` | `getGrid` | `` | 2 |
| `int` | `getId` | `` | 15 |
| `const std::vector<std::shared_ptr<Projectile>>&` | `getProjectiles` | `` | 1 |
| `Scoreboard&` | `getScoreboard` | `` | 1 |
| `Tower&` | `getTower` | `` | 1 |
| `const std::vector<std::shared_ptr<WaveManager>>&` | `getWaves` | `` | 1 |
| `` | `if` | `grid_.rowCount() > 0 && grid_.colCount() > 0` | 123 |
| `bool` | `isTowerDestroyed` | `` | 2 |
| `bool` | `isWaveComplete` | `` | 3 |
| `void` | `onEnemyKilled` | `const EnemyKilledEvent& event` | 3 |
| `bool` | `removeTurret` | `int x, int y` | 5 |
| `void` | `render` | `` | 6 |
| `void` | `setCurrencyMultiplier` | `float multiplier` | 3 |
| `void` | `spawnRandomPowerup` | `const raylib::Vector2& position` | 3 |
| `bool` | `spendCurrency` | `int amount` | 2 |
| `void` | `start` | `` | 8 |
| `` | `tower_` | `std::make_shared<Tower>(100, TowerData{MainBase{}})), nextWaveIndex_(0` | 11 |
| `void` | `update` | `float dt` | 25 |
| `void` | `updateBossAttacks` | `float dt` | 3 |

## Powerup

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `apply` | `Level& level` | 3 |
| `float` | `getDuration` | `` | 1 |
| `int` | `getId` | `` | 15 |
| `float` | `getRemainingTime` | `` | 1 |
| `PowerupType` | `getType` | `` | 2 |
| `bool` | `isActive` | `` | 1 |
| `bool` | `isExpired` | `` | 3 |
| `void` | `update` | `float dt` | 25 |

## PowerupDefinition

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `` | `switch` | `type` | 3 |

## Projectile

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `bool` | `checkCollision` | `const raylib::Vector2& enemyPosition, float enemyRadius` | 3 |
| `void` | `draw` | `` | 14 |
| `const std::optional<EffectData>&` | `getAppliedEffect` | `` | 5 |
| `float` | `getDamage` | `` | 4 |
| `int` | `getId` | `` | 15 |
| `const raylib::Vector2&` | `getPosition` | `` | 7 |
| `const ProjectileData&` | `getProjectileData` | `` | 3 |
| `float` | `getSpeed` | `` | 2 |
| `bool` | `hasImpacted` | `` | 2 |
| `void` | `setAppliedEffect` | `const EffectData& effect` | 2 |
| `void` | `update` | `float dt` | 25 |

## Scoreboard

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `addPoints` | `int points` | 3 |
| `void` | `addScore` | `const std::string& name, int score` | 2 |
| `int` | `getCurrentScore` | `` | 1 |
| `std::vector<ScoreEntry>` | `getTopScores` | `int count` | 2 |
| `void` | `load` | `const std::string& filename` | 2 |
| `void` | `save` | `const std::string& filename` | 2 |

## Tower

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `TakeDamage` | `int amount` | 2 |
| `void` | `draw` | `` | 14 |
| `std::string` | `getDisplayName` | `` | 7 |
| `int` | `getHp` | `` | 9 |
| `int` | `getMaxHp` | `` | 4 |
| `const TowerData&` | `getTowerData` | `` | 2 |
| `TowerData&` | `getTowerData` | `` | 2 |
| `bool` | `hasShield` | `` | 1 |
| `:` | `hp_` | `maxHp), maxHp_(maxHp), towerData_(std::move(data)` | 17 |
| `void` | `setHp` | `int hp` | 2 |
| `void` | `setShield` | `bool shield` | 3 |

## Turret

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `void` | `draw` | `` | 14 |
| `void` | `exchangeURWeapon` | `const URArmConfig& newConfig` | 2 |
| `float` | `getCooldown` | `` | 1 |
| `float` | `getDamageMultiplier` | `` | 1 |
| `std::string` | `getDisplayName` | `` | 7 |
| `float` | `getFireRateMultiplier` | `` | 1 |
| `int` | `getHp` | `` | 9 |
| `int` | `getId` | `` | 15 |
| `int` | `getLevel` | `` | 1 |
| `const RobotConfig&` | `getRobotConfig` | `` | 1 |
| `int` | `getSellValue` | `` | 1 |
| `const TurretStats&` | `getStats` | `` | 2 |
| `TurretStats&` | `getStats` | `` | 2 |
| `int` | `getUpgradeCost` | `` | 1 |
| `void` | `setDamageMultiplier` | `float multiplier` | 3 |
| `void` | `setFireRateMultiplier` | `float multiplier` | 3 |
| `void` | `update` | `float dt, const raylib::Vector2& position, Level& level` | 25 |
| `void` | `upgrade` | `` | 2 |
| `` | `upgradeCost_` | `upgradeCost), cooldown_(0.0f` | 5 |

## WaveManager

| Return Type | Function | Parameters | Usages |
|---|---|---|---|
| `float` | `getDifficultyMultiplier` | `` | 4 |
| `int` | `getEnemyCount` | `` | 1 |
| `int` | `getId` | `` | 15 |
| `std::shared_ptr<Enemy>` | `getNextEnemy` | `` | 2 |
| `float` | `getSpawnDelay` | `` | 1 |
| `int` | `getSpawnedCount` | `` | 1 |
| `bool` | `hasBoss` | `` | 5 |
| `bool` | `isComplete` | `` | 2 |
| `void` | `markSpawned` | `const std::shared_ptr<Enemy>& enemy` | 3 |
| `bool` | `shouldSpawn` | `` | 3 |
| `void` | `update` | `float dt` | 25 |

