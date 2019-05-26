#pragma once


enum EQUIP
{
	EQ_NONE,
	EQ_WEAPON,
	EQ_ARMOR,
	EQ_RUBY,
	EQ_END
};

enum OBJECT_TYPE
{
	OT_NONE,
	OT_PLAYER,
	OT_MONSTER,
	OT_WEAPON,
	OT_ARMOR,
	OT_RUBY
};

enum BATTLE
{
	B_NONE,
	B_PLAYER_DIE,
	B_MONSTER_DIE
};

enum JOB
{
	JOB_NONE,
	JOB_KNIGHT,
	JOB_ARCHER,
	JOB_WIZARD,
	JOB_THIEF,
	JOB_SWORD,
	JOB_GIANT,
	JOB_ELF,
	JOB_DEMON,
	JOB_DRAGON,
	JOB_ORC,
	JOB_PLUS
};

enum SAVE_CASE
{
	SC_NONE,
	SC_ONE,
	SC_TWO,
	SC_THREE,
	SC_BACK,
	SC_DELETE
};

enum POINT
{
	POINT_NONE,
	POINT_ATTACK,
	POINT_ARMOR,
	POINT_AVOID,
	POINT_LUCK,
	POINT_CRITICAL
};

enum FILE_MODE
{
	FM_NONE,
	FM_READ,
	FM_WRITE
};