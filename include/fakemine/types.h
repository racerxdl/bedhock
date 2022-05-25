#pragma once

#include <cstdint>

enum class ActorType;
enum class LevelSoundEvent;
enum class ActorLocation;
enum class InputMode;
enum class MaterialType;
enum class ActorDamageCause;
enum class ActorEvent;
enum class ArmorSlot;
enum class ArmorMaterialType;
enum class ArmorTextureType;
enum class ItemUseMethod;
enum class CommandPermissionLevel;
enum class AnimationComponentGroup;
enum class EquipmentSlot;
enum class PaletteColor;
enum class PortalAxis;
enum class ActorFlags;
enum class ActorCategory;
enum class BedSleepingResult;
enum class GameType;
enum class ClientPlayMode;
enum class MovementEventType;
enum class ContainerID;
enum class CrashDumpLogStringID;
enum class StreamReadResult;

class ItemStack;
class UpdateEquipPacket;
class UpdateTradePacket;
class ServerPlayer;
class Player;
class NetworkIdentifier;
class UserEntityIdentifierComponent;
class Certificate;
class Container;
class CompoundTag;
class LoopbackPacketSender;
class Actor;

struct ServerNetworkController {};
struct GameCallbacks {};
struct Scheduler {};
struct PrivateKeyManager {};
struct MinecraftCommands {};
struct ActiveTransfersManager {};
struct ClassroomModeNetworkHandler {};
struct BinaryStream;
struct ReadOnlyBinaryStream;
class NetworkIdentifier;
struct NetEventCallback {};
struct NetworkHandler {};

class ClientBlobCache {
   public:
    struct Server {
        class TransferBuilder;
        class ActiveTransfersManager;
    };
};

enum class ContainerType : char {
    INVENTORY = -1,
    NONE = -9,
    CONTAINER = 0,
    WORKBENCH = 1,
    FURNACE = 2,
    ENCHANTMENT = 3,
    BREWING_STAND = 4,
    ANVIL = 5,
    DISPENSER = 6,
    DROPPER = 7,
    HOPPER = 8,
    CAULDRON = 9,
    MINECART_CHEST = 10,
    MINECART_HOPPER = 11,
    HORSE = 12,
    TRADE = 15,
    MINECART_COMMAND_BLOCK = 16,
    JUKEBOX = 17,
    COMPOUND_CREATOR = 20,
    ELEMENT_CONSTRUCTOR = 21,
    MATERIAL_REDUCER = 22,
    LAB_TABLE = 23,
    BLAST_FURNACE = 27,
    SMOKER = 28,
    STONECUTTER = 29,
    HUD = 31,
    SMITHING_TABLE = 33,
};

enum class UpdateBlockLayer : int {
    UpdateBlockDefault,
    UpdateBlockLiquid
};

enum class UpdateBlockFlags : int {
    UpdateBlockNone = 0,
    BlockUpdateNeighbors = 1,
    BlockUpdateNetwork = 2,
    BlockUpdateAll = 3,
    BlockUpdateNoGraphic = 4,
    BlockUpdatePriority = 8,
    BlockUpdateAllPriority = 11
};

enum class TextType : char {
    RAW = 0,
    CHAT = 1,
    TRANSLATION = 2,
    POPUP = 3,
    JUKEBOX_POPUP = 4,
    TIP = 5,
    SYSTEM = 6,
    WHISPER = 7,
    ANNOUNCEMENT = 8,
    JSON_WHISPER = 9,
    JSON = 10
};

enum class TitleType : int {
    Clear = 0,
    Reset = 1,
    SetTitle = 2,
    SetSubtitle = 3,
    SetActionBar = 4,
    SetDurations = 5,
    TitleTextObject = 6,
    SubtitleTextObject = 7,
    ActionbarTextObject = 8
};
enum class BossEvent : int {
    Show = 0,
    RegisterPlayer = 1,
    Hide = 2,
    UnregisterPlayer = 3,
    HealthPercentage = 4,
    Title = 5,
    AppearanceProperties = 6,
    Texture = 7,
    ResendRaidBossEventData = 8,
};

enum class BossEventColour : int {
    Grey = 0,
    Blue = 1,
    Red = 2,
    Green = 3,
    Yellow = 4,
    Purple = 5,
    White = 6
};

