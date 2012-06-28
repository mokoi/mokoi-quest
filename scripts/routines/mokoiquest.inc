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
#include <mokoi>
#include <entities>
#include <graphics>
#include <string>
#include <questtypes>

stock MaskGetValue2(rx,ry)
{
	GraphicsDraw(" ", RECTANGLE, rx, ry, 5, 2, 2, 0x00FFFFFF);
	return MaskGetValue(rx,ry);
}


/* variables */
stock Fixed:_angle_ = 0.0;
stock animdir = SOUTH;
stock Fixed:_speed_ = 0.0;
stock Fixed:_x_, Fixed:_y_, Fixed:_z_;
stock dx, dy, dz;
stock dw = 16, dh = 16;
stock ox = 0, oy = 0;
stock locations[6][4];
stock _hit[2];
stock bool:_flipy_ = false;


#include <functions>
stock Fixed:Dir2Angle(d)
{
	d %= 8;
	return fixed(d*45);
}

stock Angle2Dir(Fixed:a, Fixed:offset = 0.0)
{
	a += offset;
	a %= 360.0;
	return fround(a) / 45;
}

stock StorePosition()
{
	for ( new i = 5; i > 0; i-- )
	{
		locations[i][0] = locations[i-1][0];
		locations[i][1] = locations[i-1][1];
		locations[i][2] = locations[i-1][2];
		locations[i][3] = locations[i-1][3];
	}
	locations[0][0] = dx;
	locations[0][1] = dy;
	locations[0][2] = dz;
	locations[0][3] = _dir_;
}

stock UpdateDisplayPosition()
{
	dx = fround(_x_, round_unbiased);
	dy = fround(_y_, round_unbiased);
	dz = fround(_z_, round_unbiased);
}

stock round_method:move_round[4][2] = { {round_floor, round_ceil}, {round_ceil, round_ceil}, {round_ceil, round_floor}, {round_floor, round_floor}};
stock EntityMove( maxmask, push_off = true )
{
	new Fixed:speed =  _speed_ * GameFrame2();
	new results = 0;
	
	if (speed > 1.0)
	{
		while ( speed > 1.0 )
		{
			results += EntityMoveCode(maxmask,1.0, push_off);
			speed -= 1.0;
		}
	}
	results += EntityMoveCode(maxmask,speed, push_off);
	return results;
}

stock EntityMoveCode( maxmask, Fixed:speed, push_off = true )
{
	if (!speed)
		return false;

	new Fixed:movex = fsin(_angle_, degrees) * speed;
	new Fixed:movey = ( _flipy_ ? -fcos(_angle_, degrees) : fcos(_angle_, degrees) ) * speed;
	new na = 0; // new angle
	new r = false;

	if ( MoveCheck(na, _angle_, maxmask, movex, movey) )
	{
		_x_ += movex;
		_y_ += movey;
		r = true;
	}
	else if ( push_off )
	{
		if ( na )
		{
			_angle_ += fixed(na);
			movex = fsin(_angle_, degrees);
			movey = fcos(_angle_, degrees);
			_x_ += movex/4.0;
			_y_ += movey/4.0;
			r = true;
		}
		else
		{
			_x_ = fixed( fround(_x_, move_round[animdir][0]) );
			_y_ = fixed( fround(_y_, move_round[animdir][1]) );
		}
	}
	else
	{
		_x_ = fixed( fround(_x_, move_round[animdir][0]) );
		_y_ = fixed( fround(_y_, move_round[animdir][1]) );
	}
	EntitySetPosition(_x_, _y_, _z_);
	UpdateDisplayPosition();
	StorePosition();

	if ( _angle_ < 0.0 )
		_angle_ += 360.0;
	else if ( _angle_ >= 360.0 )
		_angle_ -= 360.0;
	return r;
}


stock MoveCheck(&a, Fixed:angle, maxmask, Fixed:movex, Fixed:movey)
{
	if ( angle < 0.0 )
		angle += 360.0;
	else if ( angle >= 360.0 )
		angle -= 360.0;

	_dir_ = fround(angle) / 45;
	_dir_ = clamp(_dir_, 0, 7);
	animdir = clamp((_dir_/2), 0, 3);

	new checks[8][3];

	UpdatePoints( checks, fround(movex, move_round[animdir][0]), fround(movey, move_round[animdir][1]) );

	new l,r,q = _dir_;
	l = (_dir_ >= 7 ? 0 : _dir_ + 1);
	r = (_dir_ == 0 ? 7 : _dir_ - 1);

	new maskv = MaskGetValue2( checks[q][0], checks[q][1]);
	_hit[0] = MaskGetValue2( checks[l][0], checks[l][1]);
	_hit[1] = MaskGetValue2( checks[r][0], checks[r][1]);
	if( maskv > 255 )
	{
		return !a;
	}
	else if ( maskv > maxmask )
	{
		if ( MaskGetValue2( checks[q][0], checks[q][1]) > maxmask )
		{
			a = 0;
			return false;
		}
		return false;
	}
	else
	{
		if ( _hit[0] > maxmask )
			a -= 30;
		else
			_hit[0] = 0;
		if ( _hit[1] > maxmask )
			a += 30;
		else
			_hit[1] = 0;
	}
	
	return !a;

}

stock UpdatePoints( _points_[8][3], bx, by )
{
	_points_[SOUTH][XAXIS] = dx + (dw/2) + ox + bx;
	_points_[SOUTH][YAXIS] = dy + (dh) + oy + by;

	_points_[SOUTHEAST][XAXIS] = dx + (dw) + ox + bx;
	_points_[SOUTHEAST][YAXIS] = dy + (dh) + oy + by;

	_points_[EAST][XAXIS] = dx + (dw) + ox + bx;
	_points_[EAST][YAXIS] = dy + (dh/2) + oy + by;

	_points_[NORTHEAST][XAXIS] = dx + (dw) + ox + bx;
	_points_[NORTHEAST][YAXIS] = dy + oy + by;

	_points_[NORTH][XAXIS] = dx + (dw/2) + ox + bx;
	_points_[NORTH][YAXIS] = dy + oy + by;

	_points_[NORTHWEST][XAXIS] = dx + ox + bx +1;
	_points_[NORTHWEST][YAXIS] = dy + oy + by +1;

	_points_[WEST][XAXIS] = dx + ox + bx;
	_points_[WEST][YAXIS] = dy + (dh/2) + oy + by;

	_points_[SOUTHWEST][XAXIS] = dx + ox + bx;
	_points_[SOUTHWEST][YAXIS] = dy + (dh) + oy + by ;

}
