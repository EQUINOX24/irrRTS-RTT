#pragma once
#include <Irrlicht.h>

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
protected:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool MouseDown[2];
	bool wheel[2];
public:
	MyEventReceiver();
	virtual bool OnEvent(const SEvent& event);
	virtual bool IsKeyDown(EKEY_CODE keyCode) const;
	virtual bool isMouseLDown() const;
	virtual bool isMouseRDown() const;
	virtual bool wheelUp() const;
	virtual bool getWheelState() const;
	virtual void setWheelState(bool wheel1);
};

// ===========================================================================
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
	core::position2d<s32> cursorPosCurrent, cursorPosSaved, screenCenter, screenEdges;
	s32 deltaX, deltaY;

	// ========================================================
	f32 cosA, sinA;
	f32 rotSpeed, currentAngle, tempAngle, aSinA, bSinA;
	f32 cosASq, aSinASq, bSinASq, bSinATarTemp2X, cosATarTemp2Y, aSinATarTemp2Z;

	// ========================================================
	scene::ICameraSceneNode* camera;
	scene::ITerrainSceneNode* terrain;
	f32 camSpeed, camHeight, camRadius, recipsqrt2, tarHeight;

	// =========================================================
	bool rotationMode = false;
	__int8 n;
	core::vector2d<f32> direction; // of camera facing (camTar - camPos).Norm (normalized)
	core::vector3df camPos, camTar1, camTar2; // camera position and camera target
	f32 currentHight, Xup, Yup; // X and Y updates for translation
	// =========================================================
public:
	Action(MyEventReceiver* const receiver, IrrlichtDevice* const device,
		scene::ITerrainSceneNode* const terrain, f32* const deltaTime);
	void update();
	void drawCursor();
	//void cameraRotationAndCursor();
};