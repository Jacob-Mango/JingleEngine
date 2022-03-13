#include "Entry.h"

#include "Core/ModuleManager.h"

#include "Scene/Planet/Planet.h"
#include "Scene/Planet/PlanetQuad.h"

class TestModule : public Module
{
	DEFINE_MODULE(TestModule)

	Binding* Binding_Exit;
	Binding* Binding_Focus;

public:
	virtual void OnInitialize() override
	{
		auto bindingModule = ModuleManager::Get<BindingModule>();
		Binding_Exit = bindingModule->GetByName("exit");
		Binding_Focus = bindingModule->GetByName("focus");
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

REGISTER_MODULE(TestModule);

int JingleEngineMain(Application* app)
{
	auto bindingModule = ModuleManager::Get<BindingModule>();

	bindingModule->RegisterCombos("exit", { {{KeyCode::ESCAPE, InputType::KEY}} });
	bindingModule->RegisterCombos("focus", { {{MouseCode::BUTTON_1, InputType::MOUSE}} });

	bindingModule->RegisterCombos("forward", { {{'w', InputType::KEY}} });
	bindingModule->RegisterCombos("backward", { {{'s', InputType::KEY}} });
	bindingModule->RegisterCombos("left", { {{'a', InputType::KEY}} });
	bindingModule->RegisterCombos("right", { {{'d', InputType::KEY}} });

	bindingModule->RegisterCombos("incline_increase", { {{KeyCode::UP, InputType::KEY}} });
	bindingModule->RegisterCombos("incline_decrease", { {{KeyCode::DOWN, InputType::KEY}} });
	bindingModule->RegisterCombos("roll_left", { {{KeyCode::LEFT, InputType::KEY}} });
	bindingModule->RegisterCombos("roll_right", { {{KeyCode::RIGHT, InputType::KEY}} });

	bindingModule->RegisterCombos("buffer_prev", { {{'o', InputType::KEY}} });
	bindingModule->RegisterCombos("buffer_next", { {{'p', InputType::KEY}} });

	bindingModule->RegisterCombos("toggle_facemode", { {{'f', InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_backfaceculling", { {{'c', InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_depthtesting", { {{'z', InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_osd", { {{'h', InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_vsync", { {{'v', InputType::KEY}} });
	bindingModule->RegisterCombos("toggle_debug", { {{'g', InputType::KEY}} });

	bindingModule->RegisterCombos("turbo", { {{KeyCode::LSHIFT, InputType::KEY}} });
	bindingModule->RegisterCombos("mouse_scroll", { {{MouseCode::WHEEL_UP, InputType::MOUSE}, {MouseCode::WHEEL_DOWN, InputType::MOUSE}} });

	return 0;
}
