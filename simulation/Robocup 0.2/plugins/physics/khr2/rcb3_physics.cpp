// physics.cpp : Defines the entry point for the DLL application.
//
#include "khr2_physics.h"

KHR2_Physics *g_physics = NULL;

//Initialize the resources needed for the simulation
void webots_physics_init(dWorldID world, dSpaceID space ,dJointGroupID joint)
{
    g_physics = new KHR2_Physics(world,space,joint);
}

//Called before every simulation steps
//This will be where the gyros and accelerometers are modelled.
void webots_physics_step()
{
    g_physics->Step();
}

//Called when geometries collides obviously.
//Not needed right now but could be useful later.
int webots_physics_collide(dGeomID id0,dGeomID id1)
{
    return 0;
}

//Called to clean up any allocation.
void webots_physics_cleanup()
{
    delete g_physics;
    g_physics = NULL;
}

//Called to draw additional object
//Not planning anything but could be used to give feedback of Gyros/accelerometers
void webots_physics_draw()
{
}

//Called before drawing the world.
//I do not intend to modify that.
void webots_physics_predraw()
{
}
