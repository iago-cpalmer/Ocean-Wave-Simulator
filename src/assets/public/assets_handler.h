#ifndef ASSETS_HANDLER_H
#define ASSETS_HANDLER_H

#include "../../renderer/data/texture.h"
#include "../../renderer/data/shader.h"
#include "../../renderer/data/mesh.h"
#include "../../renderer/data/model.h"
#include "../asset_handler_types.h"

// ------------------------------------
// GENERALS
// ------------------------------------
void ah_lazy_load_assets();

// ------------------------------------
// MODELS
// ------------------------------------ 

/// <summary>
/// Registers a model from a path and returns its handle. 
/// </summary>
/// <param name="path">Path of model to register and load</param>
/// <param name="loadingType">Type of loading</param>
/// <returns>ModelHandle</returns>
ModelHandle ah_register_model(const Asset& asset, const AssetLoadType loadingType);

/// <summary>
/// Registers a model from memory (probably created procedurally)
/// </summary>
/// <param name="pModel">Pointer of Model to register</param>
/// <returns>ModelHandle</returns>
ModelHandle ah_register_model(Model* pModel);

/// <summary>
/// Returns a model pointer from it's handle if valid. This pointer should be stored for a short period of time!
/// </summary>
/// <param name="handle">Handle of model</param>
/// <returns>Model pointer. Returns nullptr if handle not valid</returns>
Model* ah_get_model(ModelHandle handle);

/// <summary>
/// Unregisters a model
/// </summary>
/// <param name="handle">Handle to unregister</param>
void ah_unregister_model(ModelHandle handle);

// ------------------------------------
// TEXTURES
// ------------------------------------ 

/// <summary>
/// Registers a texture from a path and returns its handle. 
/// </summary>
/// <param name="path">Path of texture to register and load</param>
/// <param name="loadingType">Type of loading</param>
/// <returns>TextureHandle</returns>
TextureHandle ah_register_texture(const Asset& asset, const AssetLoadType loadingType);

/// <summary>
/// Registers a texture from memory (probably created procedurally)
/// </summary>
/// <param name="pTexture">Pointer of texture to register</param>
/// <returns>TextureHandle</returns>
TextureHandle ah_register_texture(Texture* pTexture);

/// <summary>
/// Returns a texture pointer from it's handle if valid. This pointer should be stored for a short period of time!
/// </summary>
/// <param name="handle">Handle of texture</param>
/// <returns>texture pointer. Returns nullptr if handle not valid</returns>
Texture* ah_get_texture(TextureHandle handle);

/// <summary>
/// Unregisters a texture
/// </summary>
/// <param name="handle">Handle to unregister</param>
void ah_unregister_texture(TextureHandle handle);

// ------------------------------------
// SHADERS
// ------------------------------------ 

/// <summary>
/// Registers a texture from a path and returns its handle. 
/// </summary>
/// <param name="path">Path of texture to register and load</param>
/// <param name="loadingType">Type of loading</param>
/// <returns>TextureHandle</returns>
ShaderHandle ah_register_shader(const ShaderAsset& asset, const AssetLoadType loadingType);

/// <summary>
/// Registers a texture from memory (probably created procedurally)
/// </summary>
/// <param name="pShader">Pointer of texture to register</param>
/// <returns>TextureHandle</returns>
ShaderHandle ah_register_shader(Shader* pShader);

/// <summary>
/// Returns a texture pointer from it's handle if valid. This pointer should be stored for a short period of time!
/// </summary>
/// <param name="handle">Handle of texture</param>
/// <returns>texture pointer. Returns nullptr if handle not valid</returns>
Shader* ah_get_shader(ShaderHandle handle);

/// <summary>
/// Unregisters a shader
/// </summary>
/// <param name="handle">Handle to unregister</param>
void ah_unregister_shader(ShaderHandle handle);

// ------------------------------------
// MATERIALS
// ------------------------------------ 

/// <summary>
/// Registers a material from memory
/// </summary>
/// <param name="pModel">Pointer of material to register</param>
/// <returns>MaterialHandle</returns>
MaterialHandle ah_register_material(Material* pMaterial);

/// <summary>
/// Returns a material pointer from it's handle if valid. This pointer should be stored for a short period of time!
/// </summary>
/// <param name="handle">Handle of material</param>
/// <returns>material pointer. Returns nullptr if handle not valid</returns>
Material* ah_get_material(MaterialHandle handle);

/// <summary>
/// Unregisters a material
/// </summary>
/// <param name="handle">Handle to unregister</param>
void ah_unregister_material(MaterialHandle handle);

#endif // ! ASSETS_HANDLER_H


