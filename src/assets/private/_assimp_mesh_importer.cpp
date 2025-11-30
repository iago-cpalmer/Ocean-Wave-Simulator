#include "_assimp_mesh_importer.h"

#include <iostream>
#include "../public/assets_handler.h"
#include "../../core/memory_utils.h"

static std::vector<Vertex> s_Vertices;
static std::vector<unsigned int> s_Indices;

// TODO: this should maybe done in a better way?
static unsigned int s_SubmeshCount = 0;
static unsigned int s_TotalIndexCount = 0;
static unsigned int s_TotalVertexCount = 0;

void import_model(Model* rp_model, std::string r_path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(r_path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    VertexAttribute vertexAttributes[] =
    {
        VertexAttribute{VertexAttributeType::FLOAT, 3},	    // Position
        VertexAttribute{VertexAttributeType::FLOAT, 3},	    // Normal
        VertexAttribute{VertexAttributeType::FLOAT, 2}		// UV
    };

    /// vvv ---------------------------
    // TODO: should this be done here so it can allocate buffers for the mesh being created now
    // or it should be done after processing the whole mesh and then passing all data when
    // creating the mesh?
    unsigned int vertexCount = 0;
    unsigned int indexCount = 0;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        vertexCount += scene->mMeshes[i]->mNumVertices;
        indexCount += scene->mMeshes[i]->mNumFaces*3;
    }
    rp_model->p_Mesh = (Mesh*)CE_MALLOC(sizeof(Mesh));
    create_mesh(rp_model->p_Mesh, vertexAttributes, 3, nullptr, vertexCount, nullptr, indexCount, scene->mNumMeshes, GL_STATIC_DRAW);
    // ^^^ ---------------------------

    s_SubmeshCount = 0;
    s_TotalIndexCount = 0;
    s_TotalVertexCount = 0;
    
    process_node(scene->mRootNode, scene, rp_model);
}

void process_node(aiNode* rp_node, const aiScene* rp_scene, Model* rp_model)
{
   
    for (int i = 0; i < rp_node->mNumMeshes; i++)
    {
        process_mesh(rp_scene->mMeshes[rp_node->mMeshes[i]], rp_scene, rp_model);
    }

    if (rp_node->mChildren == nullptr)
    {
        return;
    }

    for (int i = 0; i < rp_node->mNumChildren; i++)
    {
        process_node(rp_node->mChildren[i], rp_scene, rp_model);
    }

}

void process_mesh(aiMesh* rp_aiMesh, const aiScene* rp_scene, Model* rp_model)
{
    Mesh mesh = { 0 };
    
    int vertexCount = rp_aiMesh->mNumVertices;
    s_Vertices.clear();
    if (s_Vertices.capacity() < rp_aiMesh->mNumVertices)
    {
        s_Vertices.reserve(rp_aiMesh->mNumVertices);
    }

    for (unsigned int i = 0; i < rp_aiMesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.Position.x = rp_aiMesh->mVertices[i].x;
        vertex.Position.y = rp_aiMesh->mVertices[i].y;
        vertex.Position.z = rp_aiMesh->mVertices[i].z;

        // TODO: If no normals, generate them from vertices
        // Now it is done like this to avoid a crash
        if (rp_aiMesh->mNormals != NULL)
        {
            vertex.Normal.x = rp_aiMesh->mNormals[i].x;
            vertex.Normal.y = rp_aiMesh->mNormals[i].y;
            vertex.Normal.z = rp_aiMesh->mNormals[i].z;
        } else 
        {
            vertex.Normal = glm::vec3(0.0f);
        } 

        if (rp_aiMesh->mTextureCoords[0])
        {
            vertex.UV.x = rp_aiMesh->mTextureCoords[0][i].x;
            vertex.UV.y = rp_aiMesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.UV = glm::vec2(0.0f, 0.0f);
        }

        s_Vertices.push_back(vertex);
    }

    s_Indices.clear();
    
    unsigned int indexCount = 0;
    for (unsigned int i = 0; i < rp_aiMesh->mNumFaces; i++)
    {
        indexCount += rp_aiMesh->mFaces[i].mNumIndices;
    }
    if (s_Indices.capacity() < indexCount)
    {
        s_Indices.reserve(indexCount);
    }

    for (unsigned int i = 0; i < rp_aiMesh->mNumFaces; i++)
    {
        aiFace face = rp_aiMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            s_Indices.push_back(face.mIndices[j] + s_TotalVertexCount);
        }
    }


    // TODO: Add option to not load any materials nor textures so the game can handle it on its own
    // Otherwise: Texture should be registered inside the assets handler and material
    // When unloading the material, make sure the texture is also unloaded (and unregistered)
    Material meshMaterial;
    if (rp_aiMesh->mMaterialIndex >= 0)
    {
        //meshMaterial = ah_create_material();
        // Always use default shader by default
        //meshMaterial.Shader = ah_get_shader_handle(ShaderName::basic_shader);

        // TODO: use default texture if no material/ texture
        //meshMaterial.AlbedoMap = ah_get_texture_handle(TextureName::container);

        aiMaterial* material = rp_scene->mMaterials[rp_aiMesh->mMaterialIndex];
        aiColor3D color(0.0f, 0.0f, 0.0f);
        float shininess;
        //material->Get(AI_MATKEY_SHININESS, shininess);
        //meshMaterial.Shininess = shininess;
    }
    else
    {
        // TODO: add default material
        //meshMaterial = DEFAULT_MAT;
    }
    Mesh* p_Mesh = rp_model->p_Mesh;
    vbo_set_data(p_Mesh->Vbo, s_Vertices.data(), sizeof(Vertex) * vertexCount, s_TotalVertexCount);
    ibo_set_data(p_Mesh->Ibo, s_Indices.data(), sizeof(unsigned int) * indexCount, s_TotalIndexCount);

    p_Mesh->Submeshes[s_SubmeshCount] = Submesh{ s_TotalIndexCount, indexCount };
    //TODO: rp_mesh->Materials[s_SubmeshCount] = meshMaterial;
    
    s_TotalIndexCount += indexCount;
    s_TotalVertexCount += vertexCount;
    ++s_SubmeshCount;
}