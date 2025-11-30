#include "mesh.h"
#ifndef MODEL_H
#define MODEL_H

// Model: is a "mesh" loaded as an asset with all its materials included

typedef struct
{
	Mesh* p_Mesh;

	MaterialHandle* p_MaterialHandles;
	unsigned int MaterialCount; // TODO: Usually, the submesh count and material count should be equal. But sometimes we could have all submeshes sharing material?
} Model;

typedef struct
{
	unsigned int Id;
	unsigned int GenerationId;
} ModelHandle;

void model_init(Model* pModel);


#endif // !MODEL_H
