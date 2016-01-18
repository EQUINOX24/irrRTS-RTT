#include <Irrlicht.h>
#include <iostream>
#include "irrRTSAction.h"

using namespace irr;

int main()
{
	// Event Receiver // ======================================================
	MyEventReceiver receiver;

	// get Screen resolution // ===============================================
	//create a NULL device to detect screen resolution
		IrrlichtDevice *nulldevice = createDevice(video::EDT_NULL);

	core::dimension2d<u32> deskres = nulldevice->getVideoModeList()->getDesktopResolution();

	nulldevice->drop();
	// Create device // =======================================================

	int FullScreen = 1; //1366, 768 //1024, 600
	IrrlichtDevice *device;
	if (FullScreen == 1)
	{
		device = createDevice(video::EDT_OPENGL, deskres, 32,
		true, false, true, &receiver);
	}
	else
	{
		device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800, 600), 32,
		false, false, true, &receiver);
	}

	// Get a pointers  // ======================================================
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	// Image // ================================================================
	video::ITexture* irrlichtLogo = driver->getTexture("./media/irrlichtlogo2.png");

	// Terrain // ==============================================================
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("./media/heightmap3.jpg");
	if (terrain)
	{
		terrain->setScale(core::vector3df(2.0f, 1.0f, 2.0f));
		terrain->setPosition(core::vector3df(0, 0, 0));
		terrain->setMaterialFlag(video::EMF_LIGHTING, false);
		terrain->setMaterialTexture(0, driver->getTexture("./media/terrainTexture3.jpg"));
	}
	// Box // ==================================================================
	scene::ISceneNode* box = smgr->addCubeSceneNode();
	if (box)
	{
		box->setMaterialTexture(0, driver->getTexture("./media/redPixel.bmp"));
		box->setMaterialFlag(video::EMF_LIGHTING, false);
		box->setPosition(core::vector3df(512, terrain->getHeight(512,512) + 10, 512));
	}

	// FPS // ==================================================================
	//scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
	//camera->setPosition(camPos); camera->setTarget(camTar);

	device->getCursorControl()->setVisible(false);
	
	// Experiment area // ======================================================
	//__int8 AB = 2;
	//int JJ[4] = { 1, 3, 5, 6 };
	//std::cout << JJ[AB] << std::endl;

	// loop variables // =======================================================
	int lastFPS = -1;
	int fps;
	core::stringw str;

	u32 now;
	f32 tempT;
	f32 deltaTime;
	// Frame rate independence // ==============================================
	u32 then = device->getTimer()->getTime();
	// My camera // ============================================================
	Action Act(&receiver, device, terrain, &deltaTime);
	gui::IGUIStaticText* guiText = guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		core::rect<s32>(10, 200, 100, 222), true);
	while (device->run())
	{
		// Frame rate independence // ==========================================
		now = device->getTimer()->getTime();
		tempT = (f32)(now - then);
		deltaTime = (f32)(((tempT) < 60) ? tempT : 60);
		then = now;

		// =====================================================================
		Act.update();

		// Draw Scene // =======================================================
		driver->beginScene(true, true, video::SColor(255, 100, 101, 140));
		smgr->drawAll();
		guienv->drawAll();

		driver->draw2DImage(irrlichtLogo, core::rect<s32>(10, 10, 138, 138), core::rect<s32>(0,
			0, 128, 128), 0, 0, true);

		Act.drawCursor();
		driver->endScene();
		// Type FPS //==========================================================
		fps = driver->getFPS();
		if (lastFPS != fps)
		{
			str = L"[";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			guiText->setText(str.c_str());
			lastFPS = fps;
		}
	}

	// Drop Device //===========================================================
	device->drop();
	return 0;
}