enum class ContainerEnumName : int8_t {
    AnvilInputContainer = 0,
    AnvilMaterialContainer = 1,
    AnvilResultPreviewContainer = 2,
    SmithingTableInputContainer = 3,
    SmithingTableMaterialContainer = 4,
    SmithingTableResultPreviewContainer = 5,
    ArmorContainer = 6,
    LevelEntityContainer = 7,
    BeaconPaymentContainer = 8,
    BrewingStandInputContainer = 9,
    BrewingStandResultContainer = 10,
    BrewingStandFuelContainer = 11,
    CombinedHotbarAndInventoryContainer = 12,
    CraftingInputContainer = 13,
    CraftingOutputPreviewContainer = 14,
    RecipeConstructionContainer = 15,
    RecipeNatureContainer = 16,
    RecipeItemsContainer = 17,
    RecipeSearchContainer = 18,
    RecipeSearchBarContainer = 19,
    RecipeEquipmentContainer = 20,
    EnchantingInputContainer = 21,
    EnchantingMaterialContainer = 22,
    FurnaceFuelContainer = 23,
    FurnaceIngredientContainer = 24,
    FurnaceResultContainer = 25,
    HorseEquipContainer = 26,
    HotbarContainer = 27,
    InventoryContainer = 28,
    ShulkerBoxContainer = 29,
    TradeIngredient1Container = 30,
    TradeIngredient2Container = 31,
    TradeResultPreviewContainer = 32,
    OffhandContainer = 33,
    CompoundCreatorInput = 34,
    CompoundCreatorOutputPreview = 35,
    ElementConstructorOutputPreview = 36,
    MaterialReducerInput = 37,
    MaterialReducerOutput = 38,
    LabTableInput = 39,
    LoomInputContainer = 40,
    LoomDyeContainer = 41,
    LoomMaterialContainer = 42,
    LoomResultPreviewContainer = 43,
    BlastFurnaceIngredientContainer = 44,
    SmokerIngredientContainer = 45,
    Trade2Ingredient1Container = 46,
    Trade2Ingredient2Container = 47,
    Trade2ResultPreviewContainer = 48,
    GrindstoneInputContainer = 49,
    GrindstoneAdditionalContainer = 50,
    GrindstoneResultPreviewContainer = 51,
    StonecutterInputContainer = 52,
    StonecutterResultPreviewContainer = 53,
    CartographyInputContainer = 54,
    CartographyAdditionalContainer = 55,
    CartographyResultPreviewContainer = 56,
    BarrelContainer = 57,
    CursorContainer = 58,
    CreatedOutputContainer = 59
};

enum class ActorDamageCause : int {
    Override = 0x0,
    Contact = 0x1,
    EntityAttack = 0x2,
    Projectile = 0x3,
    Suffocation = 0x4,
    Fall = 0x5,
    Fire = 0x6,
    FireTick = 0x7,
    Lava = 0x8,
    Drowning = 0x9,
    BlockExplosion = 0x0A,
    EntityExplosion = 0x0B,
    Void = 0x0C,
    Suicide = 0x0D,
    Magic = 0x0E,
    Wither = 0x0F,
    Starve = 0x10,
    Anvil = 0x11,
    Thorns = 0x12,
    FallingBlock = 0x13,
    Piston = 0x14,
    FlyIntoWall = 0x15,
    Magma = 0x16,
    Fireworks = 0x17,
    Lightning = 0x18,
    Charging = 0x19,
    Temperature = 0x1A,
    All = 0x1F,
    None = -1,
};

