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

#include "imgui/imgui_handler.h"


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

	imgui_init();

	Cubemap skybox;
	/*
	std::vector<std::string> skyFacesPath =
	{
		"res/images/skybox_sunset_right.png",
		"res/images/skybox_sunset_left.png",
		"res/images/skybox_sunset_top.png",
		"res/images/skybox_sunset_bottom.png",
		"res/images/skybox_sunset_front.png",
		"res/images/skybox_sunset_back.png"
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
	camera.Position = glm::vec3(300.0f, 10.0f, 300.0f);
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

	set_uniform_vec3(pShader, "uFoamColor", glm::vec3(1.0f));
	set_uniform_float(pShader, "uFoamThreshold", 0.9f);
	set_uniform_float(pShader, "uFoamHardness", 0.5f);
	set_uniform_float(pShader, "uFoamIntensity", 2.0f);
	set_uniform_float(pShader, "uFoamDistanceFade", 1000.0f);

	set_uniform_vec3(pShader, "uSeaColor", glm::vec3(0.01f, 0.05f, 0.05f));
	set_uniform_vec3(pShader, "uSeaColorSurface", glm::vec3(0.01f, 0.05f, 0.05f));

	set_uniform_float(pShader, "uFogDistance", 500.0f);

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

#pragma region INIT_IMGUI_COMPONENTS
	// Init ImGui components
	ImGuiComponent waveAmplitudeComp{};
	waveAmplitudeComp.Name = "Amplitude";
	waveAmplitudeComp.Type = ImGuiComponentType::Float;
	waveAmplitudeComp.Data = FloatComponent{
			0.0f,
			10.0f,
			1.0f,
			[](float val)
			{
				Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
				set_uniform_float(pShader, "uAmplitude", val);
			}
	};

	imgui_add_component(&waveAmplitudeComp);

	// Additional Wave parameters grouped under Wave Amplitude
	ImGuiComponent speedComp{};
	speedComp.Name = "Speed";
	speedComp.Type = ImGuiComponentType::Float;
	speedComp.Data = FloatComponent{
		0.0f,
		10.0f,
		1.0f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uSpeed", val);
		}
	};

	imgui_add_component(&speedComp);

	ImGuiComponent steepnessComp{};
	steepnessComp.Name = "Steepness";
	steepnessComp.Type = ImGuiComponentType::Float;
	steepnessComp.Data = FloatComponent{
		0.0f,
		1.0f,
		0.9f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uSteepness", val);
		}
	};
	imgui_add_component(&steepnessComp);

	ImGuiComponent waveCountComp{};
	waveCountComp.Name = "WaveCount";
	waveCountComp.Type = ImGuiComponentType::Int;
	waveCountComp.Data = IntComponent{
		1,
		2048,
		300,
		[](int val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_int(pShader, "uWaveCount", val);
		}
	};
	imgui_add_component(&waveCountComp);

	ImGuiComponent frequencyComp{};
	frequencyComp.Name = "Frequency";
	frequencyComp.Type = ImGuiComponentType::Float;
	frequencyComp.Data = FloatComponent{
		0.0f,
		10.0f,
		0.125f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uFrequency", val);
		}
	};
	imgui_add_component(&frequencyComp);

	ImGuiComponent persistanceComp{};
	persistanceComp.Name = "Persistance";
	persistanceComp.Type = ImGuiComponentType::Float;
	persistanceComp.Data = FloatComponent{
		0.0f,
		2.0f,
		0.83f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uPersistance", val);
		}
	};
	imgui_add_component(&persistanceComp);

	ImGuiComponent lacunarityComp{};
	lacunarityComp.Name = "Lacunarity";
	lacunarityComp.Type = ImGuiComponentType::Float;
	lacunarityComp.Data = FloatComponent{
		0.0f,
		4.0f,
		1.17f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uLacunarity", val);
		}
	};
	imgui_add_component(&lacunarityComp);

	ImGuiComponent initialSeedComp{};
	initialSeedComp.Name = "InitialSeed";
	initialSeedComp.Type = ImGuiComponentType::Float;
	initialSeedComp.Data = FloatComponent{
		0.0f,
		1000.0f,
		2.0f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uInitialSeed", val);
		}
	};
	imgui_add_component(&initialSeedComp);

	ImGuiComponent seedIterComp{};
	seedIterComp.Name = "SeedIter";
	seedIterComp.Type = ImGuiComponentType::Float;
	seedIterComp.Data = FloatComponent{
		0.0f,
		1000.0f,
		4.1f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uSeedIter", val);
		}
	};
	imgui_add_component(&seedIterComp);

	ImGuiComponent speedRampComp{};
	speedRampComp.Name = "SpeedRamp";
	speedRampComp.Type = ImGuiComponentType::Float;
	speedRampComp.Data = FloatComponent{
		0.0f,
		10.0f,
		1.07f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uSpeedRamp", val);
		}
	};
	imgui_add_component(&speedRampComp);
#pragma region ADD_VEC2_VEC3_COLOR

	ImGuiComponent dirLightDirComp{};
	dirLightDirComp.Name = "DirLightDir";
	dirLightDirComp.Type = ImGuiComponentType::Vec3;
	dirLightDirComp.Data = Vec3Component{
		glm::vec3(-1.0f),
		glm::vec3(1.0f),
		directionalLight.Direction,
		[&directionalLight](glm::vec3 val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			directionalLight.Direction = val;
			set_uniform_vec3(pShader, "uDirectionalLight.Direction", val);
		}
	};
	imgui_add_component(&dirLightDirComp);

	ImGuiComponent dirLightColorComp{};
	dirLightColorComp.Name = "DirLightColor";
	dirLightColorComp.Type = ImGuiComponentType::Color;
	dirLightColorComp.Data = ColorComponent{
		directionalLight.DiffuseColor,
		[&directionalLight](glm::vec3 val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			directionalLight.DiffuseColor = val;
			set_uniform_vec3(pShader, "uDirectionalLight.DiffuseColor", val);
		}
	};
	imgui_add_component(&dirLightColorComp);
#pragma endregion


	ImGuiComponent foamThresholdComp{};
	foamThresholdComp.Name = "FoamThreshold";
	foamThresholdComp.Type = ImGuiComponentType::Float;
	foamThresholdComp.Data = FloatComponent{
		0.0f,
		1.0f,
		0.9f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uFoamThreshold", val);
		}
	};
	imgui_add_component(&foamThresholdComp);

	ImGuiComponent foamHardnessComp{};
	foamHardnessComp.Name = "FoamHardness";
	foamHardnessComp.Type = ImGuiComponentType::Float;
	foamHardnessComp.Data = FloatComponent{
		0.0f,
		1.0f,
		0.5f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uFoamHardness", val);
		}
	};
	imgui_add_component(&foamHardnessComp);

	ImGuiComponent foamIntensityComp{};
	foamIntensityComp.Name = "FoamIntensity";
	foamIntensityComp.Type = ImGuiComponentType::Float;
	foamIntensityComp.Data = FloatComponent{
		0.0f,
		10.0f,
		2.0f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uFoamIntensity", val);
		}
	};
	imgui_add_component(&foamIntensityComp);

	ImGuiComponent foamDistanceFadeComp{};
	foamDistanceFadeComp.Name = "FoamDistanceFade";
	foamDistanceFadeComp.Type = ImGuiComponentType::Float;
	foamDistanceFadeComp.Data = FloatComponent{
		0.0f,
		1000.0f,
		1000.0f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uFoamDistanceFade", val);
		}
	};
	imgui_add_component(&foamDistanceFadeComp);

	ImGuiComponent foamColorComp{};
	foamColorComp.Name = "FoamColor";
	foamColorComp.Type = ImGuiComponentType::Color;
	foamColorComp.Data = ColorComponent{
		glm::vec3(1.0f, 1.0f, 1.0f),
		[&directionalLight](glm::vec3 val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_vec3(pShader, "uFoamColor", val);
		}
	};
	imgui_add_component(&foamColorComp);

	ImGuiComponent seaColorComp{};
	seaColorComp.Name = "OceanColor";
	seaColorComp.Type = ImGuiComponentType::Color;
	seaColorComp.Data = ColorComponent{
		glm::vec3(0.01f, 0.05f, 0.05f),
		[&directionalLight](glm::vec3 val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_vec3(pShader, "uSeaColor", val);
		}
	};
	imgui_add_component(&seaColorComp);

	ImGuiComponent seaColorSurfaceComp{};
	seaColorSurfaceComp.Name = "OceanColorSurface";
	seaColorSurfaceComp.Type = ImGuiComponentType::Color;
	seaColorSurfaceComp.Data = ColorComponent{
		glm::vec3(0.01f, 0.05f, 0.05f),
		[&directionalLight](glm::vec3 val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_vec3(pShader, "uSeaColorSurface", val);
		}
	};
	imgui_add_component(&seaColorSurfaceComp);

	ImGuiComponent fogDistanceFadeComp{};
	fogDistanceFadeComp.Name = "FogDistance";
	fogDistanceFadeComp.Type = ImGuiComponentType::Float;
	fogDistanceFadeComp.Data = FloatComponent{
		0.0f,
		1000.0f,
		500.0f,
		[](float val)
		{
			Shader* pShader = al_get_shader_ptr(ShaderName::basic_shader);
			set_uniform_float(pShader, "uFogDistance", val);
		}
	};
	imgui_add_component(&fogDistanceFadeComp);
	
#pragma endregion
	// ^^^ ----------------------------
	while (!window_should_close())
	{
#pragma region region_time_and_input
		if(!calculate_delta_time(window_get_time_since_start(), DESIRED_FPS)) continue;
		update_input();
#pragma endregion

		imgui_render();

#pragma region region_update_state
		if (is_input_pressed(Input::Escape))
		{
			cursor_set_state(CursorLockType::Free, true);
			locked = false;
		}
		if (!imgui_has_cursor())
		{
			
			if (is_mouse_button_pressed(MouseButton::LeftClick))
			{
				cursor_set_state(CursorLockType::CenterLock, false);
				locked = true;
			}

			if (locked)
			{
				camera_update(&camera);
			}
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

		imgui_finish_render();
		renderer_finish_render();
	}

	
	window_terminate();
	return 0;
}


