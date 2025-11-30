#include "_assets_handler.h"
#include "_assimp_mesh_importer.h"
#include <arena_allocator.h>
#include <iostream>
#include <queue>
#include "../../core/ErrorHandler.h"
#include "../../core/memory_utils.h"

static ShaderData NULL_SHADER_DATA = { nullptr, INVALID_GENERATION_MASK };
static std::vector<ShaderData> _Shaders;

static TextureData NULL_TEXTURE_DATA = { nullptr, INVALID_GENERATION_MASK };
static std::vector<TextureData> _Textures;

static ModelData NULL_MODEL_DATA = { nullptr, INVALID_GENERATION_MASK };
static std::vector<ModelData> _Models;

static MaterialData NULL_MATERIAL_DATA = { nullptr, INVALID_GENERATION_MASK };
static std::vector<MaterialData> _Materials;

static std::queue<AssetToLoadData> _AssetsToLazyLoad;

bool _is_generation_valid(unsigned int generationId)
{
    return (generationId & INVALID_GENERATION_MASK) == 0;
}

void _add_asset_to_lazy_load(AssetToLoadData& data)
{
    _AssetsToLazyLoad.push(data);
}

void _load_assets()
{
    // TODO: Do this in different jobs/ different thread than main
    while (_AssetsToLazyLoad.size() > 0)
    {
        AssetToLoadData& asset = _AssetsToLazyLoad.front();

        switch (asset.Type)
        {
        case AssetType::MODEL:
        {
            _load_model(asset);
        } break;
        case AssetType::SHADER:
        {
            _load_shader(asset);
        } break;
        case AssetType::TEXTURE:
        {
            _load_texture(asset);
        } break;
        default:
            break;
        }

        _AssetsToLazyLoad.pop();
    }
}

ModelHandle _register_new_model()
{
    ModelData data = { 0 };
    data.pModel = nullptr;
    data.GenerationId = 0;

    // --- Create handle --------------

    ModelHandle handle = { 0 };

    // Get first free slot of vector
    size_t foundIndex = UINT32_MAX;
    for (unsigned int i = 0; i < _Models.size(); i++)
    {
        if (!_is_generation_valid(_Models[i].GenerationId))
        {
            foundIndex = i;
        }
    }
    if (foundIndex >= _Models.size())
    {
        // No free slot found, create a new one and start the generation
        handle.GenerationId = 0;
        handle.Id = _Models.size();
        _Models.push_back(data);
    }
    else
    {
        // Free slot found, create new valid generation 
        // TODO: should generations be reused when reaching the maximum id? Or just make it invalid forever and try to find another slot?
        data.GenerationId = ((_Models[foundIndex].GenerationId & GENERATION_ID_MASK) + 1) % UINT32_MAX;
        handle.GenerationId = data.GenerationId;
        handle.Id = foundIndex;
        _Models[foundIndex] = data;
    }

    return handle;
}

// TODO: should a boolean be returned to handle the error?
void _load_model(const AssetToLoadData& asset)
{
    const ModelHandle& handle = asset.Handle.ModHandle;
    ModelData& data = _Models[handle.Id];

    // Check if handle is valid
    if (data.GenerationId != handle.GenerationId)
    {
        // Handle not valid!!
        THROW_ERROR("ERROR::_load_model - Handle is not valid!");
        return;
    }
  
    Model* pModel = (Model*)CE_MALLOC(sizeof(Model));
    import_model(pModel, asset.AssetData.ModelData.Path);
    data.pModel = pModel;
}

ModelHandle _get_model_handle(unsigned int id)
{
    return ModelHandle{ id, _Models[id].GenerationId };
}

Model* _get_model(ModelHandle handle)
{
    ModelData& data = _get_model_data(handle);

    return data.pModel;
}

