#pragma once
#include <Irrlicht.h>

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
protected:
	// This array is used to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool MouseRDown;
	core::position2d<s32> cursorPosCurrent;
public:
	MyEventReceiver();
	virtual bool OnEvent(const SEvent& event);
	virtual bool IsKeyDown(EKEY_CODE keyCode) const;
	virtual bool MouseR() const;
	virtual core::position2d<s32> getCursorPos() const;
};

class Action
{
protected:
	f32* deltaTime;
	IrrlichtDevice *device;
	scene::ISceneManager* smgr;
	video::IVideoDriver* driver;
	MyEventReceiver* receiver;
	// ========================================================
	video::ITexture* cursor;
	core::position2d<s32> cursorPosCurrent, cursorPosSaved;
	s32 deltaX, deltaY;

	// ========================================================
	f32 cosA, sinA;
	core::vector2d<f32> tarTemp;
	f32 rotSpeed; // rotation speed

	// ========================================================
	scene::ICameraSceneNode* camera;
	scene::ITerrainSceneNode* terrain;
	f32 camSpeed, camHeight, recipsqrt2;

	// =========================================================
	bool rotationMode = false;;
	__int8 n;
	core::vector2d<f32> direction; // of camera facing (camTar - camPos).Norm (normalized)
	core::vector3df camPos, camTar; // camera position and camera target
	f32 currentHight, Xup, Yup; // X and Y updates for translation
	// =========================================================
public:
	Action(MyEventReceiver* const receiver, IrrlichtDevice* const device,
		scene::ITerrainSceneNode* const terrain, f32* const deltaTime);
	void update();
	void drawCursor();
	//void cameraRotationAndCursor();
};