enum class MinecraftPacketIds : int {
    Login = 0x01,
    PlayStatus = 0x02,
    ServerToClientHandshake = 0x03,
    ClientToServerHandshake = 0x04,
    Disconnect = 0x05,
    ResourcePacksInfo = 0x06,
    ResourcePackStack = 0x07,
    ResourcePackClientResponse = 0x08,
    Text = 0x09,
    SetTime = 0x0A,
    StartGame = 0x0B,
    AddPlayer = 0x0C,
    AddActor = 0x0D,
    RemoveActor = 0x0E,
    AddItemActor = 0x0F,
    TakeItemActor = 0x11,
    MoveActorAbsolute = 0x12,
    MovePlayer = 0x13,
    PassengerJump = 0x14,
    UpdateBlock = 0x15,
    AddPainting = 0x16,
    TickSync = 0x17,
    LevelSoundEventV1 = 0x18,
    LevelEvent = 0x19,
    BlockEvent = 0x1A,
    ActorEvent = 0x1B,
    MobEffect = 0x1C,
    UpdateAttributes = 0x1D,
    InventoryTransaction = 0x1E,
    MobEquipment = 0x1F,
    MobArmorEquipment = 0x20,
    Interact = 0x21,
    BlockPickRequest = 0x22,
    ActorPickRequest = 0x23,
    PlayerAction = 0x24,
    HurtArmor = 0x26,
    SetActorData = 0x27,
    SetActorMotion = 0x28,
    SetActorLink = 0x29,
    SetHealth = 0x2A,
    SetSpawnPosition = 0x2B,
    Animate = 0x2C,
    Respawn = 0x2D,
    ContainerOpen = 0x2E,
    ContainerClose = 0x2F,
    PlayerHotbar = 0x30,
    InventoryContent = 0x31,
    InventorySlot = 0x32,
    ContainerSetData = 0x33,
    CraftingData = 0x34,
    CraftingEvent = 0x35,
    GuiDataPickItem = 0x36,
    AdventureSettings = 0x37,
    BlockActorData = 0x38,
    PlayerInput = 0x39,
    LevelChunk = 0x3A,
    SetCommandsEnabled = 0x3B,
    SetDifficulty = 0x3C,
    ChangeDimension = 0x3D,
    SetPlayerGameType = 0x3E,
    PlayerList = 0x3F,
    SimpleEvent = 0x40,
    Event = 0x41,
    SpawnExperienceOrb = 0x42,
    ClientboundMapItemData = 0x43,
    MapInfoRequest = 0x44,
    RequestChunkRadius = 0x45,
    ChunkRadiusUpdated = 0x46,
    ItemFrameDropItem = 0x47,
    GameRulesChanged = 0x48,
    Camera = 0x49,
    BossEvent = 0x4A,
    ShowCredits = 0x4B,
    AvailableCommands = 0x4C,
    CommandRequest = 0x4D,
    CommandBlockUpdate = 0x4E,
    CommandOutput = 0x4F,
    UpdateTrade = 0x50,
    UpdateEquip = 0x51,
    ResourcePackDataInfo = 0x52,
    ResourcePackChunkData = 0x53,
    ResourcePackChunkRequest = 0x54,
    Transfer = 0x55,
    PlaySound = 0x56,
    StopSound = 0x57,
    SetTitle = 0x58,
    AddBehaviorTree = 0x59,
    StructureBlockUpdate = 0x5A,
    ShowStoreOffer = 0x5B,
    PurchaseReceipt = 0x5C,
    PlayerSkin = 0x5D,
    SubClientLogin = 0x5E,
    AutomationClientConnect = 0x5F,
    SetLastHurtBy = 0x60,
    BookEdit = 0x61,
    NpcRequest = 0x62,
    PhotoTransfer = 0x63,
    ModalFormRequest = 0x64,
    ModalFormResponse = 0x65,
    ServerSettingsRequest = 0x66,
    ServerSettingsResponse = 0x67,
    ShowProfile = 0x68,
    SetDefaultGameType = 0x69,
    RemoveObjective = 0x6A,
    SetDisplayObjective = 0x6B,
    SetScore = 0x6C,
    LabTable = 0x6D,
    UpdateBlockSynced = 0x6E,
    MoveActorDelta = 0x6F,
    SetScoreboardIdentity = 0x70,
    SetLocalPlayerAsInitialized = 0x71,
    UpdateSoftEnum = 0x72,
    NetworkStackLatency = 0x73,
    ScriptCustomEvent = 0x75,
    SpawnParticleEffect = 0x76,
    AvailableActorIdentifiers = 0x77,
    LevelSoundEventV2 = 0x78,
    NetworkChunkPublisherUpdate = 0x79,
    BiomeDefinitionList = 0x7A,
    LevelSoundEvent = 0x7B,
    LevelEventGeneric = 0x7C,
    LecternUpdate = 0x7D,
    AddEntity = 0x7F,
    RemoveEntity = 0x80,
    ClientCacheStatus = 0x81,
    OnScreenTextureAnimation = 0x82,
    MapCreateLockedCopy = 0x83,
    StructureTemplateDataRequest = 0x84,
    StructureTemplateDataResponse = 0x85,
    ClientCacheBlobStatus = 0x87,
    ClientCacheMissResponse = 0x88,
    EducationSettings = 0x89,
    Emote = 0x8A,
    MultiplayerSettings = 0x8B,
    SettingsCommand = 0x8C,
    AnvilDamage = 0x8D,
    CompletedUsingItem = 0x8E,
    NetworkSettings = 0x8F,
    PlayerAuthInput = 0x90,
    CreativeContent = 0x91,
    PlayerEnchantOptions = 0x92,
    ItemStackRequest = 0x93,
    ItemStackResponse = 0x94,
    PlayerArmorDamage = 0x95,
    CodeBuilder = 0x96,
    UpdatePlayerGameType = 0x97,
    EmoteList = 0x98,
    PositionTrackingDBServerBroadcast = 0x99,
    PositionTrackingDBClientRequest = 0x9A,
    DebugInfo = 0x9B,
    PacketViolationWarning = 0x9C,
    MotionPredictionHints = 0x9D,
    AnimateEntity = 0x9E,
    CameraShake = 0x9F,
    PlayerFog = 0xA0,
    CorrectPlayerMovePrediction = 0xA1,
    ItemComponent = 0xA2,
    FilterText = 0xA3,
    ClientboundDebugRenderer = 0xA4,
    SyncActorProperty = 0xA5,
    AddVolumeEntity = 0xA6,
    RemoveVolumeEntity = 0xA7,
    SimulationType = 0xA8,
    NpcDialogue = 0xA9,
    EduUriResource = 0xAA,
    CreatePhoto = 0xAB,
    UpdateSubChunkBlocks = 0xAC,
    PhotoInfoRequest = 0xAD,
    SubChunk = 0xAE,
    SubChunkRequest = 0xAF,
    PlayerStartItemCooldown = 0xB0,
    ScriptMessage = 0xB1,
    CodeBuilderSource = 0xB2,
    TickingAreasLoadStatus = 0xB3,
    DimensionData = 0xB4,
    AgentActionEvent = 0xB5,
    ChangeMobProperty = 0xB6
};