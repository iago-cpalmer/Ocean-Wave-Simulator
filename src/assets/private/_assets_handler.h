#ifndef PRIVATE_ASSETS_HANDLER_H
#define PRIVATE_ASSETS_HANDLER_H

#include <vector>
#include "../../renderer/data/texture.h"
#include "../../renderer/data/shader.h"
#include "../../renderer/data/mesh.h"
#include "../../renderer/data/model.h"
#include "../asset_handler_types.h"

bool _is_generation_valid(unsigned int generationId);

typedef struct
{
	Model* pModel;
	unsigned int GenerationId;
} ModelData;

typedef struct
{
	Texture* pTexture;
	unsigned int GenerationId;
} TextureData;

typedef struct
{
	Shader* pShader;
	unsigned int GenerationId;
} ShaderData;


typedef struct
{
	Material* pMaterial;
	unsigned int GenerationId;
} MaterialData;

enum class AssetType
{
	TEXTURE,
	SHADER,
	MODEL
};

typedef struct
{
	AssetType Type;
	union
	{
		ModelHandle ModHandle;
		TextureHandle TexHandle;
		ShaderHandle ShadHandle;
	} Handle;

	union 
	{
		Asset ModelData;
		Asset TextureData;
		ShaderAsset ShaderData;
	} AssetData;
	
} AssetToLoadData;

void _load_assets();

void _add_asset_to_lazy_load(AssetToLoadData& data);

// ------------------------------------
// Textures
// ------------------------------------

void _load_texture(const AssetToLoadData& asset);
TextureHandle _register_new_texture();
TextureData& _get_texture_data(TextureHandle handle);
TextureHandle _get_texture_handle(unsigned int id);
Texture* _get_texture(TextureHandle handle);

// ------------------------------------
// Shaders
// ------------------------------------

void _load_shader(const AssetToLoadData& asset);
ShaderHandle _register_new_shader();
ShaderData& _get_shader_data(ShaderHandle handle);
ShaderHandle _get_shader_handle(unsigned int id);
Shader* _get_shader(ShaderHandle handle);

// ------------------------------------
// Models
// ------------------------------------

void _load_model(const AssetToLoadData& asset);
ModelHandle _register_new_model();
ModelData& _get_model_data(ModelHandle handle);
ModelHandle _get_model_handle(unsigned int id);
Model* _get_model(ModelHandle handle);

// ------------------------------------
// Materials
// ------------------------------------

MaterialHandle _register_new_material();
Material* _get_material(MaterialHandle handle);
MaterialData& _get_material_data(MaterialHandle handle);

#endif // ! PRIVATE_ASSETS_HANDLER_H


