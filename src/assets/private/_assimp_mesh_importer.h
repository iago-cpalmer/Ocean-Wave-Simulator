#ifndef ASSIMP_MESH_IMPORTER_H
#define ASSIMP_MESH_IMPORTER_H

#include "../../renderer/data/mesh.h"
#include "../../renderer/data/model.h"
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void import_model(Model* rp_model, std::string r_path);

void process_node(aiNode* rp_node, const aiScene* rp_scene, Model* rp_model);
void process_mesh(aiMesh* rp_aiMesh, const aiScene* rp_scene, Model* rp_model);

#endif // !ASSIMP_MESH_IMPORTER_H
