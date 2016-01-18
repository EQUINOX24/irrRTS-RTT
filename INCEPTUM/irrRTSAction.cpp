#include <irrRTSAction.h>
#include <iostream>
#include <cmath>

using namespace irr;

// MyEventReceiver //=============================================================
MyEventReceiver::MyEventReceiver()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

bool MyEventReceiver::OnEvent(const SEvent& event)
{
	if (event.EventType == EET_MOUSE_INPUT_EVENT)
	{
		if (event.MouseInput.isRightPressed())
			MouseRDown = true;
		else
			MouseRDown = false;

		if (event.MouseInput.Event == EMIE_MOUSE_WHEEL)
			std::cout << " wheeled " << std::endl;
		
		if (MouseRDown)
		{
			cursorPosCurrent = core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y);
		}
	}

	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	return false;
}

bool MyEventReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}

bool MyEventReceiver::MouseR() const
{
	return MouseRDown;
}

core::position2d<s32> MyEventReceiver::getCursorPos() const
{
	return cursorPosCurrent;
}

// Action // =====================================================================
Action::Action(MyEventReceiver* const receiver, IrrlichtDevice* const device,
	scene::ITerrainSceneNode* const terrain, f32* const deltaTime)
{
	this->device = device;
	this->driver = device->getVideoDriver();
	this->smgr = device->getSceneManager();
	this->receiver = receiver;
	this->deltaTime = deltaTime;
	// ===========================================================================
	this->terrain = terrain;
	this->camHeight = 300;
	this->camSpeed = 0.6f;
	this->rotSpeed = 1.0f / 300.0f;

	core::vector3df camPos = core::vector3df(360, 128, 360);
	core::vector3df camTar = core::vector3df(511, 0, 512);
	camPos.Y = camHeight + terrain->getHeight(camPos.X, camPos.Z);
	camTar.Y = terrain->getHeight(camPos.X, camPos.Z);
	this->camera = smgr->addCameraSceneNode(0, camPos, camTar);

	direction.X = camera->getTarget().X - camera->getPosition().X;
	direction.Y = camera->getTarget().Z - camera->getPosition().Z;
	direction.normalize();
	recipsqrt2 = core::reciprocal_squareroot((f32)2);

	// ==========================================================================
	this->cursor = driver->getTexture("./media/cursor.png");
}

void Action::update()
{
	camTar.Y = 3;
	n = (receiver->IsKeyDown(KEY_KEY_W) << 0) | (receiver->IsKeyDown(KEY_KEY_A) << 1)
		| receiver->IsKeyDown(KEY_KEY_S) << 2 | receiver->IsKeyDown(KEY_KEY_D) << 3;

	// =========================================================================
	camPos = camera->getPosition();
	camTar = camera->getTarget();

	if (n != 0)
	{
		switch (n)
		{
		case 1:
			Xup = (f32)camSpeed * *deltaTime * direction.X;
			Yup = (f32)camSpeed * *deltaTime * direction.Y;
			break;
		case 2:
			Xup = (f32)-camSpeed * *deltaTime*direction.Y;
			Yup = (f32)camSpeed * *deltaTime*direction.X;
			break;
		case 8:
			Xup = (f32)camSpeed * *deltaTime*direction.Y;
			Yup = (f32)-camSpeed * *deltaTime*direction.X;
			break;
		case 4:
			Xup = (f32)-camSpeed * *deltaTime * direction.X;
			Yup = (f32)-camSpeed * *deltaTime * direction.Y;
			break;
		case 3:
			Xup = (f32)camSpeed * *deltaTime * (-direction.Y + direction.X) * recipsqrt2;
			Yup = (f32)camSpeed * *deltaTime * (direction.X + direction.Y) * recipsqrt2;
			break;
		case 9:
			Xup = (f32)camSpeed * *deltaTime * (direction.Y + direction.X) * recipsqrt2;
			Yup = (f32)camSpeed * *deltaTime * (-direction.X + direction.Y) * recipsqrt2;
			break;
		case 6:
			Xup = (f32)-camSpeed * *deltaTime * (direction.Y + direction.X) * recipsqrt2;
			Yup = (f32)camSpeed * *deltaTime * (direction.X - direction.Y) * recipsqrt2;
			break;
		case 12:
			Xup = camSpeed * *deltaTime * (direction.Y - direction.X) * recipsqrt2;
			Yup = -camSpeed * *deltaTime * (direction.X + direction.Y) * recipsqrt2;
			break;
		}
		if (camPos.X < 200)
		{
			if (Xup > 0)
			{
				camPos.X += Xup; camTar.X += Xup;
			}
		}
		else if (camPos.X > 1848)
		{
			if (Xup < 0)
			{
				camPos.X += Xup; camTar.X += Xup;
			}
		}
		else
		{
			camPos.X += Xup; camTar.X += Xup;
		}

		if (camPos.Z < 200)
		{
			if (Yup > 0)
			{
				camPos.Z += Yup; camTar.Z += Yup;
			}
		}
		else if (camPos.Z > 1848)
		{
			if (Yup < 0)
			{
				camPos.Z += Yup; camTar.Z += Yup;
			}
		}
		else
		{
			camPos.Z += Yup; camTar.Z += Yup;
		}
	}
	// =========================================================================
	currentHight = terrain->getHeight(camPos.X, camPos.Z);
	camPos.Y = 0.9f*camPos.Y + 0.1f*(currentHight + camHeight);
	camTar.Y = 0.9f*camTar.Y + 0.1f*currentHight;

	camera->setPosition(camPos);
	camera->setTarget(camTar);

	if (receiver->MouseR())
	{
		if (!rotationMode)
		{
			rotationMode = true;
			cursorPosSaved = device->getCursorControl()->getPosition();
		}
		deltaX = cursorPosSaved.X - receiver->getCursorPos().X;
		deltaY = cursorPosSaved.Y - receiver->getCursorPos().Y;
		

		deltaX = (deltaX < 60) ? deltaX : 60; deltaX = (deltaX > -60) ? deltaX : -60;

		cosA = cos(deltaX*rotSpeed);
		sinA = sin((f32)deltaX*rotSpeed);


		tarTemp.X = camTar.X - camPos.X;
		tarTemp.Y = camTar.Z - camPos.Z;

		camTar.X = cosA*tarTemp.X - sinA*tarTemp.Y + camPos.X;
		camTar.Z = cosA*tarTemp.Y + sinA*tarTemp.X + camPos.Z;
		camera->setTarget(camTar);
		direction.X = camera->getTarget().X - camera->getPosition().X;
		direction.Y = camera->getTarget().Z - camera->getPosition().Z;
		direction.normalize();
	}
	else
	{
		if (rotationMode)
		{
			rotationMode = false;
		}
		
	}
}

void Action::drawCursor()
{
	if (rotationMode)
	{
		device->getCursorControl()->setPosition(cursorPosSaved);
	}
	else
	{
		cursorPosCurrent = device->getCursorControl()->getPosition();
		driver->draw2DImage(cursor, core::rect<s32>(
			cursorPosCurrent.X, cursorPosCurrent.Y,
			cursorPosCurrent.X + 40, cursorPosCurrent.Y + 40),
			core::rect<s32>(0, 0, 64, 64), 0, 0, true);
	}
}