ModelData& _get_model_data(ModelHandle handle)
{
    ModelData& data = _Models[handle.Id];

    // Check if handle is valid
    if (data.GenerationId != handle.GenerationId)
    {
        // Handle not valid!!
        THROW_ERROR("ERROR::_get_model_data_ptr - Handle is not valid!");
        return NULL_MODEL_DATA;
    }

    return data;
}


void _load_texture(const AssetToLoadData& asset)
{
    const TextureHandle& handle = asset.Handle.TexHandle;
    TextureData& data = _Textures[handle.Id];

    // Check if handle is valid
    if (data.GenerationId != handle.GenerationId)
    {
        // Handle not valid!!
        THROW_ERROR("ERROR::_load_texture - Handle is not valid!");
    }

    Texture* texture = (Texture*)CE_MALLOC(sizeof(Texture));
    create_texture(texture, asset.AssetData.TextureData.Path);
    data.pTexture = texture;
}

TextureHandle _register_new_texture()
{
    TextureData data = { 0 };
    data.pTexture = nullptr;
    data.GenerationId = 0;

    // --- Create handle --------------

    TextureHandle handle = { 0 };

    // Get first free slot of vector
    size_t foundIndex = UINT32_MAX;
    for (unsigned int i = 0; i < _Textures.size(); i++)
    {
        if (!_is_generation_valid(_Textures[i].GenerationId))
        {
            foundIndex = i;
        }
    }
    if (foundIndex >= _Textures.size())
    {
        // No free slot found, create a new one and start the generation
        handle.GenerationId = 0;
        handle.Id = _Textures.size();
        _Textures.push_back(data);
    }
    else
    {
        // Free slot found, create new valid generation 
        // TODO: should generations be reused when reaching the maximum id? Or just make it invalid forever and try to find another slot?
        data.GenerationId = ((_Textures[foundIndex].GenerationId & GENERATION_ID_MASK) + 1) % UINT32_MAX;
        handle.GenerationId = data.GenerationId;
        handle.Id = foundIndex;
        _Textures[foundIndex] = data;
    }

    return handle;
}

TextureData& _get_texture_data(TextureHandle handle)
{
    TextureData& data = _Textures[handle.Id];

    // Check if handle is valid
    if (data.GenerationId != handle.GenerationId)
    {
        // Handle not valid!!
        THROW_ERROR("ERROR::_get_texture_data_ptr - Handle is not valid!");
        return NULL_TEXTURE_DATA;
    }

    return data;
}

TextureHandle _get_texture_handle(unsigned int id)
{
    return TextureHandle{ id, _Textures[id].GenerationId };
}

Texture* _get_texture(TextureHandle handle)
{
    TextureData& data = _get_texture_data(handle);
    return data.pTexture;
}

void _load_shader(const AssetToLoadData& asset)
{
    const ShaderHandle& handle = asset.Handle.ShadHandle;
    ShaderData& data = _Shaders[handle.Id];

    // Check if handle is valid
    if (data.GenerationId != handle.GenerationId)
    {
        // Handle not valid!!
        THROW_ERROR("ERROR::_load_texture - Handle is not valid!");
    }

    Shader* shader = (Shader*)CE_MALLOC(sizeof(Shader));
    create_shader(shader, asset.AssetData.ShaderData.Vertex.Path, asset.AssetData.ShaderData.Fragment.Path);
    use_shader(shader);
    data.pShader = shader;
}

