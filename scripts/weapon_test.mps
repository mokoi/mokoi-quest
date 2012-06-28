#include <mokoiquest>
#include <entities>
#include <graphics>

forward public Use( xobj, dir )

new obj = -1;
new selfid[64] = "asd";
new timer = 0;

public Init(...)
{
	oy += 40;
}

public Close()
{
	ObjectDelete(obj);
}

main()
{

}


weapon_begin( xobj )
{
	if ( timer )
		return;
	timer = 1200;
	obj = ObjectCreate("sword01.png:swing_down", ANIMATION, dx, dy + oy, 4, 0, 0);
	ObjectEffect(obj, WHITE, _, _, _, 0, _, _);
	ObjectReplace(xobj, "p01swing.png:swing-front", ANIMATION);
	CollisionSet(SELF, 0, TYPE_WEAPON,  dx + 8, dy +10 +oy, 8, 32);
}

weapon_ended( xobj )
{
	if ( Countdown(timer) )
	{
		timer = 0;
		ObjectDelete(obj);
		obj = -1;
		CollisionSet(SELF, 0, 0);
		return 0;
	}
	return 1;
}

public Use( xobj, dir )
{
	EntityGetPosition(_x_, _y_);
	UpdateDisplayPosition();
	
	weapon_begin( xobj );

	if ( CollisionCalculate() )
	{
		new current[64];
		new angle;
		new dist;
		new rect;
		new type;

		while ( CollisionGetCurrent(SELF, current, angle, dist, rect, type) )
		{
			if ( type == TYPE_ENEMY )
			{
				//// public Hit( attacker[], angle, dist, attack, damage, x, y )
				EntityPublicFunction(current, "Hit", "sdddddd", selfid, angle, dist, ASWORD, 1000, dx, dy);
			}
		}
	}
	
	return weapon_ended( xobj );
}