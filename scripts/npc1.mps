/***********************************************
 * 
 ***********************************************/
//Used Params
//a: Bar-tender man
//b: Old lady
//c: Old Guy
//d: Priest
//e: Theif
//f: Young girl

#include <npc>

enum npcaction{
	action,
	nx,
	ny,
	msgid
}

new headDist 		= 8;        // Distance head is from the body
new shadowDist		= 1;
new param			= 0;

new bool:changed 	= false;
new bool:Talking	= false;    // Is the player currently talking to this NPC?

new _DIRECTION:HeadDirection = SOUTH;	// Direction of NPCs Head

new head[4][64];	// 4 strings to hold each image of the NPCs head
new walk[4][64];	// 4 strings to hold the animation id's of the walking animations
new stand[4][64];	// 4 strings to hold the animation id's of the walking animations

new obj_head = -1;
new obj_body = -1;
new obj_shadow = -1;

new instruction[10][npcaction];


//----------------------------------------
// Name: Init()
//----------------------------------------
public Init(...)
{
	/*
	Read Settings
	*/
	// Set some general parameters
	_type_ = TYPE_NPC;
	_speed_ = 40;

	if ( param == 'b' )		
		_speed_ = 30;
	else if ( param == 'c' )	
	{
		_speed_ = 30;
		shadowDist = 0;
	}
	else if ( param == 'e' )
		headDist = 9;
	else if ( param == 'f' )		
		headDist = 10;
		
	strformat(walk[_dir_], _, _, "%s:%s_%d", sheet, sprite, _dir_);


	obj_shadow = ObjectCreate("npc.png:shadow", SPRITE, _x_, _y_ + shadowDist, 3);
	obj_body = ObjectCreate(head[_dir_], SPRITE, _x_, _y_, 3);
	obj_head = ObjectCreate(head[_dir_], SPRITE, _x_, _y_ + headDist, 3);
}

//----------------------------------------
// Name: Close()
//----------------------------------------
public Close()
{
	ObjectDelete(obj_head);
	ObjectDelete(obj_shadow);
	ObjectDelete(obj_body);
}
//----------------------------------------
// Name: main()
//----------------------------------------
main()
{
	// Check if the player has just finished talking to an NPC
	if ( Talking && EntityCallFunction("npclib", "Finished") )
	{
		EntityCallFunction("npclib", "AfterTalk");
		Talking = false;
	}


	if ( _state_ != last )
		changed = TRUE;

	// Call a function for the NPC depending on its state
	switch( _state_ )
	{
		case standing:
			Stand(true);
		case waiting:
			Stand(false);
		case walking:
			Walk();
	}
	
	CollisionSet( _, 0, _x_, _y_, width[dir], height[dir] ); // Set a solid collision rectangle around the NPC

	last = _state_;
}

//----------------------------------------
// Name: Stand()
//----------------------------------------
Stand( FacePlayer )
{
	// Make the NPC face the player
	if ( FacePlayer )
	{
		HeadDirection = EntityCallFunction("npclib", "FacePlayer", _dir_);
	}
	
	if ( changed )
	{
		ObjectReplace(obj_body, stand[_dir_], SPRITE);
		ObjectReplace(obj_head, head[_dir_], SPRITE);
	}

}

//----------------------------------------
// Name: Walk()
//----------------------------------------
Walk()
{
	new Angle;
	new dir = GetDirection("this");
	new x = GetX("this");
	new y = GetY("this");
	HeadDirection = dir;
		
	// Get the width and height of the Current Image
	new width  = GetAnimWidth(walk[dir]);
	new height = GetAnimHeight(walk[dir]);
	
	// Move the NPC if the game is completely unpaused
	if (GetPauseLevel() == 0)
	{
		// Check for Collisions
		if (!AngleCollide("this", 5, 5, 126, 0, width / 2, height / 2))
		{
			// Get the angle between the NPC and the point it's walking to
			Angle = CalculateAngle(x, y, GetValue("this", 0), GetValue("this", 1));
			SetMoveAngle("this", Angle);
			AngleMove("this");
		}
	}
	
	// Check if the NPC can stop walking
	if ( x == walkTo[0] && y == walkTo[1] )
		_state_ = standing;
	
	// Draw the NPC
	if ( changed )
	{
		ObjectReplace(obj_body, walk[_dir_], ANIMATION);
		ObjectReplace(obj_head, head[_dir_], SPRITE);
	}
}


// *************************************************************************
// Person Script Functions below
// *************************************************************************
//----------------------------------------
// Name: SetInstruction()
//----------------------------------------
public SetInstruction( NewInstruction, instruction )
{
	if ( instruction != -1 )
	{
		if ( instruction != GetValue("this", 4) ) // Check this is the current instruction
			return 0;
	}
	
	// The quest maker can use this to reset the npc's person script
	// back to any point, or foward.
	SetValue("this", 4, NewInstruction);
	return 1;
}

//----------------------------------------
// Name: GetInstruction()
//----------------------------------------
public GetInstruction()
{
	// Return the current Person Script instruction
	return GetValue("this", 4);
}

//----------------------------------------
// Name: WalkTo()
//----------------------------------------
public WalkTo( x, y, instruction )
{
	
	if ( instruction != -1 )
	{
		if ( instruction != GetValue("this", 4) ) // Check this is the current instruction		
			return 0;
	}
		
	// Handle this in the NPC library
	return EntityCallFunction("_npclib", true, "WalkTo", "nnn", x, y, instruction);
	return 0;
}

//----------------------------------------
// Name: Wait()
//----------------------------------------
public Wait( timeToWait, instruction )
{
	if ( instruction != -1 )
	{		
		if ( instruction != GetValue("this", 4) ) // Check this is the current instruction
			return 0;
	}
	
	if ( GetPauseLevel() != 0)
		return 0;
		
	// Handle this in the NPC library
	return EntityCallFunction("_npclib", true, "Wait", "nn", timeToWait, instruction);
}

//----------------------------------------
// Name: WaitFor()
//----------------------------------------
public WaitFor( WhoToWaitFor[], TargetInstruction, instruction )
{
	if ( instruction != -1 )
	{
		if ( instruction != GetValue("this", 4) ) // Check this is the current instruction
			return 0;
	}
	
	// Handle this in the NPC library
	return EntityCallFunction("_npclib", true, "WaitFor", "snn", WhoToWaitFor, TargetInstruction, instruction);
}

//----------------------------------------
// Name: Say()
//----------------------------------------
public Say( Text[], instruction )
{
	if ( instruction != -1 )
	{
		if ( instruction != GetValue("this", 4) ) // Check this is the current instruction
			return 0;
	}
	
	// Handle this in the NPC library
	return EntityCallFunction("_npclib", true, "Say", "sn", Text, instruction);
}

//----------------------------------------
// Name: SetBodyDirection()
//----------------------------------------
public SetBodyDirection( dir, instruction )
{
	// Sets the direction of the player
	// Check this is the current instruction
	if ( instruction != GetValue("this", 4) )
		return 0;
	
	SetDirection("this", dir);
	SetValue("this", 4, instruction + 1);
}

//----------------------------------------
// Name: SetHeadDirection()
//----------------------------------------
public SetHeadDirection( dir, instruction )
{
	// Sets the direction of the player
	// Check this is the current instruction
	if ( instruction != GetValue("this", 4) )
		return 0;
	
	HeadDirection = dir;
	SetValue("this", 4, instruction + 1);
}

