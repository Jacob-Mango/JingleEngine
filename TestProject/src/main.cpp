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
	virtual void OnInitialize() override
	{
		auto bindingModule = ModuleManager::Get<BindingModule>();
		Binding_Exit = bindingModule->GetByName("exit");
		Binding_Focus = bindingModule->GetByName("focus");

		Application::Get()->SetScene(Scene::Create("Assets/Scenes/game.scene"));
	}

	virtual void OnTick(double DeltaTime) override
	{
		static bool isFirstPress;

		if (!Input::IsCursorInViewport())
		{
			return;
		}

		if (Binding_Exit->GetState() == InputState::PRESSED)
		{
			Input::ShowCursor(true);

			if (isFirstPress)
			{
				Application::Get()->Shutdown();
				return;
			}

			isFirstPress = true;
		}
		else if (!Input::IsCursorVisible())
		{
			isFirstPress = false;
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

	bindingModule->RegisterCombos("exit", { {{InputCode::KC_ESCAPE, InputType::KEY}} });
	bindingModule->RegisterCombos("focus", { {{InputCode::MC_BUTTON_1, InputType::MOUSE}} });

	bindingModule->RegisterCombos("forward", { {{InputCode::KC_W, InputType::KEY}} });
	bindingModule->RegisterCombos("backward", { {{InputCode::KC_S, InputType::KEY}} });
	bindingModule->RegisterCombos("left", { {{InputCode::KC_A, InputType::KEY}} });
	bindingModule->RegisterCombos("right", { {{InputCode::KC_D, InputType::KEY}} });

	bindingModule->RegisterCombos("incline_increase", { {{InputCode::KC_UP, InputType::KEY}} });
	bindingModule->RegisterCombos("incline_decrease", { {{InputCode::KC_DOWN, InputType::KEY}} });
	bindingModule->RegisterCombos("roll_left", { {{InputCode::KC_LEFT, InputType::KEY}} });
	bindingModule->RegisterCombos("roll_right", { {{InputCode::KC_RIGHT, InputType::KEY}} });

	bindingModule->RegisterCombos("buffer_prev", { {{InputCode::KC_O, InputType::KEY}} });
	bindingModule->RegisterCombos("buffer_next", { {{InputCode::KC_P, InputType::KEY}} });

	bindingModule->RegisterCombos("toggle_facemode", { {{InputCode::KC_F, InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_backfaceculling", { {{InputCode::KC_C, InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_depthtesting", { {{InputCode::KC_Z, InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_vsync", { {{InputCode::KC_V, InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_debug", { {{InputCode::KC_G, InputType::KEY}} });

	bindingModule->RegisterCombos("turbo", { {{InputCode::KC_LSHIFT, InputType::KEY}} });
	bindingModule->RegisterCombos("mouse_scroll", { {{InputCode::MC_WHEEL_UP, InputType::MOUSE}, {InputCode::MC_WHEEL_DOWN, InputType::MOUSE}} });

	return 0;
}
