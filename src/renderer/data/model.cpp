#include "model.h"
#include "../../core/memory_utils.h"

void model_init(Model* pModel, Mesh* pMesh)
{
	pModel->p_Mesh = pMesh;
	pModel->MaterialCount = pMesh->SubmeshCount;
	pModel->p_MaterialHandles = (MaterialHandle*)CE_MALLOC(sizeof(MaterialHandle)*pMesh->SubmeshCount);
}
