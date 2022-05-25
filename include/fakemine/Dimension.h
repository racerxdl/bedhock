#pragma once

#include "base.h"

class Dimension {

public:
    class Dimension& operator=(class Dimension const &) = delete;
    Dimension(class Dimension const &) = delete;
    Dimension() = delete;

    Dimension(class ILevel &, class AutomaticID<class Dimension, int>, class DimensionHeightRange, class Scheduler &, std::string);
    void addActorChunkTransferToQueue(class ChunkPos &, class ChunkPos &);
    void addActorUnloadedChunkTransferToQueue(class ChunkPos const &, class ChunkPos const &, class AutomaticID<class Dimension, int>, std::string &, std::unique_ptr<class CompoundTag>);
    void addWither(struct ActorUniqueID const &);
    float distanceToNearestPlayerSqr2D(class Vec3);
    class Actor * fetchEntity(struct ActorUniqueID, bool);
    class Player * fetchNearestAttackablePlayer(class Actor &, float);
    class Player * fetchNearestAttackablePlayer(class BlockPos, float, class Actor *);
    class Player * fetchNearestPlayer(class Actor &, float);
    class Player * fetchNearestPlayer(float, float, float, float, bool);
    class Player * findPlayer(class std::function<bool (class Player const &)>) const;
    void flagEntityforChunkMove(class Actor &);
    void flushLevelChunkGarbageCollector();
    void flushRunTimeLighting();
    void forEachPlayer(class std::function<bool (class Player &)>) const;
    void forceSaveVillageManager();
    class BlockEventDispatcher & getBlockEventDispatcher();
    class BlockSource & getBlockSourceFromMainChunkSource() const;
    class ChunkBuildOrderPolicyBase & getChunkBuildOrderPolicy();
    // class gsl::not_null<class ChunkLoadActionList *> getChunkLoadActionList();
    class ChunkSource & getChunkSource() const;
    class CircuitSystem & getCircuitSystem();
    // class gsl::not_null<class DelayActionList *> getDelayActionList();
    class AutomaticID<class Dimension, int> getDimensionId() const;
    std::vector<class WeakEntityRef> & getDisplayEntities();
    // class std::unordered_map<struct ActorUniqueID, class WeakEntityRef, struct std::hash<struct ActorUniqueID>, struct std::equal_to<struct ActorUniqueID>, class std::allocator<struct std::pair<struct ActorUniqueID const, class WeakEntityRef>>> & getEntityIdMap();
    // class std::unordered_map<struct ActorUniqueID, class WeakEntityRef, struct std::hash<struct ActorUniqueID>, struct std::equal_to<struct ActorUniqueID>, class std::allocator<struct std::pair<struct ActorUniqueID const, class WeakEntityRef>>> const & getEntityIdMapConst() const;
    class FeatureTerrainAdjustments & getFeatureTerrainAdjustments();
    short getHeight() const;
    unsigned short getHeightInSubchunks() const;
    class DimensionHeightRange const & getHeightRange() const;
    class Level & getLevel() const;
    class Level const & getLevelConst() const;
    short getMinHeight() const;
    float getMoonBrightness() const;
    int getMoonPhase() const;
    float getPopCap(int, bool) const;
    struct Brightness getSkyDarken() const;
    float getSunAngle(float) const;
    class TickingAreaList & getTickingAreas();
    class TickingAreaList const & getTickingAreasConst() const;
    float getTimeOfDay(float) const;
    std::unique_ptr<class VillageManager> const & getVillageManager() const;
    class Weather & getWeather() const;
    class WorldGenerator * getWorldGenerator() const;
    bool hasCeiling() const;
    bool hasSkylight() const;
    bool isChunkKnown(class ChunkPos const &) const;
    bool isLeaveGameDone();
    bool isRedstoneTick();
    bool isUltraWarm() const;
    void onStaticTickingAreaAdded(std::string const &);
    // void registerEntity(struct ActorUniqueID const &, class WeakRefT<struct EntityRefTraits>);
    void removeActorByID(struct ActorUniqueID const &);
    void removeWither(struct ActorUniqueID const &);
    void sendPacketForEntity(class Actor const &, class Packet const &, class Player const *);
    void sendPacketForPosition(class BlockPos const &, class Packet const &, class Player const *);
    void sendPacketToClients(class Packet const &, std::vector<struct NetworkIdentifierWithSubId>);
    void setCeiling(bool);
    void setSkylight(bool);
    void setUltraWarm(bool);
    void transferEntity(class ChunkPos const &, class Vec3 const &, std::unique_ptr<class CompoundTag>, bool);
    void transferEntityToUnloadedChunk(class Actor &);
    void transferEntityToUnloadedChunk(class ChunkPos const &, class ChunkPos const &, class AutomaticID<class Dimension, int>, std::string &, std::unique_ptr<class CompoundTag>);
    bool tryAssignNewRegionAt(class ChunkPos const &, class Actor &);
    void tryGarbageCollectStructures();
    class BlockSource * tryGetClosestPublicRegion(class ChunkPos const &) const;
    void tryLoadLimboEntities(class ChunkPos const &);
    // void unregisterDisplayEntity(class WeakRefT<struct EntityRefTraits>);
    void unregisterEntity(struct ActorUniqueID const &);
    // static enum LimboEntitiesVersion const CurrentLimboEntitiesVersion;
    static unsigned int const LOW_CPU_PACKET_BLOCK_LIMIT;
    static float const *const MOON_BRIGHTNESS_PER_PHASE;
    // static class std::chrono::duration<__int64, struct std::ratio<1, 1>> const STRUCTURE_PRUNE_INTERVAL;
    void _processEntityChunkTransfers();
    void _sendBlockEntityUpdatePacket(class NetworkBlockPosition const &);
    void _sendBlocksChangedPackets();
    void _tickEntityChunkMoves();

};