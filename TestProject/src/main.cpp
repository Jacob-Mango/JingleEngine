#include "Entry.h"

#include "Scene/Planet/Planet.h"
#include "Scene/Planet/PlanetQuad.h"

int JingleEngineMain(Application* app)
{
	EntityTypeManager::Register<EntityType>();
	EntityTypeManager::Register<MeshEntityType>();
	EntityTypeManager::Register<LightType>();
	EntityTypeManager::Register<CameraType>();
	EntityTypeManager::Register<DebugType>();
	EntityTypeManager::Register<PlanetType>();
	EntityTypeManager::Register<PlanetQuadType>();

	BindingManager::RegisterCombos("exit", { {{KeyCode::ESCAPE, InputType::KEY}} });
	BindingManager::RegisterCombos("focus", { {{MouseCode::BUTTON_1, InputType::MOUSE}} });

	BindingManager::RegisterCombos("forward", { {{'w', InputType::KEY}} });
	BindingManager::RegisterCombos("backward", { {{'s', InputType::KEY}} });
	BindingManager::RegisterCombos("left", { {{'a', InputType::KEY}} });
	BindingManager::RegisterCombos("right", { {{'d', InputType::KEY}} });

	BindingManager::RegisterCombos("incline_increase", { {{KeyCode::UP, InputType::KEY}} });
	BindingManager::RegisterCombos("incline_decrease", { {{KeyCode::DOWN, InputType::KEY}} });
	BindingManager::RegisterCombos("roll_left", { {{KeyCode::LEFT, InputType::KEY}} });
	BindingManager::RegisterCombos("roll_right", { {{KeyCode::RIGHT, InputType::KEY}} });

	BindingManager::RegisterCombos("buffer_prev", { {{'o', InputType::KEY}} });
	BindingManager::RegisterCombos("buffer_next", { {{'p', InputType::KEY}} });

	BindingManager::RegisterCombos("toggle_facemode", { {{'f', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_backfaceculling", { {{'c', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_depthtesting", { {{'z', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_osd", { {{'h', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_vsync", { {{'v', InputType::KEY}} });
	BindingManager::RegisterCombos("toggle_debug", { {{'g', InputType::KEY}} });

	BindingManager::RegisterCombos("turbo", { {{KeyCode::LSHIFT, InputType::KEY}} });
	BindingManager::RegisterCombos("mouse_scroll", { {{MouseCode::WHEEL_UP, InputType::MOUSE}, {MouseCode::WHEEL_DOWN, InputType::MOUSE}} });

	return 0;
}

/*
void SpaceGame::OnStart()
{
	SetScene(new Scene());

	auto config = Config::Load("Assets/Scenes/game.scene");
	std::cout << "game.scene:: " << (*config).AsString() << std::endl;

	auto& types = (*config)["defaultTypes"];
	for (int i = 0; i < types.Count; i++)
	{
		EntityTypeManager::Load(types[i]);
	}

	GetScene()->LoadScene((*config)["entities"]);
}

void SpaceGame::OnTick(double DeltaTime)
{
	Application::OnTick(DeltaTime);

}

int main(int argc, char** argv)
{
	OUT_LINE("main");
	SpaceGame* app = new SpaceGame();
	app->Start();
	return 0;
}
*/