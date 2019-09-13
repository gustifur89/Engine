#include "Portal.h"


// ============================ Portal ===============================

Portal::Portal()
{
	allPortals.push_back(this);
	renderTexture = RenderTexture(PORTAL_TEXTURE_SIZE, PORTAL_TEXTURE_SIZE);
	//renderTexture = RenderTexture(800, 600);
	radius = 1.0f;
//	glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.frameBuffer);

//	glSetUp();
}

std::vector<Portal*>Portal::allPortals = std::vector<Portal*>();

Portal::~Portal()
{

}

void Portal::setRadius(float radius)
{
	this->radius = radius;
	transform.scaleX = radius;
	transform.scaleY = radius;
	transform.scaleZ = radius;
}

void Portal::setWorld(std::shared_ptr<GameObject> world)
{
	this->world = world;
}

void Portal::renderFunc(Camera& camera)
{
	if (updated)
	{
		glm::mat4 MVPmatrix = camera.getProjection() * camera.getTransform() * transform.getTransform();

		glm::mat4 scale = glm::scale(glm::vec3(1.0 / radius));

		//glm::quat rotation = glm::lookAt(glm::vec3(camera.x, camera.y, camera.z), glm::vec3(transform.x, transform.y, transform.z), glm::vec3(0, 1, 0));
		glm::quat rotation = glm::lookAt(glm::vec3(transform.x, transform.y, transform.z), glm::vec3(camera.x, camera.y, camera.z), glm::vec3(0, 1, 0));
		glm::vec3 euler = (float) TO_DEG * glm::eulerAngles(rotation);

	//	transform.angleX = euler.x;
	//	transform.angleY = euler.y;
	//	transform.angleZ = euler.z;

		glm::mat4 NMmatrix = camera.getProjection();// *glm::mat4(rotation);// camera.getTransform();// *transform.getTransform();// *rotation;// *transform.getTransform() * scale;// transform.getScale();// camera.getRotation() * camera.getScale() * transform.getRotation() * transform.getScale();// camera.getTransform() * scale;
		
		glBindTexture(GL_TEXTURE_2D, renderTexture.frameBuffer);
		shader->useShader();
		shader->setLightInternal(shader->light);
		shader->setMatrixes(MVPmatrix, NMmatrix, colorMatrix);
		mesh->render();
	}

	updated = false;
}

float Portal::getMinZ(Camera camera)
{
	float x = camera.x - this->transform.x;
	float y = camera.y - this->transform.y;
	float z = camera.z - this->transform.z;

	return sqrt(x*x + y*y + z*z);
}

double Portal::distanceToPlane(glm::vec3 entPos, glm::vec3 dir, glm::vec3 planePoint)
{
	dir = glm::normalize(dir);
	dir = -dir;
	return (glm::dot(entPos, dir) - glm::dot(dir, planePoint)) / glm::length(dir);
//	glm::vec4 n = glm::vec4(dir, glm::dot(dir, planePoint));
//	return glm::dot(entPos, glm::vec3(n)) + n.w;
	//return glm::dot(entPos, dir) - glm::dot(dir, planePoint);
}

void Portal::portalRender(Camera camera)
{
	if (!otherPortal) return;

	//solve for minZ
	//get distance from the viewer to the plane of the portal, perpedicular to the direction... so closest point.

	glm::vec3 portalVertex = glm::vec3(this->transform.x, this->transform.y, this->transform.z);
	float minZ = distanceToPlane(camera.getPosition(), Transform::getTransformedZ(glm::vec3(camera.angleX, camera.angleY, camera.angleZ)), portalVertex);// fmax((getMinZ(camera) - radius), 0.1f);
//	float minZA = fmax((getMinZ(camera) - radius), 0.1f);

	minZ = fmax(0.0, minZ - 0.5);

	//to get the true minZ, we need to use the matrix projections...

	//camera.fov * (1.0 - (minZ / camera.maxZ))
	Camera portalCamera(camera.fov, 1.0f, minZ, camera.maxZ);

	portalCamera.x = otherPortal->transform.x - (this->transform.x - camera.x);
	portalCamera.y = otherPortal->transform.y - (this->transform.y - camera.y);
	portalCamera.z = otherPortal->transform.z - (this->transform.z - camera.z);

	portalCamera.angleX = camera.angleX;
	portalCamera.angleY = camera.angleY;
	portalCamera.angleZ = camera.angleZ;

	glBindTexture(GL_TEXTURE_2D, renderTexture.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.frameBuffer);
	glViewport(0, 0, PORTAL_TEXTURE_SIZE, PORTAL_TEXTURE_SIZE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	world->render(portalCamera);
	updated = true;
}

void Portal::preRenderPortals(std::vector<std::shared_ptr<Portal>> portals, Camera& camera, int depth)
{
	//find first to render
	//we need to use recursion to find them..
	//need to find if one is in the view frustum
	for (int i = 0; i < portals.size(); i++)
	{
		//portals[i]->updated = true;
		portals[i]->portalRender(camera);
	}

}

void Portal::linkPortals(std::shared_ptr<Portal> portal1, std::shared_ptr<Portal> portal2)
{
	portal1->otherPortal = portal2;
	portal2->otherPortal = portal1;
}

void Portal::collideInternal(std::shared_ptr<Entity> entity)
{
	double x = entity->transform.x - transform.x;
	double y = entity->transform.y - transform.y;
	double z = entity->transform.z - transform.z;
	double dist = sqrt(x*x + y*y + z*z);

	if (dist <= entity->collider.radius + radius)
	{
		if (!entity->portalCollide && otherPortal)
		{
			entity->transform.x = otherPortal->transform.x + x;
			entity->transform.y = otherPortal->transform.y + y;
			entity->transform.z	= otherPortal->transform.z + z;
		//	entity->portalCollide = true;
		}
		entity->portalCollide = true;
	}
	else if (dist <= 2.0 * (entity->collider.radius + radius))
	{
		entity->portalCollide = false;
	}
}

void Portal::collide(std::vector<std::shared_ptr<Portal>> portals, std::shared_ptr<Entity> entity)
{
	for (int i = 0; i < portals.size(); i++)
	{
		portals[i]->collideInternal(entity);
	}
}

void Portal::collide(std::vector<std::shared_ptr<Portal>> portals, std::vector<std::shared_ptr<Entity>> entities)
{
	for (int i = 0; i < entities.size(); i++)
	{
		collide(portals, entities[i]);
	}
}
