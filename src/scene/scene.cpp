#include "scene.h"
#include "../renderer/scene_renderer.h"
#include "../assets/public/assets_handler.h"
#include "../core/ErrorHandler.h"
//#include "../../assets/assets_handler.h"

void init_scene(Scene* rpScene)
{
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		rpScene->Entities[i] = NULL;
	}

	for (int i = 0; i < MAX_CAMERAS; i++)
	{
		rpScene->Cameras[i] = nullptr;
	}

	rpScene->SceneRenderData.LightEnv.p_DirectionalLight = nullptr;
	rpScene->SceneRenderData.LightEnv.p_PointLight = nullptr;
}

void scene_add_camera(Scene* rpScene, CameraInfo* pCamera)
{
	
	for (size_t i = 0; i < MAX_CAMERAS; ++i)
	{
		if (rpScene->Cameras[i] == nullptr)
		{
			if (rpScene->NumberOfCameras == 0)
			{
				rpScene->MainCamera = i;
			}
			rpScene->NumberOfCameras++;
			rpScene->Cameras[i] = pCamera;
			pCamera->CameraIdInScene = i;
			return;
		}
	}

	THROW_ERROR("ERROR::scene_add_camera - Trying to add more cameras to scene than the maximum (set to 10)");
}

void scene_set_main_camera(Scene* rpScene, unsigned int id)
{
	if (rpScene->Cameras[id]->CameraIdInScene == - 1)
	{
		THROW_ERROR("ERROR::scene_set_main_camera - Trying to set a non existent camera as main");
		return;
	}
	rpScene->MainCamera = id;
}

void scene_remove_camera(Scene* rpScene, unsigned int id)
{
	if (rpScene->Cameras[id]->CameraIdInScene != -1)
	{
		rpScene->NumberOfCameras--;
		rpScene->Cameras[id]->CameraIdInScene = -1;
		rpScene->Cameras[id] = nullptr;
	}
	else
	{
		THROW_ERROR("ERROR::scene_remove_cameras - Trying to remove a non existent camera");
	}
}

void scene_update(Scene* rpScene)
{
	// Update camera

	// Update entities
}

void scene_render(Scene* rpScene)
{
	for (size_t i = 0; i < MAX_CAMERAS; i++)
	{
		if (rpScene->Cameras[i] != nullptr)
		{
			begin_render(rpScene, rpScene->Cameras[i]);
			render_skybox(rpScene->Cameras[i]);
			for (int i = 0; i < MAX_ENTITIES; i++)
			{
				Entity* entity = rpScene->Entities[i];
				if (entity != NULL)
				{
					glm::mat4 transform = glm::mat4(1.0f);
					transform = glm::translate(transform, entity->Position);
					transform = glm::rotate(transform, glm::radians(entity->Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
					transform = glm::rotate(transform, glm::radians(entity->Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
					transform = glm::rotate(transform, glm::radians(entity->Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
					transform = glm::scale(transform, entity->Scale);


					Model* model = ah_get_model(entity->meshRendererData.ModelHandle);
					draw_mesh(model->p_Mesh, transform, model->p_MaterialHandles, model->MaterialCount);
				}
			}

			

			end_render();
		}
	}
}

void instantiate_entity(Scene* rp_scene, Entity* entity)
{
	// add entity to list of entities
	for (int i = 0; i < MAX_ENTITIES; i++)
	{
		if (rp_scene->Entities[i] == NULL)
		{
			rp_scene->Entities[i] = entity;
			break;
		}
	}
}


void add_directional_light(Scene* rp_scene, DirectionalLight* rp_light)
{
	rp_scene->SceneRenderData.LightEnv.p_DirectionalLight = rp_light;
}
void add_point_light(Scene* rp_scene, PointLight* rp_light)
{
	rp_scene->SceneRenderData.LightEnv.p_PointLight = rp_light;
}