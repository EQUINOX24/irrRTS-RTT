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

	core::dimension2d<u32> deskRes = nulldevice->getVideoModeList()->getDesktopResolution();

	nulldevice->drop();
	// Create device // =======================================================
	IrrlichtDevice *device; //1366, 768 //1024, 600
	int FullScreen = 1;
	//std::cout << "FullScreen = ";
	//std::cin >> FullScreen;
	if (FullScreen == 1)
	{
		device = createDevice(video::EDT_DIRECT3D9, deskRes, 32,
		true, false, true, &receiver);
	}
	else
	{
		device = createDevice(video::EDT_DIRECT3D9, core::dimension2d<u32>(1280, 720), 32,
		false, false, true, &receiver);
	}

	// Get a pointers  // ======================================================
	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	// Disable mip maps // =====================================================
	//driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	// Image // ================================================================
	video::ITexture* controlsPic = driver->getTexture("./media/Controls.png");
	video::ITexture* irrlichtLogo = driver->getTexture("./media/irrlichtlogo2.png");

	// Terrain // ==============================================================
	scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("./media/heightmap3.jpg");
	if (terrain)
	{
		terrain->setScale(core::vector3df(2.0f, 1.0f, 2.0f));
		terrain->setPosition(core::vector3df(0, 0, 0));
		terrain->setMaterialFlag(video::EMF_LIGHTING, false);
		terrain->setMaterialTexture(0, driver->getTexture("./media/terrainTexture2.bmp"));
	}
	// Box // ==================================================================
	scene::ISceneNode* box = smgr->addCubeSceneNode();
	box->setMaterialTexture(0, driver->getTexture("./media/redPixel.bmp"));
	box->setMaterialFlag(video::EMF_LIGHTING, false);
	box->setPosition(core::vector3df(812, terrain->getHeight(812, 812) + 0.5f, 812));
	//box->setScale(core::vector3df(1, 2, 1));
	// Tank // =================================================================
	scene::IAnimatedMesh* mesh = smgr->getMesh("./media/daushleli.3DS");
	scene::IAnimatedMeshSceneNode* tank = smgr->addAnimatedMeshSceneNode(mesh);
	tank->setPosition(core::vector3df(812, terrain->getHeight(812, 812), 812));
	tank->setScale(core::vector3df(0.8f, 0.8f, 0.8f));
	tank->setRotation(core::vector3df(0.0f, 90.0f, 0.0f));
	tank->setMaterialFlag(video::EMF_LIGHTING, false);
	tank->setMD2Animation(scene::EMAT_STAND);
	tank->setMaterialTexture(0, driver->getTexture("./media/textureTest.jpg"));
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
	//int fps;
	core::stringw str;

	u32 now;
	f32 tempT;
	f32 deltaTime;
	//core::vector3df tankRotation = tank->getRotation();
	// Frame rate independence // ==============================================
	u32 then = device->getTimer()->getTime();
	// My camera // ============================================================
	Action Act(&receiver, device, terrain, &deltaTime);
	//gui::IGUIStaticText* guiText = guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
	//	core::rect<s32>(10, 200, 100, 222), true);
	while (device->run())
	{
		if (device->isWindowActive())
		{
			// Frame rate independence // ==========================================
			now = device->getTimer()->getTime();
			tempT = (f32)(now - then);
			deltaTime = (f32)(((tempT) < 60) ? tempT : 60);
			then = now;

			// =====================================================================
			Act.update();
			//tankRotation.Y += deltaTime*0.1f;
			//tank->setRotation(tankRotation);
			// Draw Scene // =======================================================
			driver->beginScene(true, true, video::SColor(255, 100, 101, 140));
			smgr->drawAll();
			guienv->drawAll();

			driver->draw2DImage(controlsPic, core::rect<s32>(10, 10, 200, 200), core::rect<s32>(0,
				0, 256, 256), 0, 0, true);
			driver->draw2DImage(irrlichtLogo, core::rect<s32>(1100, 10, 1250, 150), core::rect<s32>(0,
				0, 128, 128), 0, 0, true);

			Act.drawCursor();
			driver->endScene();
			// Type FPS //==========================================================
			/*fps = driver->getFPS();
			if (lastFPS != fps)
			{
				str = L"[";
				str += driver->getName();
				str += "] FPS:";
				str += fps;

				guiText->setText(str.c_str());
				lastFPS = fps;
			}*/
		}
		else
			device->yield();
	}

	// Drop Device //===========================================================
	device->drop();
	return 0;
}