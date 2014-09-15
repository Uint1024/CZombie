#include "common.h"

float C_AngleBetween2Points(int xa, int ya, int xb, int yb)
{
    float adjacent  =    xb - xa;
	float opposite  =    yb - ya;

	float angle     =    atan2f(opposite, adjacent);
	return angle;
}
