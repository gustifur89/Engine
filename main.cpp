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
#include "Toggle.h"
#include "PhysicsBody.h"

int main()
{
	UIManager UI;
	//UI_SSAO
	if (!UI.create(900, 650, "nihilnauts", 120, UI_SOBEL | UI_SSAO, "windowFragment2"))
	{
		std::cout << "failed to create UI\n";
		return -1;
	}
	UI.setClearColor(140, 180, 240);

	Audio::init();

	std::shared_ptr<GameObject> stage = UI.stage;
	std::shared_ptr<GameObject> world1 = std::shared_ptr<GameObject>(new GameObject());
	std::shared_ptr<GameObject> world2 = std::shared_ptr<GameObject>(new GameObject());
	stage->addChild(world1);
	stage->addChild(world2);

	std::vector<std::shared_ptr<Entity>> entities;
	std::vector<std::shared_ptr<Holdable>> items;

	std::shared_ptr<ColorShader> colorShader = ColorShader::loadShader("color");
	std::shared_ptr<ColorShader> flatShader = ColorShader::loadShader("flat");
	std::shared_ptr<ColorShader> portalShader = ColorShader::loadShader("portal");
	Portal::setUpShader(portalShader);
	std::shared_ptr<TextureShader> texShader = TextureShader::loadShader("texture");

	std::shared_ptr<ColorMesh> cubeMesh = ColorMesh::loadFromFile("DefaultCube");
	std::shared_ptr<ColorMesh> ballMesh = ColorMesh::loadFromFile("DefaultSphere");
	std::shared_ptr<ColorMesh> monsterMesh = ColorMesh::loadFromFile("Mob");
	std::shared_ptr<ColorMesh> robotMesh = ColorMesh::loadFromFile("robot");
	std::shared_ptr<ColorMesh> landMesh = ColorMesh::loadFromFile("land");
	std::shared_ptr<ColorMesh> placeMesh = ColorMesh::loadFromFile("central_pillar");//central_pillar area_model area
	std::shared_ptr<ColorMesh> crystalMesh = ColorMesh::loadFromFile("crystal");//crystal
	std::shared_ptr<ColorMesh> pillarMesh = ColorMesh::loadFromFile("pillar");
	std::shared_ptr<ColorMesh> portalGunMesh = ColorMesh::loadFromFile("portalGun");
	std::shared_ptr<ColorMesh> portalMesh = ColorMesh::loadFromFile("portalMesh2");//portalMesh3  planar_portal  portalMesh2
	std::shared_ptr<ColorMesh> xwingMesh = ColorMesh::loadFromFile("x-wing");
	
	std::vector<std::shared_ptr<Portal>> portals;

	std::shared_ptr<Sound> sound0 = Sound::loadFromFile("buzz");
	std::shared_ptr<SoundEmitter> soundEmmiter0 = std::shared_ptr<SoundEmitter>(new SoundEmitter(sound0));
	soundEmmiter0->transform.position = glm::vec3(0.0);
	soundEmmiter0->setAttenuation(0.5);
	soundEmmiter0->setMinDistance(15.0);
	soundEmmiter0->setLoop(true);
	//soundEmmiter0->play();


//	/*
	std::shared_ptr<Portal> portal2(new Portal(UI.width, UI.height));
	portal2->mesh = portalMesh;
	portal2->shader = portalShader;
	portal2->transform.position = glm::vec3(-26, 6, 0);
	portal2->setFillColor(189, 255, 142);
	portal2->setRadius(2.0);
	portal2->setWorld(world1);
	stage->addChild(portal2);
	portals.push_back(portal2);

	std::shared_ptr<Portal> portal1(new Portal(UI.width, UI.height));
	portal1->mesh = portalMesh;
	portal1->shader = portalShader;
	portal1->transform.position = glm::vec3(-26, -3, 0);
	portal1->setFillColor(255, 158, 44);
	portal1->setWorld(world1);
	portal1->setRadius(2.0);
	stage->addChild(portal1);
	portals.push_back(portal1);

	/*
	std::shared_ptr<Portal> portal3(new Portal(UI.width, UI.height));
	portal3->mesh = portalMesh;
	portal3->shader = portalShader;
	portal3->setWorld(world1);
	portal3->setRadius(1.0);
	portal3->transform.position = glm::vec3(0, 0, -6);
	stage->addChild(portal3);
	portals.push_back(portal3);

	std::shared_ptr<Portal> portal4(new Portal(UI.width, UI.height));
	portal4->mesh = portalMesh;
	portal4->shader = portalShader;
	portal4->transform.position = glm::vec3(0, 20, -6);
	portal4->setRadius(1.0);
	portal4->setWorld(world1);
	stage->addChild(portal4);
	portals.push_back(portal4);
	*/

	Portal::linkPortals(portal1, portal2);
//	Portal::linkPortals(portal3, portal4);


	std::shared_ptr<GameObjectColor> xwing(new GameObjectColor);
	xwing->transform.position = glm::vec3(5, 8, 1);
	xwing->mesh = xwingMesh;
	xwing->shader = colorShader;
	world1->addChild(xwing);


	//*/
	/*
	std::shared_ptr<GameObjectColor> ground(new GameObjectColor);
	ground->mesh = placeMesh;
	ground->shader = colorShader;
	ground->transform.scale = glm::vec3(4, 4, 4);
	ground->persistentVisible = true;
	ground->transform.scale = glm::vec3(0.6, 0.6, 0.6);
	world1->addChild(ground);
	*/

	/*
	std::shared_ptr<GameObjectColor> monster(new GameObjectColor);
	monster->mesh = monsterMesh;
	monster->shader = colorShader;
	monster->transform.scale = glm::vec3(0.5, 0.5, 0.5);
	world1->addChild(monster);
	*/

	//crystals
	for (int i = 0; i < 10; i++)
	{
		std::shared_ptr<GameObjectColor> obj(new GameObjectColor);
		obj->transform.position = glm::vec3(rand() % 120 - 60, rand() % 60 - 30, rand() % 120 - 60);
		obj->transform.rotation.y = (rand() % 360);
		double scale = ((rand() % 1000) / 1000.0) * 1.0 + 0.6;
		obj->transform.scale.y = ((rand() % 1000) / 1000.0) * 1.0 + 1.0;

		obj->transform.scale *= scale;
		obj->mesh = pillarMesh;
		obj->shader = colorShader;

		int grey = rand() % 60 + 60;
		obj->setFillColor(grey, grey, grey);
		world1->addChild(obj);
	}
	
	Camera camera(90, UI.aspectRatio, 0.1f, 1000.0f);

	bool lockToggle = true;

	bool shootToggle = true;

	
	//std::shared_ptr<Terrain> land(new Terrain(1024, glm::vec3(30, 40, 30), 20, 1.0, 2.0, glm::vec3(1.0, 0.5, 1.0), 0.002, 2));
	std::shared_ptr<Terrain> land(new Terrain(1024, glm::vec3(40, 40, 40), 20, 0.5, 2.0, glm::vec3(1.0, 1.0, 1.0), 0.002, 2));
	land->shader = colorShader;
	land->persistentVisible = true;
	world1->addChild(land);
	
	//land->addNew(glm::vec2(0, 0));
	//land->generateTerrain();

	std::shared_ptr<Player> player(new Player(0.4, std::shared_ptr<Mesh>(NULL), &UI));
	player->addCollider(glm::vec3(0), player->radius);
	player->setJump(10.0);
	player->setJumpTime(0.5);
	entities.push_back(player);
	land->setTarget(player);
	player->testName = 13;
	player->colliders[0]->colliderType = 2;
	Physics::addPhysicsBody(player);

	//HUD
	
	std::shared_ptr<Gun> gun(new Gun(0.05, portalGunMesh, &UI));
	gun->relative.position = glm::vec3(0, 0.5, -0.5);
	gun->relative.rotation.y = 0;
	gun->relative.rotation.x = -10;
	double gunScale = 0.1;
	//gun->transform.scale = glm::vec3(gunScale);
	//gun->transform.position = glm::vec3(0, 0, 0);
	gun->setScale(glm::vec3(gunScale));
	gun->setPosition(glm::vec3(0));
	gun->mesh = portalGunMesh;
	gun->shader = colorShader;
	
//	world1->addChild(gun);
//	items.push_back(gun);

	//LIGHTING

	std::shared_ptr<Light> light0 = Light::createPointLight(glm::vec3(0, 20, 0), 60.0, 50.0, glm::vec3(255, 255, 255));//Light::createSpotLight(glm::vec3(0,15,0), glm::vec3(0, 0, 0), 30.0, 20.0);
	//UI.addLight(light0);

	std::shared_ptr<Light> light1 = Light::createPointLight(glm::vec3(0, 40, 0), 120, 10.0, glm::vec3(255, 255, 255));//Light::createSpotLight(glm::vec3(0,15,0), glm::vec3(0, 0, 0), 30.0, 20.0);
	//UI.addLight(light1);

	glm::vec4 lightAxis(0, 1, 0, 0);

	double lightAngleX = 60;
	double lightAngleZ = 0;
	double lightAngleY = 120;
	glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(lightAngleX * TO_RAD, lightAngleY * TO_RAD, lightAngleZ * TO_RAD)));
	lightAxis = rotation * lightAxis;

	Light::globalLightDirection = glm::vec3(lightAxis);
	Light::globalLightIntensity = 0.2;
	Light::ambient = 0.6;

	double shootDelay = 0.1;
	double shootRefill = 0.0;
	float ballSpeed = 30.0;
	int numBalls = 1;//3

	bool updateToggle = false;

	camera.gamma = 10.0;

	double lightPolarity = 1.0;
	double lightPolarity1 = 1.0;

	Toggle mouseLockToggle;
	Toggle updateToggleB;

	std::shared_ptr<OctTree> noCollider(NULL);
	//Physics test
	
	Toggle spawnBallToggle;
	
	//physics setup
	std::shared_ptr<GameObjectColor> basic0(new GameObjectColor);
	basic0->mesh = cubeMesh;
	basic0->shader = colorShader;
	basic0->setFillColor(200, 200, 80);
	basic0->transform.scale = glm::vec3(4, 1, 4);
	basic0->transform.position = glm::vec3(0, -2, 0);
	world1->addChild(basic0);
	
	std::shared_ptr<OctTree> tree(new OctTree());
	tree->build(world2, 6, 2);
	Physics::addCollisionStructure(tree);
	Physics::addCollisionStructure(land);

	//Physics::gravity = glm::vec3(0, -10, 0);

	do
	{
		if (spawnBallToggle.toggle(UI.isKeyPressed(GLFW_KEY_F)))
		{
			std::shared_ptr<Entity> ball(new Entity(0.0, ballMesh, &UI));
			ball->mesh = ballMesh;
			ball->shader = colorShader;
			ball->setFillColor(rand() % 160, rand() % 160, rand() % 160);
			world1->addChild(ball);
			ball->setPosition(player->getPosition());
			ball->friction = 2.0;
			ball->addCollider(glm::vec3(0, 0, 0), 1.0);
			ball->applyImpulse(player->velocity, glm::vec3(0));
			ball->mass = 1.0;
			//ball->colliders[0]->colliderType = 2;
			Physics::addPhysicsBody(ball);
		}


		player->move();
		player->interact(items);

		Physics::physicsStep(UI.deltaTime, 4);


		if (mouseLockToggle.toggle(UI.isKeyPressed(GLFW_KEY_2)))
		{
			if (UI.getMouseLockState())
				UI.unlockMouse();
			else
				UI.lockMouse();
		}
		if (updateToggleB.toggle(UI.isKeyPressed(GLFW_KEY_5)))
		{
			updateToggle = !updateToggle;
		}

		if (UI.isMouseDown(GLFW_MOUSE_BUTTON_1) && shootRefill > shootDelay)
		{
			//spawn projectile
			shootRefill = 0.0;
			for (int i = 0; i < numBalls; i++)
			{
				double radius = 0.2;
				/*
				glm::vec3 direction = Transform::getTransformedZ(player->lookRotation + glm::vec3(Gen::random() * 60, Gen::random() * 60, Gen::random() * 60));
				//glm::vec3 direction = Transform::getTransformedZ(player->lookRotation);
				std::shared_ptr<Projectile> proj = Projectile::makeProjectile(radius, ballMesh, &UI, player->getPosition(), direction, ballSpeed);
				proj->shader = colorShader;
				proj->setFillColor(rand() % 255, rand() % 255, rand() % 255);
				proj->addCollider(glm::vec3(0), radius);
				proj->friction = 0.0;// 0.2;
				//proj->slidable = true;
				//proj->applyImpulse((float)ballSpeed * direction, glm::vec3(0, 0, 0));
				world1->addChild(proj);
				//entities.push_back(proj);
				proj->setPosition(glm::vec3(0,3,0));
				Physics::addPhysicsBody(proj);
				*/
				glm::vec3 direction = Transform::getTransformedZ(player->lookRotation);// +glm::vec3(Gen::random() * 60, Gen::random() * 60, Gen::random() * 60));

				std::shared_ptr<Entity> ball(new Entity(0.0, ballMesh, &UI));
				ball->mesh = ballMesh;
				ball->shader = colorShader;
				ball->setFillColor(rand() % 255, rand() % 255, rand() % 255);
				world1->addChild(ball);
				ball->setPosition(player->getPosition());
				ball->setScale(glm::vec3(radius));
				ball->friction = 2.0;
				ball->addCollider(glm::vec3(0, 0, 0), radius);
				//ball->colliders[0]->colliderType = 1;
				ball->elasticity = 1.0;
				ball->mass = 0.2;
				ball->applyImpulse(player->velocity, glm::vec3(0));
				Physics::addPhysicsBody(ball);
				ball->applyImpulse(ballSpeed * direction, glm::vec3(0, 0, 0));
			}
		}
		else
		{
			shootRefill += UI.deltaTime;
		}


		/*
		portal2->transform.position.y -= 10 * UI.deltaTime;
		if (portal2->transform.position.y < 0) portal2->transform.position.y = 40;
		portal1->transform.rotation.y += 60 * UI.deltaTime;
		*/

		Audio::configureListener(player->getPosition(), player->lookRotation);

		if (updateToggle)
			land->update();

		//	lightAngleY += UI.deltaTime * 30.0;
			//glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(lightAngleX * TO_RAD, lightAngleY * TO_RAD, lightAngleZ * TO_RAD)));
			//lightAxis = rotation * lightAxis;


		light0->position.x += lightPolarity * 6.0 * UI.deltaTime;
		if (light0->position.x >= 20.0) lightPolarity = -1.0;
		if (light0->position.x <= -20.0) lightPolarity = 1.0;

		light1->position.z += lightPolarity1 * 10.0 * UI.deltaTime;
		if (light1->position.z >= 30.0) lightPolarity1 = -1.0;
		if (light1->position.z <= -20.0) lightPolarity1 = 1.0;

		//	light0->x = player->transform.x;
		//	light0->y = player->transform.y;
		//	light0->z = player->transform.z;

		//shader0->setLight(lightAxis);

		if (UI.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
			player->updateCamera(camera, -0.1);
		else
			player->updateCamera(camera, 0.9);

		Holdable::move(items, noCollider);//land noCollider tree
		Portal::collide(portals, entities);
		Portal::preRenderPortals(portals, camera, UI.width, UI.height);
		//Entity::entityBounds(entities);

		UI.display(camera, stage);
	} while (UI.isWindowOpen());
	//	land->closeThread();

	UI.delay(0.2);

	return 1;
}
