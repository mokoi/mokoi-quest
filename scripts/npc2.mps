/***********************************************
 * 
 ***********************************************/
#include <openzelda>

//   Global Data
new headDist 		= 8;        // Distance head is from the body
new shadowDist		= 1;
new Talking 		= false;    // Is the player currently talking to this NPC?
new HeadDirection	= south;	// Direction of NPCs Head
new param		= 0;
new hasHead		= false;	// not all NPCs have a seperate head
new LooksAround		= true;

new head[4][20];	// 4 strings to hold each image of the NPCs head
new stand[20];		// Holds the south standing animation ID

//----------------------------------------
// Name: Init()
//----------------------------------------
public Init(...)
{
	
	// Set some general parameters
	_type_ = TYPE_NPC;
	_speed_ = 40;

/*
		if ( param == 'a' ) // Tall Kid
		{
			AddAnimframe(stand, 0, 0, "_npc2aa1"); 
			head[1] = "_npc2ab";  // Assign the head sprites
			head[2] = "_npc2ac";
			head[3] = "_npc2ad";
			hasHead = true;
			headDist = 12;
		}
		if ( param == 'b' )	// Voodoo Man
		{
			AddAnimframe(stand, 0, 0, "_npc2b"); 
			AddAnimframe(stand, 0, 0, "_npc2b1");
			shadowDist = 10;
		}
		if ( param == 'c' )	// Fotune teller dude
		{
			AddAnimframe(stand, 0, 0, "_npc2c"); 
			AddAnimframe(stand, 0, 0, "_npc2c1");
			SetAnimSpeed(stand, 1);
			shadowDist = 10;
		}
		if ( param == 'd' )	// Sahasrahl
		{
			AddAnimframe(stand, 0, 0, "_npc2da1"); 
			AddAnimframe(stand, 0, 0, "_npc2da2");
			head[2] = "_npc2db";
			hasHead = true;
			headDist = 10;
			SetAnimSpeed(stand, 1);
			LooksAround = false;
		}
		if ( param == 'e' ) // prize guy
		{
			AddAnimframe(stand, 0, 0, "_npc2ea1"); 
			head[1] = "_npc2eb";  // Assign the head sprites
			head[2] = "_npc2ec";
			head[3] = "_npc2ed";
			hasHead = true;
			headDist = 11;
		}
		if ( param == 'f' ) // little kid
		{
			AddAnimframe(stand, 0, 0, "_npc2fa1"); 
			head[1] = "_npc2fb";  // Assign the head sprites
			head[2] = "_npc2fc";
			head[3] = "_npc2fd";
			hasHead = true;
			headDist = 9;
		}
		if ( param == 'g' ) // bug-catcher kid
		{
			AddAnimframe(stand, 0, 0, "_npc2ga1"); 
			head[1] = "_npc2gb";  // Assign the head sprites
			head[2] = "_npc2gc";
			head[3] = "_npc2gd";
			hasHead = true;
			headDist = 11;
		}
		if ( param == 'h' ) // Geeky looking guy
		{
			AddAnimframe(stand, 0, 0, "_npc2ha1"); 
			head[1] = "_npc2hb";  // Assign the head sprites
			head[2] = "_npc2hc";
			head[3] = "_npc2hd";
			hasHead = true;
			headDist = 10;
		}
	}
*/
		
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
	// Check if the Player wants to talk to this NPC
	if ( EntityCallFunction("npclib", "CheckForTalk") )
		Talking = true;
		
	// Check if the player has just finished talking to an NPC
	if ( Talking && EntityCallFunction("npclib", "Finished") )
	{
		EntityCallFunction("npclib", "AfterTalk");
		Talking = false;
	}

	// Make the NPC face the player
	if ( FacePlayer )
	{
		HeadDirection = EntityCallFunction("npclib", "FacePlayer", _dir_);
		ObjectReplace(obj_head, head[_dir_], SPRITE);
	}
	
	CollisionSet( _, 0, _x_, _y_, width[dir], height[dir] ); // Set a solid collision rectangle around the NPC
}


