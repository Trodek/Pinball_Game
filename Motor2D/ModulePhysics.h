#pragma once
#include "j1Module.h"
#include "p2Point.h"
//#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -10.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum Layers {
	LAUNCH = 1, BOARD, TOP, KICKERS, BALL
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	j1Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public j1Module, public b2ContactListener // TODO
{
public:
	ModulePhysics();
	~ModulePhysics();

	bool Start();
	bool PreUpdate();
	bool PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, int cat = BOARD, int mask = BALL);
	PhysBody* CreateStaticCircle(int x, int y, int radius, int cat = BOARD, int mask = BALL);
	PhysBody* CreateRectangle(int x, int y, int width, int height, int cat = BOARD, int mask = BALL);
	PhysBody* CreateStaticRectangle(int x, int y, int width, int height, int cat = BOARD, int mask = BALL);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, int cat = BOARD, int mask = BALL);
	PhysBody* CreateStaticPolygon(int x, int y, int* points, int size, int cat = BOARD, int mask = BALL);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, int cat = BOARD, int mask = BALL);
	PhysBody* CreateChain(int x, int y, int* points, int size, int cat = BOARD, int mask = BALL);
	PhysBody* CreateStaticChain(int x, int y, int* points, int size, int cat = BOARD, int mask = BALL);

	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* anchor, PhysBody* body, iPoint anchor_offset, iPoint body_offset, bool enable_limit, float max_angle, float min_angle, bool enable_motor, int motor_speed, int max_torque);

	void SetGround(b2Body* body) {
		ground = body;
	}

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint = nullptr;
	b2Body* ground;
	b2Body* selected = nullptr;
};