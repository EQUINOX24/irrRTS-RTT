#pragma once
#include <Irrlicht.h>

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
protected:
	// This array is used to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool MouseRDown;
public:
	MyEventReceiver();
	virtual bool OnEvent(const SEvent& event);
	virtual bool IsKeyDown(EKEY_CODE keyCode) const;
	virtual bool MouseR() const;
};

class CameraAction
{
protected:
	MyEventReceiver* receiver;
	scene::ICameraSceneNode* camera;
	scene::ITerrainSceneNode* terrain;
	f32 speed;
	f32 recipsqrt2;
	bool tempCol; // collision

	f32 camHeight;

	__int8 n;
	core::vector2d<f32> direction;
	core::vector3df camPos;
	core::vector3df camTar;
	f32 Xup; // Xupdate
	f32 Yup; // Yupdate
	f32 currentHight;
public:
	CameraAction(MyEventReceiver* receiver, scene::ICameraSceneNode* camera, scene::ITerrainSceneNode* terrain);
	void update(const f32* const deltaTime);
};