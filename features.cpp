#include "features.h"
#include "../Hooking/natives/natives.h"
#include "Cheat.h"
#include "enums.h"
#include "States.h"
#include "game.h"
#include "log.h"

#define DEG2RAD(deg) ((deg) * 0.01745329251f)

States state;

void features::Loop()
{
	if (GetAsyncKeyState(VK_F3) & 1)
		Godmode(!state.GodmodeBool);

	if (GetAsyncKeyState(VK_F4) & 1)
	{
		state.NoclipBool = !state.NoclipBool;
		if (!state.NoclipBool)
		{
			Entity target = !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? PLAYER::PLAYER_PED_ID() : PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_ID(), false);
			ENTITY::SET_ENTITY_COLLISION(target, true, true);
			ENTITY::FREEZE_ENTITY_POSITION(target, false);
		}
	}

	state.NoclipBool ? Noclip() : void();
}


void features::Godmode(bool toggle)
{
	ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), toggle);
	state.GodmodeBool = toggle;
	INIT_WARN("Enable godmode");
}



void features::Noclip()
{
    Vector3 camPos = CAM::GET_GAMEPLAY_CAM_COORD();
    float camHeading = CAM::GET_GAMEPLAY_CAM_ROT(2).z;

    float speed = 0.5f; // Скорость движения
    float dx = 0.0f;
    float dy = 0.0f;

    // Чтение ввода для движения влево/вправо
    if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_LEFT_ONLY))
    {
        dx = -speed;
    }
    else if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_RIGHT_ONLY))
    {
        dx = speed;
    }

    // Поворот движения влево/вправо относительно камеры
    float headingRad = DEG2RAD(camHeading);
    float cosHeading = cosf(headingRad);
    float sinHeading = sinf(headingRad);

    float tempX = dx * cosHeading - dy * sinHeading;
    dy = dx * sinHeading + dy * cosHeading;
    dx = tempX;

    // Перемещение персонажа
    ENTITY::APPLY_FORCE_TO_ENTITY(PLAYER::PLAYER_PED_ID(), 1, dx, dy, 0.0f, 0.0f, 0.0f, 0.0f, 0, true, true, true, false, true);
}