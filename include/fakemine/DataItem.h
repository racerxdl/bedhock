#pragma once

#include "base.h"

enum class ActorDataIDs : unsigned short {
    FLAGS = 0,                       // Int64
    HEALTH = 1,                      // Int
    VARIANT = 2,                     // Int
    COLOR = 3,                       // Byte
    NAMETAG = 4,                     // String
    OWNER = 5,                       // Int64
    TARGET = 6,                      // Int64
    AIR = 7,                         // Short
    POTION_COLOR = 8,                // Int (ARGB!)
    POTION_AMBIENT = 9,              // Byte
    JUMP_DURATION = 10,              // Byte (not Int64?)
    HURT_TIME = 11,                  // Int (minecart/boat)
    HURT_DIRECTION = 12,             // Int (minecart/boat)
    PADDLE_TIME_LEFT = 13,           // Float
    PADDLE_TIME_RIGHT = 14,          // Float
    EXPERIENCE_VALUE = 15,           // Int (xp orb)
    DISPLAY_ITEM = 16,               // Byte    Int    CompoundTag
    HORSE_FLAGS = 16,                // Int, old
    WITHER_SKULL_IS_DANGEROUS = 16,  // Byte, old
    MINECART_DISPLAY_OFFSET = 17,    // Int    Int64    Vec3
    ARROW_SHOOTER_ID = 17,           // Int64, old
    MINECART_HAS_DISPLAY = 18,       // Byte    Int64
    HORSE_TYPE = 19,                 // Byte
    SWELL = 19,                      // old
    OLD_SWELL = 20,                  // old
    SWELL_DIR = 21,                  // old
    CHARGE_AMOUNT = 22,              // Byte
    ENDERMAN_HELD_RUNTIME_ID = 23,   // Int, (not Short?)
    ACTOR_AGE = 24,                  // Byte, (not Short?)
    // UNKNOWN_25                          = 25,  // 25 Byte Int
    PLAYER_FLAGS = 26,                        // Byte
    PLAYER_INDEX = 27,                        // Int
    PLAYER_BED_POSITION = 28,                 // BlockPos
    FIREBALL_POWER_X = 29,                    // Float
    FIREBALL_POWER_Y = 30,                    // Float
    FIREBALL_POWER_Z = 31,                    // Float
    AUX_POWER = 32,                           // old
    FISH_X = 33,                              // old
    FISH_Z = 34,                              // old
    FISH_ANGLE = 35,                          // old
    POTION_AUX_VALUE = 36,                    // Short
    LEAD_HOLDER = 37,                         // Int64
    SCALE = 38,                               // Float
    INTERACTIVE_TAG = 39,                     // Byte, (not String?)
    NPC_SKIN_ID = 40,                         // String
    URL_TAG = 41,                             // String
    MAX_AIR = 42,                             // Short
    MARK_VARIANT = 43,                        // Int
    CONTAINER_TYPE = 44,                      // Byte
    CONTAINER_BASE_SIZE = 45,                 // Int
    CONTAINER_EXTRA_SLOTS_PER_STRENGTH = 46,  // Int
    BLOCK_TARGET = 47,                        // BlockPos (ENDER CRYSTAL)
    WITHER_INVULNERABLE_TICKS = 48,           // Int
    WITHER_TARGET_1 = 49,                     // Int64
    WITHER_TARGET_2 = 50,                     // Int64
    WITHER_TARGET_3 = 51,                     // Int64
    AERIAL_ATTACK = 52,                       // Short
    BOUNDING_BOX_WIDTH = 53,                  // Float
    BOUNDING_BOX_HEIGHT = 54,                 // Float
    FUSE_LENGTH = 55,                         // Int
    RIDER_SEAT_POSITION = 56,                 // Vec3
    RIDER_ROTATION_LOCKED = 57,               // Byte
    RIDER_MAX_ROTATION = 58,                  // Float
    RIDER_MIN_ROTATION = 59,                  // Byte, (not Float?)
    // UNKNOWN_60                          = 60,  // Float
    AREA_EFFECT_CLOUD_RADIUS = 61,       // Float
    AREA_EFFECT_CLOUD_WAITING = 62,      // Int
    AREA_EFFECT_CLOUD_PARTICLE_ID = 63,  // Int
    SHULKER_PEEK_ID = 64,                // Int
    SHULKER_ATTACH_FACE = 65,            // Byte
    SHULKER_ATTACHED = 66,               // Short
    SHULKER_ATTACH_POS = 67,             // BlockPos
    TRADING_PLAYER_EID = 68,             // Int64
    TRADING_CAREER = 69,
    HAS_COMMAND_BLOCK = 70,              // Byte
    COMMAND_BLOCK_COMMAND = 71,          // String
    COMMAND_BLOCK_LAST_OUTPUT = 72,      // String
    COMMAND_BLOCK_TRACK_OUTPUT = 73,     // Byte
    CONTROLLING_RIDER_SEAT_NUMBER = 74,  // Byte
    STRENGTH = 75,                       // Int
    MAX_STRENGTH = 76,                   // Int
    SPELL_CASTING_COLOR = 77,            // Int
    LIMITED_LIFE = 78,                   // Int
    ARMOR_STAND_POSE_INDEX = 79,         // Int
    ENDER_CRYSTAL_TIME_OFFSET = 80,      // Int
    ALWAYS_SHOW_NAMETAG = 81,            // Byte
    COLOR_2 = 82,                        // Byte
    NAME_AUTHOR = 83,
    SCORE_TAG = 84,                // String
    BALLOON_ATTACHED_ENTITY = 85,  // Int64
    PUFFERFISH_SIZE = 86,          // Byte
    BUBBLE_TIME = 87,
    AGENT = 88,                    // Int64
    SITTING_AMOUNT = 89,           // Float
    SITTING_AMOUNT_PREVIOUS = 90,  // Float
    EATING_COUNTER = 91,           // Int
    FLAGS_EXTENDED = 92,           // Int64
    LAYING_AMOUNT = 93,            // Float
    LAYING_AMOUNT_PREVIOUS = 94,   // Float
    DURATION = 95,
    SPAWN_TIME = 96,
    CHANGE_RATE = 97,
    CHANGE_ON_PICKUP = 98,
    PICKUP_COUNT = 99,
    INTERACT_TEXT = 100,                        // String
    TRADE_TIER = 101,                           // Int
    MAX_TRADE_TIER = 102,                       // Int
    TRADE_EXPERIENCE = 103,                     // Int
    SKIN_ID = 104,                              // Int
    SPAWNING_FRAMES = 105,                      // Int
    COMMAND_BLOCK_TICK_DELAY = 106,             // Int
    COMMAND_BLOCK_EXECUTE_ON_FIRST_TICK = 107,  // Byte
    AMBIENT_SOUND_INTERVAL = 108,               // Float
};

enum class DataItemType : unsigned char {
    Byte = 0,
    Short = 1,
    Int = 2,
    Float = 3,
    String = 4,
    CompoundTag = 5,
    BlockPos = 6,
    Int64 = 7,
    Vec3 = 8,
};

class DataItem {
   public:
    DataItemType mType;  // 8
    unsigned short mId;  // 10
    bool mDirty = true;  // 12

    template <typename T>
    inline T const& getData() const;
    template <typename T>
    inline T& getData();
    template <typename T>
    inline bool setData(T const& value);

    DataItem(DataItemType type, unsigned short id)
        : mId(id), mType(type) {
    }

    template <typename T>
    inline static std::unique_ptr<DataItem> create(unsigned short key, T const& value);

    template <typename T>
    inline static std::unique_ptr<DataItem> create(ActorDataIDs key, T const& value);
    class DataItem& operator=(class DataItem const&) = delete;
    DataItem(class DataItem const&) = delete;
    DataItem() = delete;
    virtual ~DataItem();
    virtual bool isDataEqual(class DataItem const&) const;
    virtual std::unique_ptr<DataItem> clone() const = 0;
};
