#include "assets_handler.h"

#include "../asset_handler_types.h"
#include "../private/_assets_handler.h"

#include "../../core/ErrorHandler.h"

void ah_lazy_load_assets()
{
    _load_assets();
}

ModelHandle ah_register_model(const Asset& asset, const AssetLoadType loadingType)
{
    ModelHandle handle = _register_new_model();

    // --- Handle loading -------------

    AssetToLoadData loadData;
    loadData.AssetData.ModelData = asset;
    loadData.Handle.ModHandle = handle;
    loadData.Type = AssetType::MODEL;

    if (loadingType == AssetLoadType::FORCE_LOAD)
    {
        _load_model(loadData);
    }
    else
    {
        _add_asset_to_lazy_load(loadData);
    }

    return handle;
}

ModelHandle ah_register_model(Model* pModel)
{
    ModelHandle handle = _register_new_model();
    ModelData& data = _get_model_data(handle);
    data.pModel = pModel;
    return handle;
}

Model* ah_get_model(ModelHandle handle)
{
    return _get_model(handle);
}

void ah_unregister_model(ModelHandle handle)
{
    ModelData& data = _get_model_data(handle);
    // This should be true here as error is handled inside the prev function
    if (data.GenerationId == handle.GenerationId)
    {
        data.GenerationId |= INVALID_GENERATION_MASK;
    }
}

TextureHandle ah_register_texture(const Asset& asset, const AssetLoadType loadingType)
{
    TextureHandle handle = _register_new_texture();

    // --- Handle loading -------------

    AssetToLoadData loadData;
    loadData.AssetData.TextureData = asset;
    loadData.Handle.TexHandle = handle;
    loadData.Type = AssetType::TEXTURE;

    if (loadingType == AssetLoadType::FORCE_LOAD)
    {
        _load_texture(loadData);
    }
    else
    {
        _add_asset_to_lazy_load(loadData);
    }

    return handle;
}

TextureHandle ah_register_texture(Texture* pTexture)
{
    TextureHandle handle = _register_new_texture();
    TextureData& data = _get_texture_data(handle);
    data.pTexture = pTexture;
    return handle;
}

Texture* ah_get_texture(TextureHandle handle)
{
    return _get_texture(handle);
}

void ah_unregister_texture(TextureHandle handle)
{
    TextureData& data = _get_texture_data(handle);
    // This should be true here as error is handled inside the prev function
    if (data.GenerationId == handle.GenerationId)
    {
        data.GenerationId |= INVALID_GENERATION_MASK;
    }
}

ShaderHandle ah_register_shader(const ShaderAsset& asset, const AssetLoadType loadingType)
{
    ShaderHandle handle = _register_new_shader();

    // --- Handle loading -------------

    AssetToLoadData loadData;
    loadData.AssetData.ShaderData = asset;
    loadData.Handle.ShadHandle = handle;
    loadData.Type = AssetType::SHADER;

    if (loadingType == AssetLoadType::FORCE_LOAD)
    {
        _load_shader(loadData);
    }
    else
    {
        _add_asset_to_lazy_load(loadData);
    }

    return handle;
}

ShaderHandle ah_register_shader(Shader* pShader)
{
    ShaderHandle handle = _register_new_shader();
    ShaderData& data = _get_shader_data(handle);
    data.pShader = pShader;
    return handle;
}

Shader* ah_get_shader(ShaderHandle handle)
{
    return _get_shader(handle);
}

void ah_unregister_shader(ShaderHandle handle)
{
    ShaderData& data = _get_shader_data(handle);
    // This should be true here as error is handled inside the prev function
    if (data.GenerationId == handle.GenerationId)
    {
        data.GenerationId |= INVALID_GENERATION_MASK;
    }
}

MaterialHandle ah_register_material(Material* pMaterial)
{
    MaterialHandle handle = _register_new_material();
    MaterialData& data = _get_material_data(handle);
    data.pMaterial = pMaterial;
    return handle;
}

Material* ah_get_material(MaterialHandle handle)
{
    return _get_material(handle);
}

void ah_unregister_material(MaterialHandle handle)
{
    MaterialData& data = _get_material_data(handle);
    // This should be true here as error is handled inside the prev function
    if (data.GenerationId == handle.GenerationId)
    {
        data.GenerationId |= INVALID_GENERATION_MASK;
    }
}