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
#include <mokoiquest>
#include <core>
#include <graphics>

forward public Hit( attacker[], angle, dist, type, x, y);

new bouncing = false;
new initialBounce;
new willTimeOut = false;
new TempBounce;    
new ImageAlpha = 255;
new Fixed:bounceVelocity = 0.00;
new Fixed:bounceY;
new param;
new width;
new height;
new worth = 0;

new playSound = true;
new pickable = true;

new obj1 = -1;
new obj2 = -1;

new type = 1;
new sprite_type = 's';
new sprite[64];
new numstring = 0;

public Init(...)
{
	//new count = getarg(1);
	worth = 0;//count;
	dw  = MiscGetWidth(sprite);
	dh = MiscGetHeight(sprite);

	EntityGetPosition(_x_,_y_, _z_);
	UpdateDisplayPosition();

	obj1 = ObjectCreate(sprite, sprite_type, dx, dy, 4);
	obj2 = ObjectCreate("dropitems.png:shadow", SPRITE, dx, dy + dh - 13, 2, 0, 0);
	InitDropItem( worth );
	//LanguageString(line, returnstring[], maxlength=sizeof returnstring);
}


public Close()
{
	ObjectDelete(obj1);
	ObjectDelete(obj2);
}

main()
{
	if ( pickable )
	{
		EntityGetPosition(_x_,_y_, _z_);
		UpdateDisplayPosition();
		CollisionSet(SELF, 0, 0, dx, dy, dw, dh); // Set up 1 collision rectangle for the item
		
		//CallFunction("_itemlib", false, "DrawItemNumber", "nnnnn", arrowsWorth, x + 4, y + 8  - floatround(bounceY), y + height, ImageAlpha);
		ObjectPosition(obj1, dx, dy - fround(bounceY), 3, 0, 0);
		
	}	
	// If the Item has been out for too long then make it disappear
	if ( willTimeOut )
		TimeOut();
	
	if ( bouncing )
		Bounce();
}


//----------------------------------------
// Name: TimeOut()
//----------------------------------------
TimeOut()
{
	static timer = 0;
	static alpha = 25500;
	
	timer += GameFrame();
	
	if ( timer > 10000 )
	{
		alpha -= GameFrame();
		ImageAlpha = alpha / 10;
		
		// If the Timer runs out then delete the item
		if ( alpha < 0 )
			EntityDelete();
	}
}

//----------------------------------------
// Name: StartBounce()
//----------------------------------------
public StartBounce()
{
	/* StartBounce is called from another script using CallFunction, it should be
	called just after the item is created to make it "bounce" a bit. */

	CollisionSet(SELF, 0, 0);
	
	bouncing = true;
	bounceVelocity = 160.00;
	bounceY = 0.00;
	TempBounce = 120;
	
	initialBounce = _y_;
	willTimeOut = true;
}

//----------------------------------------
// Name: Bounce()
//----------------------------------------
Bounce()
{
	bounceVelocity -= GameFrame();
	bounceY += (bounceVelocity * GameFrame());
		
	if ( initialBounce - fround(bounceY) > initialBounce )
	{
		// Check if the item has more bounce left in it
		if ( TempBounce < 20 )
		{
			bouncing = false;
			pickable = true;
		}
		else
		{
			bounceVelocity = fixed(TempBounce);
			TempBounce = TempBounce / 2;
			bounceY = 0.00;
		}
	}
}

//----------------------------------------
// Name: Hit()
//----------------------------------------
public Hit( attacker[], angle, dist, type, x, y)
{
	//EntityCallFunction(attacker, "Increase", type, worth);
	
	if ( playSound )
		AudioPlaySound("powerup.wav");
	
	// Delete this entity if its been taken
	EntityDelete();
}