ShaderHandle _register_new_shader()
{
    ShaderData data = { 0 };
    data.pShader = nullptr;
    data.GenerationId = 0;

    // --- Create handle --------------

    ShaderHandle handle = { 0 };

    // Get first free slot of vector
    size_t foundIndex = UINT32_MAX;
    for (unsigned int i = 0; i < _Shaders.size(); i++)
    {
        if (!_is_generation_valid(_Shaders[i].GenerationId))
        {
            foundIndex = i;
        }
    }
    if (foundIndex >= _Shaders.size())
    {
        // No free slot found, create a new one and start the generation
        handle.GenerationId = 0;
        handle.Id = _Shaders.size();
        _Shaders.push_back(data);
    }
    else
    {
        // Free slot found, create new valid generation 
        // TODO: should generations be reused when reaching the maximum id? Or just make it invalid forever and try to find another slot?
        data.GenerationId = ((_Shaders[foundIndex].GenerationId & GENERATION_ID_MASK) + 1) % UINT32_MAX;
        handle.GenerationId = data.GenerationId;
        handle.Id = foundIndex;
        _Shaders[foundIndex] = data;
    }

    return handle;
}

ShaderData& _get_shader_data(ShaderHandle handle)
{
    ShaderData& data = _Shaders[handle.Id];

    // Check if handle is valid
    if (data.GenerationId != handle.GenerationId)
    {
        // Handle not valid!!
        THROW_ERROR("ERROR::_get_shader_data - Handle is not valid!");
        return NULL_SHADER_DATA;
    }

    return data;
}

ShaderHandle _get_shader_handle(unsigned int id)
{
    return ShaderHandle{ id, _Shaders[id].GenerationId };
}

Shader* _get_shader(ShaderHandle handle)
{
    ShaderData& data = _get_shader_data(handle);
    return data.pShader;
}

MaterialHandle _register_new_material()
{
    MaterialData data = { 0 };
    data.pMaterial = nullptr;
    data.GenerationId = 0;

    // --- Create handle --------------

    MaterialHandle handle = { 0 };

    // Get first free slot of vector
    size_t foundIndex = UINT32_MAX;
    for (unsigned int i = 0; i < _Materials.size(); i++)
    {
        if (!_is_generation_valid(_Materials[i].GenerationId))
        {
            foundIndex = i;
        }
    }
    if (foundIndex >= _Materials.size())
    {
        // No free slot found, create a new one and start the generation
        handle.GenerationId = 0;
        handle.Id = _Materials.size();
        _Materials.push_back(data);
    }
    else
    {
        // Free slot found, create new valid generation 
        // TODO: should generations be reused when reaching the maximum id? Or just make it invalid forever and try to find another slot?
        data.GenerationId = ((_Materials[foundIndex].GenerationId & GENERATION_ID_MASK) + 1) % UINT32_MAX;
        handle.GenerationId = data.GenerationId;
        handle.Id = foundIndex;
        _Materials[foundIndex] = data;
    }

    return handle;
}

MaterialData& _get_material_data(MaterialHandle handle)
{
    MaterialData& data = _Materials[handle.Id];

    // Check if handle is valid
    if (data.GenerationId != handle.GenerationId)
    {
        // Handle not valid!!
        THROW_ERROR("ERROR::_get_material_data - Handle is not valid!");
        return NULL_MATERIAL_DATA;
    }

    return data;
}

Material* _get_material(MaterialHandle handle)
{
    return _get_material_data(handle).pMaterial;
}

//
//void load_shaders()
//{
//    _Shaders.resize((int)ShaderName::COUNT);
//    for (int i = 0; i < (int)ShaderName::COUNT; i++)
//    {
//        load_shader(&_Shaders[i], _ShaderAssets[i]);
//    }
//}
//
//void load_shader(Shader* rp_shader, ShaderAsset& r_shader)
//{
//    create_shader(rp_shader, r_shader.Vertex.Path, r_shader.Fragment.Path);
//    use_shader(rp_shader);
//}
//
//ShaderHandle ah_get_shader_handle(ShaderName r_shaderName)
//{
//    return ShaderHandle{ (unsigned int)r_shaderName };
//}
//
//Shader* ah_get_shader(ShaderHandle r_shaderHandle)
//{
//    return &_Shaders[r_shaderHandle.Id];
//}
//
//Material* ah_create_material()
//{
//    return (Material*) arena_alloc(&_MaterialArena, sizeof(Material));
//}
