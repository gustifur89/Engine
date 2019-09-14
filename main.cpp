#include "Headers.h"
#include "UIManager.h"
#include "Mesh.h"
#include "GameObject.h"
#include "CollisionStructure.h"
#include "Entity.h"
#include "Portal.h"
#include "Projectile.h"
#include "Item.h"
#include "Terrain.h"
#include "Audio.h"

int main()
{
	UIManager UI;
	//1024 768   :   800 600
	//"fog"  "sobel_color"
	if (!UI.create(900, 650, "peeps out here", 120, "windowFragment2"))
	{
		std::cout << "failed to create UI\n";
		return -1;
	}
	UI.setClearColor(140, 180, 240);	//blue (sky)
//	UI.setClearColor(160, 116, 3);		//yellow
//	UI.setClearColor(16, 6, 20);		//purple (void)

	Audio::init();

	std::shared_ptr<GameObject> stage = UI.stage;
	std::shared_ptr<GameObject> world1 = std::shared_ptr<GameObject>(new GameObject());
	stage->addChild(world1);
	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Holdable>> items;

	std::shared_ptr<ColorShader> shader0 = ColorShader::loadShader("color");
	std::shared_ptr<ColorShader> flatShader = ColorShader::loadShader("flat");
	std::shared_ptr<ColorShader> shaderPortal = ColorShader::loadShader("portal");
	std::shared_ptr<TextureShader> texShader = TextureShader::loadShader("texture");

	std::shared_ptr<ColorMesh> mesh0 = ColorMesh::loadFromFile("DefaultCube");
	std::shared_ptr<ColorMesh> mesh1 = ColorMesh::loadFromFile("DefaultSphere");
	std::shared_ptr<ColorMesh> cloud = ColorMesh::loadFromFile("SmoothSphere");
	std::shared_ptr<ColorMesh> robot = ColorMesh::loadFromFile("robot");
	std::shared_ptr<ColorMesh> mesh3 = ColorMesh::loadFromFile("land");
	std::shared_ptr<ColorMesh> mesh4 = ColorMesh::loadFromFile("roomVer");
	std::shared_ptr<ColorMesh> meshCrystal = ColorMesh::loadFromFile("crystal");//crystal
	std::shared_ptr<ColorMesh> pillarMesh = ColorMesh::loadFromFile("pillar");
	std::shared_ptr<ColorMesh> portalGun = ColorMesh::loadFromFile("portalGun");
	std::shared_ptr<ColorMesh> portalMesh = ColorMesh::loadFromFile("portalMesh2");//portalMesh3  planar_portal  portalMesh2
	std::shared_ptr<TextureMesh> texCube = TextureMesh::loadFromFile("textureCube");//texturePlane textureCube

	std::shared_ptr<Texture> texture0 = Texture::loadFromFile("backrooms_wall.jpg");//basic  blank

	std::vector<std::shared_ptr<Portal>> portals;

	std::shared_ptr<Sound> sound0 = Sound::loadFromFile("buzz");
	std::shared_ptr<SoundEmitter> soundEmmiter0 = std::shared_ptr<SoundEmitter>(new SoundEmitter(sound0));
	soundEmmiter0->transform.x = 0;
	soundEmmiter0->transform.y = 0;
	soundEmmiter0->transform.z = 0;
	soundEmmiter0->setAttenuation(0.5);
	soundEmmiter0->setMinDistance(15.0);
	soundEmmiter0->setLoop(true);
	//soundEmmiter0->play();

//
//	sf::SoundBuffer buffer;
//	if (!buffer.loadFromFile("src/sounds/lightBuzz.ogg"))
//		return -1;
//	sf::Sound sound;
//	sound.setVolume(50);
//	sound.setBuffer(buffer);
//	sound.play();

	
//	sf::Listener::setPosition(0,0,0);

	/*
	std::shared_ptr<GameObjectTexture> texObj(new GameObjectTexture());
	texObj->mesh = texCube;
	texObj->shader = texShader;
	texObj->texture = texture0;
	world1->addChild(texObj);
	*/

	/*
	std::shared_ptr<Portal> portal1(new Portal);
	portal1->mesh = portalMesh;
	portal1->shader = shaderPortal;
	portal1->setWorld(world1);
	portal1->setRadius(2.0);
	portal1->transform.x = 0;
	portal1->transform.y = 1.5;
	portal1->transform.z = 0;
	portal1->setFillColor(255, 158, 44);
	stage->addChild(portal1);
	portals.push_back(portal1);

	std::shared_ptr<Portal> portal2(new Portal);
	portal2->mesh = portalMesh;
	portal2->shader = shaderPortal;
	portal2->transform.x = -26;
	portal2->transform.y = 1.5;
	portal2->transform.z = 0;
	portal2->setFillColor(189,255,142);
	portal2->setRadius(2.0);
	portal2->setWorld(world1);
	stage->addChild(portal2);
	portals.push_back(portal2);
	*/
	/*
	std::shared_ptr<Portal> portal3(new Portal);
	portal3->mesh = portalMesh;
	portal3->shader = shaderPortal;
	portal3->setWorld(world1);
	portal3->setRadius(1.0);
	portal3->transform.x = 0;
	portal3->transform.y = 18;
	portal3->transform.z = 0;
	stage->addChild(portal3);
	portals.push_back(portal3);

	std::shared_ptr<Portal> portal4(new Portal);
	portal4->mesh = portalMesh;
	portal4->shader = shaderPortal;
	portal4->transform.x = -19;
	portal4->transform.y = 18;
	portal4->transform.z = 0;
	portal4->setRadius(1.0);
	portal4->setWorld(world1);
	stage->addChild(portal4);
	portals.push_back(portal4);
	*/
	//Portal::linkPortals(portal3, portal4);
	//Portal::linkPortals(portal1, portal2);
	
	std::shared_ptr<GameObjectColor> ground(new GameObjectColor);
	ground->mesh = mesh4;
	ground->transform.y = 0;
	ground->shader = shader0;
	world1->addChild(ground);

	/*
	for (int i = 0; i < 30; i++)
	{
		std::shared_ptr<GameObjectColor> obj(new GameObjectColor);
		obj->transform.x = rand() % 60 - 30;// +i * 6;
		obj->transform.y = rand() % 60 - 30;
		obj->transform.z = rand() % 60 - 30;
		obj->transform.angleX = (rand() % 360);
		obj->transform.angleY = (rand() % 360);
		obj->transform.angleZ = (rand() % 360);
	//	obj->transform.scaleX = 3.0;
	//	obj->transform.scaleY = 1.0;
	//	obj->transform.scaleZ = 3.0;
		obj->mesh = mesh0;
		obj->shader = shader0;
		obj->setFillColor(rand() % 255, rand() % 255, rand() % 255);

		//obj->setFillColor(0, 255, 255);

		world1->addChild(obj);
	}
	
	for (int i = 0; i < 45; i++)
	{
		std::shared_ptr<GameObjectColor> obj(new GameObjectColor);
		obj->transform.x = rand() % 80 - 40;
		obj->transform.y = rand() % 80 - 40;
		obj->transform.z = rand() % 80 - 40;

		obj->transform.angleX = (rand() % 360);
		obj->transform.angleY = (rand() % 360);
		obj->transform.angleZ = (rand() % 360);

		obj->mesh = mesh1;
		obj->shader = shader0;
		obj->setFillColor(rand() % 255, rand() % 255, rand() % 255);

		//obj->setFillColor(0, 255, 255);

		world1->addChild(obj);
	}
	*/
	/*
	std::shared_ptr<GameObjectColor> obj2(new GameObjectColor);
	obj2->transform.angleX = (45);
	obj2->transform.angleY = (45);
	obj2->transform.angleZ = (45);
	obj2->transform.x = 12;
	obj2->transform.y = 15;
	obj2->transform.z = 0;
	obj2->mesh = mesh2;
	obj2->shader = shader0;
	obj2->setFillColor(255, 255, 255);
	stage->addChild(obj2);
	*/
	/*
	std::shared_ptr<ColorMesh> wolrdMesh = OctTree::combineMeshes(stage, 255, 255, 0);
	std::shared_ptr<GameObjectColor> obj3(new GameObjectColor);
	obj3->transform.x = 0.5;
	obj3->transform.y = 0.5;
	obj3->transform.z = 0.5;
	obj3->mesh = wolrdMesh;
	obj3->shader = shader0;
	stage->addChild(obj3);
	//	*/


	std::shared_ptr<GameObjectColor> objK(new GameObjectColor);
	objK->transform.x = 4;
	objK->transform.y = 8;
	objK->transform.z = 8;
	objK->transform.angleY = (rand() % 360);
	objK->mesh = pillarMesh;
	objK->shader = shader0;
	int greyK = rand() % 60 + 60;
	objK->setFillColor(greyK, greyK, greyK);
	world1->addChild(objK);

	std::shared_ptr<GameObjectColor> objL(new GameObjectColor);
	objL->transform.x = 6;
	objL->transform.y = 8;
	objL->transform.z = 4;
	objL->transform.angleY = (rand() % 360);
	objL->mesh = pillarMesh;
	objL->shader = shader0;
	objL->setFillColor(greyK, greyK, greyK);
	world1->addChild(objL);

	//crystals
	for (int i = 0; i < 30; i++)
	{
		std::shared_ptr<GameObjectColor> obj(new GameObjectColor);
		obj->transform.x = rand() % 120 - 60;
		obj->transform.y = rand() % 60 - 30;// +20;
		obj->transform.z = rand() % 120 - 60;
		obj->transform.angleY = (rand() % 360);
		double scale = ((rand() % 1000) / 1000.0) * 1.0 + 0.6;
		obj->transform.scaleY = ((rand() % 1000) / 1000.0) * 1.0 + 1.0;
		
		obj->transform.scaleX *= scale;
		obj->transform.scaleY *= scale;
		obj->transform.scaleZ *= scale;
		
		obj->mesh = pillarMesh;
		obj->shader = shader0;

		//int purple = rand() % 120 + 60;
		//int r = rand() % 24;
		//int g = rand() % (purple*2/3);
		//int b = rand() % 30;
		//obj->setFillColor(purple + r, g, purple + b);

		int grey = rand() % 60 + 60;
		obj->setFillColor(grey,grey,grey);


		world1->addChild(obj);
	}

	//clouds
	for (int i = 0; i < 10; i++)
	{
		int cloudNum = rand() % 3 + 3;

		double x = rand() % 120 - 60;
		double y = rand() % 20 + 46;
		double z = rand() % 120 - 60;
		double angle = (rand() % 360);
		int color = rand() % 100 + 140;

		for (int i = 0; i < cloudNum; i++)
		{
			std::shared_ptr<GameObjectColor> obj(new GameObjectColor);
			obj->transform.x = x + rand() % 4 - 2;
			obj->transform.y = y + rand() % 2 - 1;
			obj->transform.z = z + rand() % 4 - 2;
			obj->transform.angleY = angle;

			double scale = ((rand() % 1000) / 1000.0) * 1.6 + 1.0;

			obj->transform.scaleX = scale;// ((rand() % 1000) / 1000.0) * 1.2 + 1.0;
			obj->transform.scaleY = scale*0.8;// ((rand() % 1000) / 1000.0) * 1.2 + 1.0;
			obj->transform.scaleZ = scale;// ((rand() % 1000) / 1000.0) * 1.2 + 1.0;
			obj->collidable = false;
			obj->mesh = cloud;
			obj->shader = flatShader;
			obj->setFillColor(color, color, color);
			world1->addChild(obj);
		}
	}

	Camera camera(90, UI.aspectRatio, 0.1f, 100.0f);

	bool lockToggle = true;
	bool toggleA = true;
	
	bool shootToggle = true;

//	OctTree tree;
//	tree.build(world1, 6, 2);

	//16, 16, 100
	std::shared_ptr<Terrain> land(new Terrain(1024, glm::vec3(15,40,15), 20, 1.0, 2.0, glm::vec3(1.0, 1.0, 1.0), 0.0, 2));
	land->shader = shader0;
	world1->addChild(land);	
	land->addNew(glm::vec2(0, 0));
	//land->generateTerrain();

	std::shared_ptr<OctTree> tree(new OctTree());
	std::shared_ptr<OctTree> noCollider(NULL);
	tree->build(world1, 6, 2);


	std::shared_ptr<Player> player(new Player(0.4, std::shared_ptr<Mesh>(NULL), &UI));
	player->setJump(10.0);
	player->setJumpTime(0.5);
	player->mesh = cloud;
	player->transform.scaleX = 1.2;
	player->transform.scaleY = 1.2;
	player->transform.scaleZ = 1.2;
	player->shader = shader0;
	world1->addChild(player);
	entities.push_back(player);
	double ballSpeed = 100;
	land->setTarget(player);

	/*
	std::shared_ptr<GameObjectColor> objK(new GameObjectColor);
	objK->transform.x = 8;
	objK->transform.y = 6;
	objK->transform.z = 4;
	objK->transform.angleY = (rand() % 360);
	objK->mesh = pillarMesh;
	objK->shader = shader0;
	int greyK = rand() % 60 + 60;
	objK->setFillColor(greyK, greyK, greyK);
	world1->addChild(objK);*/


	//HUD
	std::shared_ptr<Gun> gun(new Gun(0.05, portalGun, &UI));
	gun->relative.x = 0;
	gun->relative.z = 0.5;
	gun->relative.y = -0.5;
	gun->relative.angleY = 0;
	gun->relative.angleX = -10;
	double gunScale = 0.1;
	gun->transform.scaleX = gunScale;
	gun->transform.scaleY = gunScale;
	gun->transform.scaleZ = gunScale;
	gun->mesh = portalGun;
	gun->shader = shader0;
	world1->addChild(gun);
	items.push_back(gun);

	//LIGHTING

	std::shared_ptr<Light> light0 = Light::createPointLight(glm::vec3(0, 20, 0), 60.0, 50.0, glm::vec3(255,255,255));//Light::createSpotLight(glm::vec3(0,15,0), glm::vec3(0, 0, 0), 30.0, 20.0);
	UI.addLight(light0);

	std::cout << light0->y << "\n";

	glm::vec4 lightAxis(0, 1, 0, 0);

	double lightAngleX = 60;
	double lightAngleZ = 0;
	double lightAngleY = 120;
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(lightAngleX * TO_RAD, lightAngleY * TO_RAD, lightAngleZ * TO_RAD)));
	lightAxis = rotation * lightAxis;
	
	double shootDelay = 0.001;
	double shootRefill = 0.0;
	int numBalls = 5;

	bool updateToggle = false;
	bool updateToggleB = true;

	camera.gamma = 10.0;

	double lightPolarity = 1.0;

	do
	{
		if (UI.isKeyPressed(GLFW_KEY_2))
		{
			if (lockToggle)
			{
				if (UI.getMouseLockState())
					UI.unlockMouse();
				else
					UI.lockMouse();
			}
			lockToggle = false;
		}
		else
		{
			lockToggle = true;
		}

		if (UI.isKeyPressed(GLFW_KEY_0))
		{
			if (toggleA)
			{
				//soundEmmiter0->play(); ;//
				//world1->visible = !world1->visible;
			}
			toggleA = false;
		}
		else
		{
			toggleA = true;
		}

	//	portal2->transform.y -= 10 * UI.deltaTime;
	//	if (portal2->transform.y < 0) portal2->transform.y = 40;

		if (UI.isKeyPressed(GLFW_KEY_5))
		{
			if (updateToggleB)
			{
				updateToggle = !updateToggle;
			}
			updateToggleB = false;
		}
		else
		{
			updateToggleB = true;
		}

		//portal1->transform.angleY += 60 * UI.deltaTime;

		if (UI.isMouseDown(GLFW_MOUSE_BUTTON_1) && shootRefill > shootDelay)
		{
			//	if (shootToggle)
			//	{
					//spawn projectile
			for (int i = 0; i < numBalls; i++)
			{
				double radius = 0.2;
				glm::vec3 direction = Transform::getTransformedZ(player->lookRotation + glm::vec3(Gen::random() * 60, Gen::random() * 60, Gen::random() * 60));
				std::shared_ptr<Projectile> proj = Projectile::makeProjectile(radius, mesh1, &UI, glm::vec3(player->transform.x, player->transform.y, player->transform.z), direction, ballSpeed);
				proj->transform.scaleX = radius;
				proj->transform.scaleY = radius;
				proj->transform.scaleZ = radius;
				proj->setFillColor(rand() % 255, rand() % 255, rand() % 255);
				proj->shader = shader0;
				world1->addChild(proj);
				entities.push_back(proj);
			}
		//	}
			//shootToggle = false;
		}
		else
		{
			//shootToggle = true;
			shootRefill += UI.deltaTime;
		}

		for (int i = 0; i < Projectile::projectiles.size(); i++)
		{
			Projectile::projectiles[i]->move(noCollider);//land
		}
		
		player->move(noCollider);//land
		player->interact(items);

		Audio::configureListener(glm::vec3(player->transform.x, player->transform.y, player->transform.z), player->lookRotation);

		if(updateToggle)
			land->update();

	//	lightAngleY += UI.deltaTime * 30.0;
		//glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(lightAngleX * TO_RAD, lightAngleY * TO_RAD, lightAngleZ * TO_RAD)));
		//lightAxis = rotation * lightAxis;


		light0->x += lightPolarity * 2.0 * UI.deltaTime;
		if (light0->x >= 20.0) lightPolarity = -1.0;
		if (light0->x <= -20.0) lightPolarity = 1.0;

	//	light0->x = player->transform.x;
	//	light0->y = player->transform.y;
	//	light0->z = player->transform.z;

		shader0->setLight(lightAxis);

		if (UI.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
			player->updateCamera(camera, -0.1);
		else
			player->updateCamera(camera, 0.9);

		Holdable::move(items, noCollider);//land
		Portal::collide(portals, entities);
		Portal::preRenderPortals(portals, camera);
		Entity::entityBounds(entities);

		UI.display(camera);
	} while (UI.isWindowOpen());
//	land->closeThread();

	UI.delay(0.1);

	return 1;
}