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

Portal::Portal(int width, int height)
{
	allPortals.push_back(this);
	renderTexture = RenderTexture(width, height);
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
	transform.scale = glm::vec3(radius);
}

void Portal::setWorld(std::shared_ptr<GameObject> world)
{
	this->world = world;
}

void Portal::renderFunc(Camera& camera, glm::mat4 parentTransform)
{
	if (updated)
	{
		glm::mat4 MVPmatrix = camera.getProjection() * camera.getTransform() * parentTransform * transform.getTransform();

		glm::mat4 scale = glm::scale(glm::vec3(1.0 / radius));

		//glm::quat rotation = glm::lookAt(glm::vec3(camera.x, camera.y, camera.z), glm::vec3(transform.x, transform.y, transform.z), glm::vec3(0, 1, 0));
		glm::quat rotation = glm::lookAt(transform.position, camera.position, glm::vec3(0, 1, 0));
		glm::vec3 euler = (float) TO_DEG * glm::eulerAngles(rotation);

	//	transform.angleX = euler.x;
	//	transform.angleY = euler.y;
	//	transform.angleZ = euler.z;

		glm::mat4 NMmatrix = camera.getProjection();// *glm::mat4(rotation);// camera.getTransform();// *transform.getTransform();// *rotation;// *transform.getTransform() * scale;// transform.getScale();// camera.getRotation() * camera.getScale() * transform.getRotation() * transform.getScale();// camera.getTransform() * scale;
		
		glBindTexture(GL_TEXTURE_2D, renderTexture.frameBuffer);
		glBindTextureUnit(3, renderTexture.colTex);
		glBindTextureUnit(4, renderTexture.posTex);
		glBindTextureUnit(5, renderTexture.normTex);
		shader->useShader();
		shader->setLightInternal(shader->light);
		shader->setMatrixes(MVPmatrix, NMmatrix, colorMatrix);
		mesh->render();
	}

	updated = false;
}

float Portal::getMinZ(Camera camera)
{
	return glm::length(camera.position - this->transform.position);
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

void Portal::portalRender(Camera camera, int width, int height)
{
	if (!otherPortal) return;

	//solve for minZ
	//get distance from the viewer to the plane of the portal, perpedicular to the direction... so closest point.

	glm::vec3 portalVertex = this->transform.position;
	float minZ = distanceToPlane(camera.position, camera.getTransformedZ(), portalVertex);// fmax((getMinZ(camera) - radius), 0.1f);
//	float minZA = fmax((getMinZ(camera) - radius), 0.1f);

	minZ = fmax(0.0, minZ - 0.5);

	//to get the true minZ, we need to use the matrix projections...

	//camera.fov * (1.0 - (minZ / camera.maxZ))
	Camera portalCamera(camera.fov, camera.aspectRatio, minZ, camera.maxZ);

	portalCamera.position = otherPortal->transform.position + (camera.position - this->transform.position);

	portalCamera.setRotation(camera);

	//glBindTexture(GL_TEXTURE_2D, renderTexture.frameBuffer);
//	glBindTextureUnit(0, renderTexture.colTex);
//	glBindTextureUnit(1, renderTexture.posTex);
//	glBindTextureUnit(2, renderTexture.normTex);
	glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.frameBuffer);
//	glViewport(0, 0, PORTAL_TEXTURE_SIZE, PORTAL_TEXTURE_SIZE);
	glViewport(0, 0, width, height);
//	glViewport(0, 0, 900, 650);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	world->render(portalCamera, glm::mat4(1.0));
	updated = true;
}

void Portal::preRenderPortals(std::vector<std::shared_ptr<Portal>> portals, Camera& camera, int width, int height, int depth)
{
	//find first to render
	//we need to use recursion to find them..
	//need to find if one is in the view frustum
	for (int i = 0; i < portals.size(); i++)
	{
		//portals[i]->updated = true;
		portals[i]->portalRender(camera, width, height);
	}

}

void Portal::linkPortals(std::shared_ptr<Portal> portal1, std::shared_ptr<Portal> portal2)
{
	portal1->otherPortal = portal2;
	portal2->otherPortal = portal1;
}

void Portal::setUpShader(std::shared_ptr<ColorShader> shader)
{
	shader->useShader();
	GLuint colTexLoc = shader->getUniformLocation("colTex");
	GLuint posTexLoc = shader->getUniformLocation("posTex");
	GLuint normTexLoc = shader->getUniformLocation("normTex");
	glUniform1i(colTexLoc, 3);
	glUniform1i(posTexLoc, 4);
	glUniform1i(normTexLoc, 5);
}

void Portal::collideInternal(std::shared_ptr<Entity> entity)
{
	glm::vec3 pos = entity->getPosition() - transform.position;
	double dist = glm::length(pos);

	if (dist <= entity->radius + radius)
	{
		if (!entity->portalCollide && otherPortal)
		{
			entity->setPosition(otherPortal->transform.position + pos);
		//	entity->portalCollide = true;
		}
		entity->portalCollide = true;
	}
	else if (dist <= 2.0 * (entity->radius + radius))
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
