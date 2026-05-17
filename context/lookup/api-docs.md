# API Documentation

## Class `Enemy`

- `int getId()` - Usages found: 15
- `int getHp()` - Usages found: 9
- `int getMaxHp()` - Usages found: 4
- `float getSpeed()` - Usages found: 2
- `int getDamage()` - Usages found: 4
- `int getReward()` - Usages found: 2
- `bool isFlying()` - Usages found: 7
- `const EnemyData& getEnemyData()` - Usages found: 2
- `const std::vector<raylib::Vector2>& getWaypoints()` - Usages found: 1
- `const raylib::Vector2& getPosition()` - Usages found: 7
- `const std::vector<EffectData>& getActiveEffects()` - Usages found: 1
- `bool isFrozen()` - Usages found: 1
- `float getSpeedMultiplier()` - Usages found: 1
- `void setFrozen(bool frozen)` - Usages found: 3
- `void setSpeedMultiplier(float multiplier)` - Usages found: 3
- `void move(float dt)` - Usages found: 16
- `void takeDamage(int amount)` - Usages found: 4
- `void applyEffect(const EffectData& effect)` - Usages found: 4
- `bool isVisibleToTurret()` - Usages found: 3
- `void update(float dt)` - Usages found: 25
- `void draw()` - Usages found: 14
- `std::string getDisplayName()` - Usages found: 7

## Class `IdGenerator`

- `int getNextId()` - Usages found: 4

## Class `Game`

- `bool isRunning()` - Usages found: 2
- `GameState getGameState()` - Usages found: 1
- `const std::vector<std::shared_ptr<Level>>& getLevels()` - Usages found: 1
- `bool isBossDefeated()` - Usages found: 1
- `void start()` - Usages found: 8
- `void update(float dt)` - Usages found: 25
- `void render()` - Usages found: 6
- `void addLevel(std::shared_ptr<Level> level)` - Usages found: 3
- `void loadLevel(int levelIndex)` - Usages found: 3
- `void nextLevel()` - Usages found: 2
- `void quit()` - Usages found: 3
- `void setGameState(GameState state)` - Usages found: 1
- `void setBossDefeated(bool defeated)` - Usages found: 1
- `void handleGameOver()` - Usages found: 3
- `void handleGameWon()` - Usages found: 3

## Class `Boss`

- `bool isPhaseTwo()` - Usages found: 1
- `bool isPhaseThree()` - Usages found: 1
- `int getCurrentPhase()` - Usages found: 1
- `void advancePhase()` - Usages found: 2
- `bool shouldPhaseChange()` - Usages found: 2
- `int getPhaseScaledDamage(int baseDamage)` - Usages found: 2
- `const std::variant<Tank, DPS, Stealth>& getBossType()` - Usages found: 1

## Class `Grid`

- `void InitCells()` - Usages found: 3
- `bool placeTurret(int r, int c, std::shared_ptr<Turret> turret)` - Usages found: 2
- `bool removeTurret(int r, int c)` - Usages found: 5
- `size_t rowCount()` - Usages found: 5
- `size_t colCount()` - Usages found: 3
- `const std::vector<std::vector<Cell>>& getCells()` - Usages found: 7
- `std::vector<std::vector<Cell>>& getCells()` - Usages found: 7
- `const Cell* getCell(int r, int c)` - Usages found: 7
- `Cell* getCell(int r, int c)` - Usages found: 7
- `bool setCellData(int r, int c, CellData data)` - Usages found: 6

## Class `WaveManager`

- `int getId()` - Usages found: 15
- `int getEnemyCount()` - Usages found: 1
- `float getSpawnDelay()` - Usages found: 1
- `float getDifficultyMultiplier()` - Usages found: 4
- `bool hasBoss()` - Usages found: 5
- `bool isComplete()` - Usages found: 2
- `int getSpawnedCount()` - Usages found: 1
- `void update(float dt)` - Usages found: 25
- `bool shouldSpawn()` - Usages found: 3
- `void markSpawned(const std::shared_ptr<Enemy>& enemy)` - Usages found: 3
- `std::shared_ptr<Enemy> getNextEnemy()` - Usages found: 2

## Class `Scoreboard`

- `void addScore(const std::string& name, int score)` - Usages found: 2
- `void addPoints(int points)` - Usages found: 3
- `int getCurrentScore()` - Usages found: 1
- `std::vector<ScoreEntry> getTopScores(int count)` - Usages found: 2
- `void save(const std::string& filename)` - Usages found: 2
- `void load(const std::string& filename)` - Usages found: 2

## Class `Powerup`

- `int getId()` - Usages found: 15
- `PowerupType getType()` - Usages found: 2
- `float getDuration()` - Usages found: 1
- `float getRemainingTime()` - Usages found: 1
- `bool isExpired()` - Usages found: 3
- `bool isActive()` - Usages found: 1
- `void apply(Level& level)` - Usages found: 3
- `void update(float dt)` - Usages found: 25

## Class `Level`

