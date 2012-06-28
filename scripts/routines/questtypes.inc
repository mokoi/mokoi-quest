/***********************************************
 * Copyright © Luke Salisbury
 *
 * You are free to share, to copy, distribute and transmit this work
 * You are free to adapt this work
 * Under the following conditions:
 *  You must attribute the work in the manner specified by the author or licensor (but not in any way that suggests that they endorse you or your use of the work). 
 *  You may not use this work for commercial purposes.
 * Full terms of use: http://creativecommons.org/licenses/by-nc/3.0/
 * Changes:
 *     2010/01/11 [luke]: new file.
 ***********************************************/

#include <entities>
#define SOUTH 0		//0 deg
#define SOUTHEAST 1	//45 deg
#define EAST 2		//90 deg
#define NORTHEAST 3	//135 deg
#define NORTH 4		//180 deg
#define NORTHWEST 5	//225 deg
#define WEST 6		//270 deg
#define SOUTHWEST 7	//315 deg

enum _STATE {
	ALIVE,
	STANDING,
	MOVING,
	HIT,
	KNOCKED,
	JUMPING,
	SWIMMING,
	FLOATING,
	CHASING,
	USING,
	LIFTING,
	STANDLIFT,
	WALKLIFT,
	FALLING,
	FALLING2,
	STUNNED,
	FROZEN,
	SHOCKED,
	BURNING,
	SLEEPING,
	WAITING,
	LEAPING,
	PUSHING,
	PULLING,
	DYING,
	DEAD,
	GONE
}

enum _TYPE {
	TYPE_UNKNOWN,
	TYPE_ENEMY,
	TYPE_DOOR,
	TYPE_WEAPON,
	TYPE_AWAKING,
	TYPE_TRANSPORT,
	TYPE_PUSHABLE,
	TYPE_SWITCH,
	TYPE_ITEM,
	TYPE_PLAYER
}

/* Item Types */
#define ITEM_BOMBS 1

/* Mask Values */
/* */
#define MASK_CLEAR 0
#define MASK_DIRT 4
#define MASK_GRASS 8
#define MASK_PUDDLE 16
#define MASK_ICE 24

#define MASK_NORMALGROUND 64

#define MASK_ENEMYSOLID1 120 // Air Based

#define MASK_HOLE1 140
#define MASK_HOLE2 142

#define MASK_ENEMYSOLID2 160 // Ground Based

#define MASK_WALK 162
#define MASK_AUTOWALK 164

#define MASK_PLAYERSOLID 200
#define MASK_PLAYERSOLID2 202

#define MASK_BLOCK 240

#define MASK_LEAP 250
#define MASK_SOLID3 253
#define MASK_SOLID2 254
#define MASK_SOLID 255

/* Weapon Types */
#define AEXPLOSION 1
#define ASWORD 2
#define APLAYER 4
#define AFIRE 8
#define AICE 16
#define ASTUN 32
#define ASHIELD 64
#define AMAGIC 128

stock TransitionPlayer( ntarget[], nx, ny, nplayer[], nsection[] )
{
	if ( nx < 0 && ny < 0 )
	{
		MapGetGrid(nx, ny);
	}
	EntityPublicFunction("transition", "SetTarget", "snnss", ntarget, nx, ny, nplayer, nsection );
}


/*
stock AwakingEntity(entity[], id[], rect)
{
	EntityPublicFunction(entity, "Awaking", "sn", selfid, rect);
}
stock HitEntity(entity[], attacker[], angle, dist, attack, damage, x, y, rect)
{
	EntityPublicFunction(entity, "Hit", "snnnnnn", selfid, angle, dist, APLAYER, 0, dx,dy, rect);
}

			else if ( type == TYPE_TRANSPORT )
			{
				//MovePlayer(player, dir)
				if ( EntityPublicFunction(current, "MovePlayer", "sn", selfid, _dir_) )
				{
					 _state_ = STANDING;
				}
			}
			else if ( type == TYPE_PUSHABLE && _state_ == PUSHING )
			{
				// public Push(attacker[], rect, angle)
				EntityPublicFunction(current, "Push", "snn", selfid, rect, angle);
			}
			else if ( type == TYPE_SWITCH )
			{
				// public Pressed(attacker[])
				EntityPublicFunction(current, "Pressed", "s", selfid);
			}
			else if ( type == TYPE_ITEM )
			{
				// public Pickup(attacker[])
				if ( EntityPublicFunction(current, "Pickup", "s", selfid) )
*/
