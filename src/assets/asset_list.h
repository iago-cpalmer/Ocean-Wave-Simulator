#ifndef ASSET_LIST_H
#define ASSET_LIST_H

#include "asset_handler_types.h"
#include "../renderer/data/model.h"
#include "public/assets_handler.h"
#include "../core/ErrorHandler.h"

// THIS ASSET LIST WILL BE PARTICULAR AND IMPLEMENTED DIFFERENTLY FOR EACH
// GAME. MAYBE ONE GAME WANTS TO HAVE THE ASSETS MANAGED BY SCENES OR ZONES
// AND SOME OTHER GAMES MAY WANT TO HAVE THEM PERMANENTLY LOADED OR WHATEVER

typedef struct
{
	Asset AssetData;

	// Type of handle stored doesn't have to be stored as functions already know the types they are handling
	union
	{
		ModelHandle ModHandle;
		TextureHandle TexHandle;
		ShaderHandle ShadHandle;
	} Handle;
} AssetState;

typedef struct
{
	ShaderAsset AssetData;
	ShaderHandle ShadHandle;
} ShaderAssetState;

// ------------------------------------
// Textures 
// ------------------------------------
enum class TextureName
{
	point_light,
	COUNT
};
static AssetState _TextureAssets[] =
{
	AssetState{ Asset{ "res/images/point_light.png"} }
};


inline TextureHandle al_get_texture_handle(TextureName texture)
{
	return _TextureAssets[(unsigned int)texture].Handle.TexHandle;
}

void al_lazy_load_textures()
{
	for (unsigned int i = 0; i < (unsigned int)TextureName::COUNT; i++)
	{
		TextureHandle handle = ah_register_texture(_TextureAssets[i].AssetData, AssetLoadType::LOAD_WHEN_POSSIBLE);
		_TextureAssets[i].Handle.TexHandle = handle;
	}
}

void al_load_textures()
{
	for (unsigned int i = 0; i < (unsigned int)TextureName::COUNT; i++)
	{
		TextureHandle handle = ah_register_texture(_TextureAssets[i].AssetData, AssetLoadType::FORCE_LOAD);
		_TextureAssets[i].Handle.TexHandle = handle;
	}
}

// ------------------------------------
// Shaders 
// ------------------------------------
enum class ShaderName
{
	basic_shader,
	
	COUNT
};
static ShaderAssetState _ShaderAssets[] =
{
	ShaderAssetState{ ShaderAsset { Asset{ "res/shaders/basic_shader.frag"}, Asset{ "res/shaders/basic_shader.vert"}} }
};


inline ShaderHandle al_get_shader_handle(ShaderName shader)
{
	return _ShaderAssets[(unsigned int)shader].ShadHandle;
}

void al_lazy_load_shaders()
{
	for (unsigned int i = 0; i < (unsigned int)ShaderName::COUNT; i++)
	{
		ShaderHandle handle = ah_register_shader(_ShaderAssets[i].AssetData, AssetLoadType::LOAD_WHEN_POSSIBLE);
		_ShaderAssets[i].ShadHandle = handle;
	}
}

void al_load_shaders()
{
	for (unsigned int i = 0; i < (unsigned int)ShaderName::COUNT; i++)
	{
		ShaderHandle handle = ah_register_shader(_ShaderAssets[i].AssetData, AssetLoadType::FORCE_LOAD);
		_ShaderAssets[i].ShadHandle= handle;
	}
}

// ------------------------------------
// Models 
// ------------------------------------

enum class ModelName
{
	wooden_sphere,
	COUNT
};

static AssetState _ModelAssets[] =
{
	AssetState { Asset{ "res/models/wooden_sphere.fbx"} }
};

inline ModelHandle al_get_model_handle(ModelName model)
{
	return _ModelAssets[(unsigned int)model].Handle.ModHandle;
}

void al_lazy_load_models()
{
	for (unsigned int i = 0; i < (unsigned int)ModelName::COUNT; i++)
	{
		ModelHandle handle = ah_register_model(_ModelAssets[i].AssetData, AssetLoadType::LOAD_WHEN_POSSIBLE);
		_ModelAssets[i].Handle.ModHandle = handle;
	}
}

void al_load_models()
{
	for (unsigned int i = 0; i < (unsigned int)ModelName::COUNT; i++)
	{
		ModelHandle handle = ah_register_model(_ModelAssets[i].AssetData, AssetLoadType::FORCE_LOAD);
		_ModelAssets[i].Handle.ModHandle = handle;
	}
}

void al_load_all_assets()
{
	al_load_models();
	al_load_textures();
	al_load_shaders();
}

void al_lazy_load_assets()
{
	al_lazy_load_models();
	al_lazy_load_textures();
	al_lazy_load_shaders();
}


// ------------------------------------
// --- Helper functions
// ------------------------------------
inline Model* al_get_model_ptr(ModelName name)
{
	Model* model = ah_get_model(al_get_model_handle(name));
	if (model == nullptr)
	{
		// Handle is invalid. Throw error instead of loading because in this case, this file is being implemented having in mind that
		// assets are permanently loaded through the lifetime of the game. If that wasn't the case, another system would be implemented
		// to properly handle it
		THROW_ERROR("ERROR::al_get_model_ptr - Handle is invalid!");
		return nullptr;
	}
	return model;
}

inline Texture* al_get_texture_ptr(TextureName name)
{
	Texture* texture = ah_get_texture(al_get_texture_handle(name));
	if (texture == nullptr)
	{
		// Handle is invalid. Throw error instead of loading because in this case, this file is being implemented having in mind that
		// assets are permanently loaded through the lifetime of the game. If that wasn't the case, another system would be implemented
		// to properly handle it
		THROW_ERROR("ERROR::al_get_texture_ptr - Handle is invalid!");
		return nullptr;
	}
	return texture;
}

inline Shader* al_get_shader_ptr(ShaderName name)
{
	Shader* shader = ah_get_shader(al_get_shader_handle(name));
	if (shader == nullptr)
	{
		// Handle is invalid. Throw error instead of loading because in this case, this file is being implemented having in mind that
		// assets are permanently loaded through the lifetime of the game. If that wasn't the case, another system would be implemented
		// to properly handle it
		THROW_ERROR("ERROR::al_get_shader_ptr - Handle is invalid!");
		return nullptr;
	}
	return shader;
}



#endif // ASSET_LIST_H
