#include <iostream>

#include "core/window.h"

#include "renderer/data/shader.h"
#include "renderer/data/texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "renderer/data/mesh.h"

#include "core/input_handler.h"
#include "renderer/renderer.h"
#include "renderer/camera.h"
#include "core/time_manager.h"
#include "meshes.h"
#include "renderer/light/directional_light.h"
#include "renderer/light/point_light.h"
#include "core/utils.h"

#include "scene/entity.h"
#include "scene/scene.h"

#include "assets/public/assets_handler.h"
#include "assets/asset_list.h"
#include "core/memory_utils.h"
#include "renderer/data/buffers/frame_buffer.h"
#include "renderer/data/cubemap.h"
#include "renderer/scene_renderer.h"

const float DESIRED_FPS = 120;

int main()
{
	if (create_window(800, 600, "Ocean Waves Simulator") == -1)
	{
		return -1;
	}
	glfwSwapInterval(0);
	renderer_init_opengl();
	renderer_set_clear_color_normalized(0.6f* 1.5, 0.8f* 1.5, 1.0f* 1.5);
	init_input();
	cursor_set_state(CursorLockType::CenterLock, false);

	Cubemap skybox;
	/*
	std::vector<std::string> skyFacesPath =
	{
		"res/images/skybox_right.jpg",
		"res/images/skybox_left.jpg",
		"res/images/skybox_top.jpg",
		"res/images/skybox_bottom.jpg",
		"res/images/skybox_front.jpg",
		"res/images/skybox_back.jpg",
	};*/
	std::vector<std::string> skyFacesPath =
	{
		"res/images/skybox_storm_left.png",
		"res/images/skybox_storm_right.png",
		"res/images/skybox_storm_top.png",
		"res/images/skybox_storm_bottom.png",
		"res/images/skybox_storm_front.png",
		"res/images/skybox_storm_back.png",
	};
	
	load_cubemap(&skybox, skyFacesPath);
	
	CameraInfo camera{};
	camera.RenderTarget = RenderTargetType::Screen;
	camera.Projection = ProjectionType::Perspective;
	camera.Position = glm::vec3(0.0f, 0.0f, 6.0f);
	camera.Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	camera.Fov = 45.0f;
	camera.SensitivityX = 10.0f;
	camera.SensitivityY = 15.0f;
	camera.MovementSpeed = 10.0f;
	camera.NearPlane = 0.1f;
	camera.FarPlane = 2000.0f;
	camera.BackgroundType = SkyType::Skybox;
	camera.Background.Skybox = skybox;

	bool locked = true;

	Mesh skyboxMesh;
	VertexAttribute vAttr[] =
	{
		VertexAttribute{VertexAttributeType::FLOAT, 3}
	};
	create_mesh(&skyboxMesh, vAttr, 1, VERTICES_SKYBOX, 24, INDICES_SKYBOX, 36, 1, GL_STATIC_DRAW);

	init_scene_renderer(&skyboxMesh);
	
	Scene scene;
	init_scene(&scene);
	scene_add_camera(&scene, &camera);

	al_load_all_assets();


#pragma region OCEAN_DEFINITION
	// vvv ----------------------------
	// Ocean
	Model oceanModel;
	Mesh oceanMesh;
	create_mesh_grid(&oceanMesh, 1024, 1024);
	oceanModel.p_Mesh = &oceanMesh;
	Entity sphereEntity;
	sphereEntity.meshRendererData.ModelHandle = ah_register_model(&oceanModel);
	sphereEntity.Position = glm::vec3(0);
	sphereEntity.Rotation = glm::vec3(0, 0, 0);
	sphereEntity.Scale = glm::vec3(1.0);

	// --- Definition of material --------
	Material sphereMat;
	sphereMat.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	sphereMat.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	sphereMat.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	sphereMat.Shininess = 128.0f;
	sphereMat.Shader = al_get_shader_handle(ShaderName::basic_shader);
	sphereMat.AlbedoMap = al_get_texture_handle(TextureName::point_light);
	// -----------------------------------
	
   	oceanModel.p_MaterialHandles = (MaterialHandle*)CE_MALLOC(sizeof(MaterialHandle));
   	oceanModel.p_MaterialHandles[0] = ah_register_material(&sphereMat);
   	oceanModel.MaterialCount = 1;

	Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
	set_uniform_vec2(pShader, "uWaveDirection", glm::vec2(1.0f, 1.0f));
	set_uniform_float(pShader, "uSpeed", 1.0f);
	set_uniform_float(pShader, "uSteepness", 0.9);
	set_uniform_int(pShader, "uWaveCount", 300);
	set_uniform_float(pShader, "uTime", glfwGetTime());	
	set_uniform_float(pShader, "uFrequency", 0.125f);	
	set_uniform_float(pShader, "uAmplitude", 1.0f);	
	set_uniform_float(pShader, "uPersistance", 0.83f);	
	set_uniform_float(pShader, "uLacunarity", 1.17f);

	set_uniform_float(pShader, "uInitialSeed", 2);	
	set_uniform_float(pShader, "uSeedIter", 4.1f);	

	set_uniform_float(pShader, "uSpeedRamp", 1.07f);	


	instantiate_entity(&scene, &sphereEntity);
	// ^^^ ----------------------------
#pragma endregion


	PointLight pointLight;
	pointLight.Position = glm::vec3(2.0f, 0.0f, 0.0f);
	pointLight.AmbientColor = glm::vec3(0.1f, 0.1f, 0.1f);
	pointLight.DiffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	pointLight.SpecularColor = glm::vec3(0.75f, 0.75f, 0.75f);
	pointLight.Constant = 1.0f;
	pointLight.Linear = 0.09f;
	pointLight.Quadratic = 0.032f;

	DirectionalLight directionalLight;
	directionalLight.Direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	directionalLight.AmbientColor = glm::vec3(0.25f, 0.25f, 0.25f);
	directionalLight.DiffuseColor = glm::vec3(0.75f, 0.75f, 0.75f);
	directionalLight.SpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);

	add_directional_light(&scene, &directionalLight);
	add_point_light(&scene, &pointLight);

	// ^^^ ----------------------------
	while (!window_should_close())
	{
#pragma region region_time_and_input
		if(!calculate_delta_time(window_get_time_since_start(), DESIRED_FPS)) continue;
		update_input();
#pragma endregion

#pragma region region_update_state
		if (is_input_pressed(Input::Escape))
		{
			cursor_set_state(CursorLockType::Free, true);
			locked = false;
		}
		if (is_mouse_button_pressed(MouseButton::LeftClick))
		{
			cursor_set_state(CursorLockType::CenterLock, false);
			locked = true;
		}

		// vvv Camera stuff -----------
		if (locked)
		{
			camera_update(&camera);
		}
		// ^^^ ------------------------

		scene_update(&scene);

		// vvv Rotating Light 
		float lightX = 5.0f * sin(glfwGetTime());
		float lightY = 0.0f;
		float lightZ = 5.0f * cos(glfwGetTime());
		pointLight.Position = glm::vec3(lightX, lightY, lightZ);
		// ^^^ ------------------------

#pragma endregion

		set_uniform_float(pShader, "uTime", glfwGetTime());		

		renderer_prepare_frame();

		scene_render(&scene);

		renderer_finish_render();
	}

	window_terminate();
	return 0;
}


