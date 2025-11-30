#ifndef ASSET_HANDLER_TYPES
#define ASSET_HANDLER_TYPES

static const unsigned int INVALID_GENERATION_MASK = 0b1 << 31;
static const unsigned int GENERATION_ID_MASK = ~INVALID_GENERATION_MASK;

enum class AssetLoadingState
{
	UNLOADED = 0,
	LOADED
};

enum class AssetLoadType
{
	LOAD_WHEN_POSSIBLE,
	FORCE_LOAD
};

typedef struct
{
	const char* Path;
} Asset;

typedef struct
{
	Asset Fragment;
	Asset Vertex;
} ShaderAsset;


#endif // !ASSET_HANDLER_TYPES

