#include "RenderManager.h"

void RenderManager::init()
{
	Shader shader;
	shader.load("DATA/SHADER/Example.vsh", GL_VERTEX_SHADER);
	shader.load("DATA/SHADER/Example.fsh", GL_FRAGMENT_SHADER);
	shaders.push_back(shader);
	ShaderManager& shM = ShaderManager::getInstance();
	shM.AddShader(&shaders[0]);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
}

void RenderManager::prepare()
{
	drawcallCount = 0;
	materialChangeCount = 0;
	graphicObjects.clear();
}

void RenderManager::setCamera(Camera* camera)
{
	this->camera = camera;
}

void RenderManager::setLight(Light* light)
{
	this->light = light;
}

void RenderManager::setFog(Fog* fog)
{
	this->fog = fog;
}

void RenderManager::addToRenderQueue(GraphicObject* graphicObject)
{
	graphicObjects.push_back(graphicObject);
	objCount = graphicObjects.size();
}

void RenderManager::render()
{
	// Clear the framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	// Tex filtering mode
	Texture::FilteringMode = TexFilters::BILINEAR_MIPMAP;

	ResourceManager& rm = ResourceManager::Instance();
	glm::mat4& view = camera->getViewMatrix();
	glm::vec4 eye_pos = glm::vec4(camera->getPosition(), 1.0);
	// Shaders
	shaders[0].activate();
	shaders[0].setUniform("projection", camera->getProjectionMatrix());
	if (fog)
	{
		shaders[0].setUniform("fogColor", fog->getColor());
		shaders[0].setUniform("fogDensity", fog->getDensity());
		shaders[0].setUniform("fogHeight", fog->getHeight());
	}
	// Lighting
	shaders[0].setUniform("lAmbient", light->getAmbient());
	shaders[0].setUniform("lDirection", view * light->getDirection());
	shaders[0].setUniform("lDiffuse", light->getDiffuse());
	shaders[0].setUniform("lSpecular", light->getSpecular());

	// Sort
	std::sort(graphicObjects.begin(), graphicObjects.end(),
		[](GraphicObject* a, GraphicObject* b)
		{
			if (a->getMaterialId() < b->getMaterialId())
				return true;
			if (a->getMaterialId() == b->getMaterialId() && a->getMeshId() < b->getMeshId())
				return true;
			return false;
		}
	);

	// Initialization
	std::vector<glm::mat4> models;
	
	int instCount = 0;
	shaders[0].setUniform("view", view);
	shaders[0].setUniform("eye_pos", eye_pos);
	int maxInd = graphicObjects.size() - 1;
	for (int i = 0; i < graphicObjects.size(); i++)
	{
		int meshCur = graphicObjects[i]->getMeshId();
		int matCur = graphicObjects[i]->getMaterialId();
		int matNext = i != maxInd ? graphicObjects[i + 1]->getMaterialId() : -1;
		int meshNext = i != maxInd? graphicObjects[i + 1]->getMeshId(): -1;
		glm::mat4 model = graphicObjects[i]->getModelMatrix();
		models.push_back(model);
		instCount++;
		
		Material* material = rm.getMaterial(matCur);
		if (material && models.size() == 1)
		{
			materialChangeCount++;
			shaders[0].setUniform("tex0", 0);
			shaders[0].setUniform("mAmbient", material->getAmbient());
			shaders[0].setUniform("mDiffuse", material->getDiffuse());
			shaders[0].setUniform("mSpecular", material->getSpecular());
			shaders[0].setUniform("mShininess", material->getShininess());
			int texId = material->getTexture();
			if (texId > -1)
			{
				Texture* tex = rm.getTexture(material->getTexture());
				tex->bind(GL_TEXTURE0);
			}
		}

		Mesh* mesh = rm.getMesh(meshCur);
		if ( mesh && (meshCur != meshNext || instCount == 5 || matCur != matNext) )
		{
			
			shaders[0].setUniform("model", models);
			mesh->drawInstanced(instCount);
			drawcallCount++;
			models.clear();
			instCount = 0;
		}
	}
	fps++;
	//system("pause");

}