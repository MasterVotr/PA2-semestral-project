/**
 * Project sesmestrální práce BI-PA2
 * @author votruja6
 */

#ifndef _TCoordinates_H
#define _TCoordinates_H

#include <math.h>

/**
 * Structure holds Coordinates, for easier and more wraped up use
 */

struct TCoordinates
{
	int t_PosX;
	int t_PosY;

	TCoordinates(int y = 0, int x = 0)
		: t_PosX(x)
		, t_PosY(y)
	{}

	bool operator == (TCoordinates other)
	{
		return (t_PosX == other.t_PosX) && (t_PosY == other.t_PosY);
	}

	/**
	 * Calculates the distance between this and other
	 * @param TCoordinates other
	 */
	double distanceTo(const TCoordinates & other)
	{
		return fabs
		( 
			sqrt
			( 
				( t_PosX - other.t_PosX ) * ( t_PosX - other.t_PosX )
										+ 
				( t_PosY - other.t_PosY ) * ( t_PosY - other.t_PosY )
			) 
		);
	}
};

#endif //_TCoordinates_H