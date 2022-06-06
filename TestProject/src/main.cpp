#include "Entry.h"

#include "Core/ModuleManager.h"

#include "Scene/Planet/Planet.h"
#include "Scene/Planet/PlanetQuad.h"

class TestModule : public Module
{
	DEFINE_MODULE(TestModule, Module);

	Binding* Binding_Exit;
	Binding* Binding_Focus;

public:
	TestModule()
	{
	}

	virtual void OnInitialize() override
	{
		auto bindingModule = ModuleManager::Get<BindingModule>();
		Binding_Exit = bindingModule->GetByName("exit");
		Binding_Focus = bindingModule->GetByName("focus");

		Application::Get()->SetScene(Scene::Create("Assets/Scenes/game.scene"));
	}

	virtual void OnTick(double DeltaTime) override
	{
		static bool isSecondPress;

		if (Binding_Exit->GetState() == InputState::PRESSED)
		{
			Input::ShowCursor(true);

			if (isSecondPress)
			{
				Application::Get()->Shutdown();
				return;
			}

			isSecondPress = true;
		}
		else if (!Input::IsCursorVisible())
		{
			isSecondPress = false;
		}

		if (Binding_Focus->GetState() >= InputState::PRESSED)
		{
			Input::ShowCursor(false);
		}
	}
};

BEGIN_MODULE_LINK(TestModule);
END_MODULE_LINK();

int JingleEngineMain(Application* app)
{
	auto bindingModule = ModuleManager::Get<BindingModule>();

	bindingModule->RegisterCombos("exit", { {{KeyCode::KC_ESCAPE, InputType::KEY}} });
	bindingModule->RegisterCombos("focus", { {{MouseCode::MC_BUTTON_1, InputType::MOUSE}} });

	bindingModule->RegisterCombos("forward", { {{KeyCode::KC_W, InputType::KEY}} });
	bindingModule->RegisterCombos("backward", { {{KeyCode::KC_S, InputType::KEY}} });
	bindingModule->RegisterCombos("left", { {{KeyCode::KC_A, InputType::KEY}} });
	bindingModule->RegisterCombos("right", { {{KeyCode::KC_D, InputType::KEY}} });

	bindingModule->RegisterCombos("incline_increase", { {{KeyCode::KC_UP, InputType::KEY}} });
	bindingModule->RegisterCombos("incline_decrease", { {{KeyCode::KC_DOWN, InputType::KEY}} });
	bindingModule->RegisterCombos("roll_left", { {{KeyCode::KC_LEFT, InputType::KEY}} });
	bindingModule->RegisterCombos("roll_right", { {{KeyCode::KC_RIGHT, InputType::KEY}} });

	bindingModule->RegisterCombos("buffer_prev", { {{KeyCode::KC_O, InputType::KEY}} });
	bindingModule->RegisterCombos("buffer_next", { {{KeyCode::KC_P, InputType::KEY}} });

	bindingModule->RegisterCombos("toggle_facemode", { {{KeyCode::KC_F, InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_backfaceculling", { {{KeyCode::KC_C, InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_depthtesting", { {{KeyCode::KC_Z, InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_vsync", { {{KeyCode::KC_V, InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_debug", { {{KeyCode::KC_G, InputType::KEY}} });

	bindingModule->RegisterCombos("turbo", { {{KeyCode::KC_LSHIFT, InputType::KEY}} });
	bindingModule->RegisterCombos("mouse_scroll", { {{MouseCode::MC_WHEEL_UP, InputType::MOUSE}, {MouseCode::MC_WHEEL_DOWN, InputType::MOUSE}} });

	return 0;
}
