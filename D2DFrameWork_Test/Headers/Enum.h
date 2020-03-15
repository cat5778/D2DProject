#pragma once

enum DISPLAY_MODE
{
	MODE_FULL,
	MODE_WIN
};
enum LIGHT_TYPE
{
	HORIZONTAL_LIGHT,
	VERTICAL_LIGHT
};
enum TEXTURE_TYPE
{
	TEXTURE_SINGLE,
	TEXTURE_MULTI
};
enum MONTER_TYPE
{
	SLIME,
	FUNGUS,
	MONK,
	PRIEST,
	BOSS1,
	BOSS2,
	MONSTER_END
};
enum COLLSION_TYPE
{
	BUTTON_COLLISION,
	PLAYER_STEP_COLLISION,
	PLAYER_HITBOX_COLLISION,
	PLAYER_PROJECTILE_COLLISION,
	MONSTER_HITBOX_COLLISION,
	MONSTER_PROJECTILE_COLLISION,
	OBJECT_HITBOX_COLLISION,
	BOSS_HITBOX_COLLISION,
	COLLSION_END
};
enum OBJECT_STATE
{
	STATE_IDLE,
	STATE_ATTACK,
	STATE_MOVE,
	STATE_BEATTACK,
	STATE_DEAD,
	STATE_END
};
enum OBJECT_TYPE
{
	OBJECT_TERRAIN,
	OBJECT_OBSTACLE,
	OBJECT_TRAP,
	OBJECT_BUILDING,
	OBJECT_MONSTER,
	OBJECT_NPC,
	OBJECT_GRASS,
	OBJECT_TREE,
	OBJECT_PLAYER,
	OBJECT_EFFECT,
	OBJECT_UI,
	OBJECT_BUTTON,
	OBJECT_CHOOSE,
	OBJECT_TITLE,
	OBJECT_LIGHT,
	OBJECT_WEAPON,
	OBJECT_PROJECTILE,
	OBJECT_END
};
enum OBSTACLE_TYPE
{
	OBSTACLE_TREE,
	OBSTACLE_GRASS,
	OBSTACLE_BUILDING,
	OBSTACLE_TP,
	OBSTACLE_FENCE,
	OBSTACLE_LAMP,
	OBSTACLE_END
};
enum WEAPONE_TYPE
{
	WEAPONE_FIST,
	WEAPONE_JAVELIN,
	WEAPONE_JAVELIN1,
	WEAPONE_BOW,
	WEAPONE_BOW1,
	WEAPONE_END
};
enum SCENE_TYPE
{
	SCENE_LOGO,
	SCENE_STAGE,
	SCENE_TUTORIAL,
	SCENE_TOWN,
	SCENE_BOSS,
	SCENE_MEVIUS,
	SCENE_END
};
enum FILL_TYPE
{
	FILL_HP,
	FILL_MANA,
	FILL_END
};