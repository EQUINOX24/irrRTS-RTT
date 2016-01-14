#include <Action.h>
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
		if (event.MouseInput.isRightPressed())
			MouseRDown = true;
		else
			MouseRDown = false;
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_MOUSE_WHEEL)
		std::cout << " wheeled lol " << std::endl;
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


// CameraAction // ===============================================================
CameraAction::CameraAction(MyEventReceiver* receiver, scene::ICameraSceneNode* camera, scene::ITerrainSceneNode* terrain)
{
	this->receiver = receiver;
	this->camera = camera;
	this->terrain = terrain;
	direction.X = camera->getTarget().X - camera->getPosition().X;
	direction.Y = camera->getTarget().Z - camera->getPosition().Z;
	direction.normalize();
	speed = (f32) 0.4;
	camHeight = 160;
	recipsqrt2 = core::reciprocal_squareroot((f32)2);
}

void CameraAction::update(const f32* const deltaTime)
{
	this->camTar.Y = 3;
	n = (receiver->IsKeyDown(KEY_KEY_W) << 0) | (receiver->IsKeyDown(KEY_KEY_A) << 1)
		| receiver->IsKeyDown(KEY_KEY_S) << 2 | receiver->IsKeyDown(KEY_KEY_D) << 3;

	// =========================================================================
	camPos = camera->getPosition();
	camTar = camera->getTarget();

	//std::cout << camPos.X << ", " << camPos.Z << std::endl;
	// =========================================================================
	if (n != 0)
	{
		switch (n)
		{
		case 1:
			Xup = (f32)speed * *deltaTime * direction.X;
			Yup = (f32)speed * *deltaTime * direction.Y;
			break;
		case 2:
			Xup = (f32)-speed * *deltaTime*direction.Y;
			Yup = (f32)speed * *deltaTime*direction.X;
			break;
		case 8:
			Xup = (f32)speed * *deltaTime*direction.Y;
			Yup = (f32)-speed * *deltaTime*direction.X;
			break;
		case 4:
			Xup = (f32)-speed * *deltaTime * direction.X;
			Yup = (f32)-speed * *deltaTime * direction.Y;
			break;
		case 3:
			Xup = (f32)speed * *deltaTime * (-direction.Y + direction.X) * recipsqrt2;
			Yup = (f32)speed * *deltaTime * (direction.X + direction.Y) * recipsqrt2;
			break;
		case 9:
			Xup = (f32)speed * *deltaTime * (direction.Y + direction.X) * recipsqrt2;
			Yup = (f32)speed * *deltaTime * (-direction.X + direction.Y) * recipsqrt2;
			break;
		case 6:
			Xup = (f32)-speed * *deltaTime * (direction.Y + direction.X) * recipsqrt2;
			Yup = (f32)speed * *deltaTime * (direction.X - direction.Y) * recipsqrt2;
			break;
		case 12:
			Xup = speed * *deltaTime * (direction.Y - direction.X) * recipsqrt2;
			Yup = -speed * *deltaTime * (direction.X + direction.Y) * recipsqrt2;
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
}
