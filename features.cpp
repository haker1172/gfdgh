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
	Vector3 camera_direction = Game::vector::get_cam_direction();
	float speed = 0.5f;

	Entity target = !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? PLAYER::PLAYER_PED_ID() : PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
	Vector3 position = ENTITY::GET_ENTITY_COORDS(target, false);

	if (ENTITY::IS_ENTITY_A_PED(target))
		CAM::SET_FOLLOW_PED_CAM_VIEW_MODE(ThirdPersonFar);
	else
		CAM::SET_FOLLOW_VEHICLE_CAM_ZOOM_LEVEL(ThirdPersonFar);

	WEAPON::GIVE_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xA2719263, 0, false, true);
	ENTITY::SET_ENTITY_COLLISION(target, false, false);
	ENTITY::FREEZE_ENTITY_POSITION(target, true);

	Vector3 rotation = CAM::GET_GAMEPLAY_CAM_ROT(0);
	ENTITY::SET_ENTITY_ROTATION(target, rotation.x, rotation.y, rotation.z, 2, true);

	if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_UP_ONLY))
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(target, position.x + (camera_direction.x * speed), position.y + (camera_direction.y * speed), position.z + (camera_direction.z * speed), false, false, false);
	}
	else if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_DOWN_ONLY))
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(target, position.x - (camera_direction.x * speed), position.y - (camera_direction.y * speed), position.z - (camera_direction.z * speed), false, false, false);
	}
}
