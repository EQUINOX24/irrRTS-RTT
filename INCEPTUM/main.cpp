#include <Irrlicht.h>
#include "Action.h"

using namespace irr;

int main()
{
	// Event Receiver // =======================================================
	MyEventReceiver receiver;

	// Create device // =======================================================

	IrrlichtDevice *device =
		createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800, 600), 32,//1366, 768 //1024, 600
		false, false, true, &receiver);
	if (!device)
		return 1; // Could not create device. //

	// Get pointers  // ========================================================
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	// Cursor // ===============================================================
	video::ITexture* cursor = driver->getTexture("./media/cursor.png");
	core::position2d<s32> cursorPos;
	// Image // ================================================================
	video::ITexture* irrlichtLogo = driver->getTexture("./media/irrlichtlogo2.png");

	// Terrain // ==============================================================
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("./media/heightmap3.jpg");
	if (terrain)
	{
		terrain->setScale(core::vector3df(2.0f, 1.0f, 2.0f));
		terrain->setPosition(core::vector3df(0, 0, 0));
		terrain->setMaterialFlag(video::EMF_LIGHTING, false);
		terrain->setMaterialTexture(0, driver->getTexture("./media/terrainTexture2.jpg"));
	}
	// Box // ==================================================================
	scene::ISceneNode* box = smgr->addCubeSceneNode();
	if (box)
	{
		box->setMaterialTexture(0, driver->getTexture("./media/redPixel.bmp"));
		box->setMaterialFlag(video::EMF_LIGHTING, false);
		box->setPosition(core::vector3df(512, terrain->getHeight(512,512) + 10, 512));
	}

	// Camera init coord // ====================================================
	core::vector3df camPos = core::vector3df(360, 128, 360);
	core::vector3df camTar = core::vector3df(511, 0, 512);

	// FPS // ==================================================================
	//scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	//camera->setPosition(camPos); camera->setTarget(camTar);

	// My camera // ============================================================
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(0, camPos, camTar);
	CameraAction cameraAction(&receiver, camera, terrain);
	device->getCursorControl()->setVisible(false);
	//f32 camHeight = 128;
	//camPos = core::vector3df(camPos.X,camHeight + terrain->getHeight(camPos.X, camPos.Z),camPos.Z);
	//camera->setPosition(camPos);

	// Loop variables // =======================================================
	int lastFPS = -1;

	// Frame rate independence // ==============================================
	u32 then = device->getTimer()->getTime();
	while (device->run())
	{
		// Frame rate independence // ==========================================
		const u32 now = device->getTimer()->getTime();
		const f32 tempT = (f32)(now - then);
		const f32 deltaTime = (f32)(((tempT) < 60) ? tempT : 60);
		then = now;

		// =====================================================================
		cameraAction.update(&deltaTime);

		// Draw Scene // =======================================================
		driver->beginScene(true, true, video::SColor(255, 100, 101, 140));

		smgr->drawAll();

		driver->draw2DImage(irrlichtLogo, core::rect<s32>(10, 10, 138, 138), core::rect<s32>(0,
			0, 128, 128), 0, 0, true);

		if (!receiver.MouseR())
		{
			cursorPos = device->getCursorControl()->getPosition();
			driver->draw2DImage(cursor, core::rect<s32>(
				cursorPos.X, cursorPos.Y,
				cursorPos.X + 40, cursorPos.Y + 40),
				core::rect<s32>(0, 0, 64, 64), 0, 0, true);
		}
		
		driver->endScene();

		// Type FPS //==========================================================
		int fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Irrlicht Engine - Quake 3 Map example [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	// Drop Device //===========================================================
	device->drop();
	return 0;
}