- `int getId()` - Usages found: 15
- `Difficulty getDifficulty()` - Usages found: 1
- `Grid& getGrid()` - Usages found: 2
- `const Grid& getGrid()` - Usages found: 2
- `const std::vector<std::shared_ptr<Enemy>>& getEnemies()` - Usages found: 2
- `const std::vector<std::shared_ptr<Projectile>>& getProjectiles()` - Usages found: 1
- `const std::vector<std::shared_ptr<WaveManager>>& getWaves()` - Usages found: 1
- `const std::vector<std::shared_ptr<Powerup>>& getActivePowerups()` - Usages found: 1
- `int getCurrency()` - Usages found: 1
- `Tower& getTower()` - Usages found: 1
- `Scoreboard& getScoreboard()` - Usages found: 1
- `entt::dispatcher& events()` - Usages found: 1
- `float getCurrencyMultiplier()` - Usages found: 1
- `void setCurrencyMultiplier(float multiplier)` - Usages found: 3
- `void start()` - Usages found: 8
- `void update(float dt)` - Usages found: 25
- `void addCurrency(int amount)` - Usages found: 3
- `bool spendCurrency(int amount)` - Usages found: 2
- `bool removeTurret(int x, int y)` - Usages found: 5
- `void addEnemy(std::shared_ptr<Enemy> enemy)` - Usages found: 3
- `void addProjectile(std::shared_ptr<Projectile> projectile)` - Usages found: 3
- `void addWave(std::shared_ptr<WaveManager> wave)` - Usages found: 2
- `void render()` - Usages found: 6
- `bool isWaveComplete()` - Usages found: 3
- `bool isTowerDestroyed()` - Usages found: 2
- `void updateBossAttacks(float dt)` - Usages found: 3
- `void spawnRandomPowerup(const raylib::Vector2& position)` - Usages found: 3
- `void applyPowerupEffect(PowerupType type)` - Usages found: 3
- `void updatePowerups(float dt)` - Usages found: 2
- `void onEnemyKilled(const EnemyKilledEvent& event)` - Usages found: 3

## Class `Tower`

- `int getHp()` - Usages found: 9
- `int getMaxHp()` - Usages found: 4
- `const TowerData& getTowerData()` - Usages found: 2
- `TowerData& getTowerData()` - Usages found: 2
- `bool hasShield()` - Usages found: 1
- `void setHp(int hp)` - Usages found: 2
- `void setShield(bool shield)` - Usages found: 3
- `void draw()` - Usages found: 14
- `std::string getDisplayName()` - Usages found: 7
- `void TakeDamage(int amount)` - Usages found: 2

## Class `Cell`

- `int getX()` - Usages found: 2
- `int getY()` - Usages found: 2
- `const CellData& getCellData()` - Usages found: 14
- `CellData& getCellData()` - Usages found: 14
- `void setPosition(int x, int y)` - Usages found: 2
- `void setCellData(CellData data)` - Usages found: 6
- `void draw()` - Usages found: 14

## Class `ISprite`

- `void draw()` - Usages found: 14

## Class `PowerupDefinition`

- ` switch(type)` - Usages found: 3

## Class `Turret`

- ` upgradeCost_(upgradeCost), cooldown_(0.0f)` - Usages found: 5
- `int getId()` - Usages found: 15
- `int getHp()` - Usages found: 9
- `int getLevel()` - Usages found: 1
- `int getUpgradeCost()` - Usages found: 1
- `const TurretStats& getStats()` - Usages found: 2
- `TurretStats& getStats()` - Usages found: 2
- `const RobotConfig& getRobotConfig()` - Usages found: 1
- `float getCooldown()` - Usages found: 1
- `float getFireRateMultiplier()` - Usages found: 1
- `float getDamageMultiplier()` - Usages found: 1
- `void setFireRateMultiplier(float multiplier)` - Usages found: 3
- `void setDamageMultiplier(float multiplier)` - Usages found: 3
- `void upgrade()` - Usages found: 2
- `void exchangeURWeapon(const URArmConfig& newConfig)` - Usages found: 2
- `int getSellValue()` - Usages found: 1
- `void draw()` - Usages found: 14
- `std::string getDisplayName()` - Usages found: 7
- `void update(float dt, const raylib::Vector2& position, Level& level)` - Usages found: 25

## Class `Projectile`

- `int getId()` - Usages found: 15
- `const raylib::Vector2& getPosition()` - Usages found: 7
- `float getDamage()` - Usages found: 4
- `float getSpeed()` - Usages found: 2
- `const ProjectileData& getProjectileData()` - Usages found: 3
- `const std::optional<EffectData>& getAppliedEffect()` - Usages found: 5
- `bool hasImpacted()` - Usages found: 2
- `void update(float dt)` - Usages found: 25
- `void setAppliedEffect(const EffectData& effect)` - Usages found: 2
- `bool checkCollision(const raylib::Vector2& enemyPosition, float enemyRadius)` - Usages found: 3
- `void draw()` - Usages found: 14

## Class `IIdentifier`

- `int getId()` - Usages found: 15
- `return getId() == other.getId()` - Usages found: 15
- `return getId() != other.getId()` - Usages found: 15

## Class `IDisplayName`

- `std::string getDisplayName()` - Usages found: 7
