#pragma once

#include <string>
#include <vector>
#include <map>

enum class BindingState
{
	NONE = 0b0000,
	RELEASED = 0b0001,
	PRESSED = 0b0010,
	HOLDING = 0b0100,
};

enum class BindingType
{
	KEY,
	MOUSE
};

#define MOUSE_WHEEL_UP 30
#define MOUSE_WHEEL_DOWN 31
#define MOUSE_WHEEL_LEFT 32
#define MOUSE_WHEEL_RIGHT 33

class BindingManager;

class Binding
{
	friend class BindingManager;

private:
	std::string m_Name = "";

	std::map<int, std::pair<BindingState, BindingType>> m_Combo;

	BindingState m_State = BindingState::NONE;
	float m_Value = 0;

public:
	void UpdateState();

	void UpdateKey(int keyCode, BindingState state, BindingType type, float value);
};

class BindingManager
{
private:
	static std::map<std::string, std::vector<Binding*>> m_Bindings;
	static bool m_KeyChanged;

	static int m_PreviousMouseX;
	static int m_PreviousMouseY;
	static int m_DeltaMouseX;
	static int m_DeltaMouseY;
	static int m_MouseX;
	static int m_MouseY;

	static bool m_CalledMouseEvent;

	static bool m_MouseLock;

	static class Application* m_Application;

public:
	static void OnEvent(union SDL_Event& event);

private:
	static void OnKeyboardEventEvent(struct SDL_KeyboardEvent& event);

	static void OnMouseMotionEvent(struct SDL_MouseMotionEvent& event);

	static void OnMouseButtonEvent(struct SDL_MouseButtonEvent& event);

	static void OnMouseWheelEvent(struct SDL_MouseWheelEvent& event);

public:
	static void Init(class Application* app);
	static void Destroy();

	static void RegisterCombo(std::string name, std::initializer_list<std::pair<int, BindingType>> combo);

	static void RegisterCombos(std::string name, std::initializer_list<std::initializer_list<std::pair<int, BindingType>>> combos);

	static BindingState Get(std::string name);
	static float GetValue(std::string name);

	static std::pair<int, int> GetMouseDelta();

	static bool IsMouseLocked();
	static void MouseLock(bool yes);

	static void Update();

	static void ResetKeyChange();
	static bool AnyKeyPressed();
};

std::ostream& operator<<(std::ostream& os, const BindingState& state);