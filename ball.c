#include "entity.h"
/*
Entity* createBall()
{
	Entity* ball = (Entity*)malloc(sizeof(Entity));
	ball->t = Ball;
	ball->x = 50;
	ball->y = 50;
	ball->dx = 10;
	ball->dy = 15;
	createBox(ball, 40, 40);

	return ball;
}


void moveBall(Entity* ball, Entity* other)
{
	bool trajectoryCutShot = false;
	float tempDx = ball->dx;
	float tempDy = ball->dy;
	Box* tempBox = createTempBox(ball);
	Direction bounced_off_screen = checkOutOfScreen(tempBox);
	Direction bounced_off_object = checkCollision(&ball->box, tempBox, &other->box);

	if (bounced_off_screen == Bottom || bounced_off_screen == Top)
	{
		ball->dy = -ball->dy;
	}
	else if (bounced_off_screen == Left || bounced_off_screen == Right)
	{
		ball->dx = -ball->dx;
	}


	if (bounced_off_object == Left)
	{
		if (other->box.right != ball->box.left)
		{
			trajectoryCutShot = true;
			moveEntity(ball, other->box.right - ball->box.left, ball->dy);
		}
		else
		{
			ball->dx = -ball->dx;
		}
	}

	if (bounced_off_object == Right)
	{

		if (other->box.left != ball->box.right)
		{
			trajectoryCutShot = true;
			moveEntity(ball, other->box.left - ball->box.right, ball->dy);
		}
		else
		{
			ball->dx = -ball->dx;

		}
	}

	if (bounced_off_object == Bottom)
	{
		if (other->box.top != ball->box.bottom)
		{
			trajectoryCutShot = true;
			moveEntity(ball, ball->dx, other->box.top - ball->box.bottom);
		}
		else
		{
			ball->dy = -ball->dy;
		}
	}

	if (bounced_off_object == Top)
	{
		if (other->box.bottom != ball->box.top)
		{
			trajectoryCutShot = true;
			moveEntity(ball, ball->dx, other->box.bottom - ball->box.top);
		}
		else
		{
			ball->dy = -ball->dy;
		}
	}


	if (!trajectoryCutShot)
	{
		moveEntity(ball, ball->dx, ball->dy);
	}

	}*/
