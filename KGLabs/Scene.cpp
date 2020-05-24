#include "Scene.h"

using namespace std;
using namespace rapidjson;

void Scene::init(std::string filepath)
{
	loadModels(filepath);

		//if(!loadJSON(filepath))
		//return;

}

bool Scene::loadJSON(std::string filepath)
{
	sceneFilename = filepath;

	if (modelsDescription.Empty())
	{
		printf("Error: cannot load the scene. Load models description file first!\n");
		return false;
	}
	// Clear old GO instances
	graphicObjects.clear();

	// Open the file
	ifstream file(filepath);
	if (!file.is_open())
	{
		printf("Scene: couldn't open the file \"%s\"\n", filepath.c_str());
		return false;
	}

	// Read from file into document
	rapidjson::IStreamWrapper isw(file);
	rapidjson::Document doc;
	doc.ParseStream(isw);
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	if (doc.HasParseError())
	{
		printf("Scene: json loading error %i\n Offset: %i\n", doc.GetParseError(), doc.GetErrorOffset());
		return false;
	}

	// Parse scene :(
	auto scene = doc["scene"]["GraphicObject"].GetArray();
	for (auto it = scene.Begin(); it != scene.End(); it++)
	{
		// Get object with model, position, rotation
		auto obj  = it->GetObjectW();
		// Create graphic object
		GraphicObject gObj = createGraphicObject(obj["Model"]["-id"].GetString());
		// Position and rotation
		// Position
		glm::vec3 position;
		istringstream is(obj["Position"]["-value"].GetString());
		is >> position.x;
		is >> position.y;
		is >> position.z;
		#ifdef _DEBUG
		printf("(pos: %f, %f, %f)\n", position.x, position.y, position.z);
		#endif
		gObj.setPosition(position);
		// Rotation
		float rotation;
		is = istringstream(obj["Rotation"]["-value"].GetString());
		is >> rotation;
		gObj.setAngle(-rotation);
		graphicObjects.push_back(gObj);
	}

	//printf("Document has member scene: %s\n", docScene->value);
	

	return true;
}

void Scene::initQtree(Boundary boundary)
{
	if (!graphicObjects.size())
		return;

	qtree.setBoundary(boundary);

	for (int i = 0; i < graphicObjects.size(); i++)
	{
		glm::vec3 pos = graphicObjects[i].getPosition();
		glm::vec3 dimensions = graphicObjects[i].getDimension();
		qtree.insert(
			Point(
				pos, dimensions,  i
			)
		);
	}
	//qtree.outNodes(0);
	//system("pause");
}

void Scene::simulate(float sec)
{
}

void Scene::draw()
{
	RenderManager& rm = RenderManager::Instance();
	if (camera)
		rm.setCamera(camera);
	if (light)
		rm.setLight(light);

	// Get visible objects range
	glm::mat4 VP = camera->getProjectionMatrix() * camera->getViewMatrix();
	std::vector<Point> points = qtree.queryRange(VP);

	// LOD algorithm
	int size = points.size();
	for (int i = 0; i < size; i++)
	{
		int index = points[i].index;
		if (!isFar(graphicObjects[index]))
		{
			if (isGraphicObjectInFrustum(graphicObjects[index], VP))
				rm.addToRenderQueue(&graphicObjects[index]);
		}
	}

	
}

Camera* Scene::getCamera()
{
	return camera;
}

std::string Scene::getSceneDescription()
{
	return sceneFilename;
}

GraphicObject Scene::createGraphicObject(std::string model_id)
{
	map<string, GraphicObjectType> types =
	{
		{"road", GraphicObjectType::road},
		{"building", GraphicObjectType::building},
		{"vehicle", GraphicObjectType::vehicle},
		{"big nature", GraphicObjectType::big_nature},
		{"small nature", GraphicObjectType::small_nature},
		{"big prop", GraphicObjectType::big_prop},
		{"medium prop", GraphicObjectType::medium_prop},
		{"small prop", GraphicObjectType::small_prop},
	};
	ResourceManager& rm = ResourceManager::Instance();

	GraphicObject newObject;
	auto arModels = modelsDescription["Models"]["Model"].GetArray();
	for (auto it = arModels.Begin(); it != arModels.End(); it++)
	{
		auto objModel = it->GetObjectW();
		if (objModel["-id"].GetString() == model_id)
		{
			newObject.setType(types[objModel["-type"].GetString()]);
			
			// Load dimensions from model description
			string dimensions = objModel["dimensions"]["-value"].GetString();
			// Debug
			#ifdef _DEBUG
			printf("Dimensions: %s\n", dimensions.c_str());
			#endif		
			istringstream istr(dimensions);
			glm::vec4 temp;
			istr >> temp.x;
			istr >> temp.y;
			istr >> temp.z;
			newObject.setDimension(temp); // Set it

			// Load mesh if it's not loaded
			newObject.setMesh(rm.loadMesh(objModel["Mesh"]["-path"].GetString()));
			// Initialize gobj material
			Material mat;
			mat.setTexture(
				rm.loadTexture(objModel["Material"]["Texture"]["-path"].GetString())
			);
			// Load phong parameters
			// diffuse
			istr = istringstream(objModel["Material"]["PhongParameters"]["-diffuse"].GetString());
			istr >> temp.x;
			istr >> temp.y;
			istr >> temp.z;
			mat.setDiffuse(temp);
			// ambient
			istr = istringstream(objModel["Material"]["PhongParameters"]["-ambient"].GetString());
			istr >> temp.x;
			istr >> temp.y;
			istr >> temp.z;
			mat.setAmbient(temp);
			// specular
			istr = istringstream(objModel["Material"]["PhongParameters"]["-specular"].GetString());
			istr >> temp.x;
			istr >> temp.y;
			istr >> temp.z;
			mat.setSpecular(temp);
			// shininess
			istr = istringstream(objModel["Material"]["PhongParameters"]["-shininess"].GetString());
			float shin;
			istr >> shin;
			mat.setShininess(shin);
			// Set material
			newObject.setMaterial(ResourceManager::Instance().createSharedMaterial(mat));

			break;
		}
	}

	return newObject;
}

bool Scene::loadModels(std::string filenameJSON)
{
	// Open the file
	ifstream ifs(filenameJSON);

	if (!ifs.is_open())
	{
		cerr << "Couldn't open file " << filenameJSON << " for reading." << endl;
		return false;
	}
	// Read from file
	rapidjson::IStreamWrapper isw(ifs);
	modelsDescription.ParseStream(isw); // Parse file
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	modelsDescription.Accept(writer);

	if (modelsDescription.HasParseError())
	{
		std::cout << "Error: " << modelsDescription.GetParseError() << endl
			<< "Offset: " << modelsDescription.GetErrorOffset() << endl;
		return false;
	}

	return true;
}

bool Scene::isFar(GraphicObject& gObj)
{
	GraphicObjectType type = gObj.getType();
	glm::vec3 vDistance = camera->getPosition() - gObj.getPosition();
	float distance = glm::length(vDistance);
	bool res = false;
	switch (type)
	{
		case GraphicObjectType::vehicle:
			if (distance >= 500.0)
				res = true;
			break;
		case GraphicObjectType::big_nature:
			if (distance >= 350.0)
				res = true;
			break;
		case GraphicObjectType::small_nature:
			if (distance >= 200.0)
				res = true;
			break;
		case GraphicObjectType::big_prop:
			if (distance >= 400.0)
				res = true;
			break;
		case GraphicObjectType::medium_prop:
			if (distance >= 300.0)
				res = true;
			break;
		case GraphicObjectType::small_prop:
			if (distance >= 200.0)
				res = true;
			break;
	}
	return res;
}

bool Scene::isGraphicObjectInFrustum(GraphicObject& gObj, glm::mat4& PVM)
{
	glm::vec3 pos = gObj.getPosition();
	glm::vec3 dim = gObj.getDimension() * 0.5f;
	glm::vec4 boundary_verts[] = {
		{pos.x + dim.x, pos.y + dim.y, pos.z + dim.z, 1.0},
		{pos.x + dim.x, pos.y + dim.y, pos.z - dim.z, 1.0},
		{pos.x + dim.x, pos.y - dim.y, pos.z + dim.z, 1.0},
		{pos.x + dim.x, pos.y - dim.y, pos.z - dim.z, 1.0},
		{pos.x - dim.x, pos.y + dim.y, pos.z + dim.z, 1.0},
		{pos.x - dim.x, pos.y + dim.y, pos.z - dim.z, 1.0},
		{pos.x - dim.x, pos.y - dim.y, pos.z + dim.z, 1.0},
		{pos.x - dim.x, pos.y - dim.y, pos.z - dim.z, 1.0}
	};

	int left, up, down, right, front, back;
	left = up = down = right = front = back = 0;
	for (int i = 0; i < 8; i++)
	{
		glm::vec4 clipCoord = PVM * boundary_verts[i];
		if (clipCoord.x < -clipCoord.w)
		{
			left++;
			continue;
		}
		if (clipCoord.x > clipCoord.w)
		{
			right++;
			continue;
		}
		if (clipCoord.y < -clipCoord.w)
		{
			down++;
			continue;
		}
		if (clipCoord.y > clipCoord.w)
		{
			up++;
			continue;
		}
		if (clipCoord.z < -clipCoord.w)
		{
			front++;
			continue;
		}
		if (clipCoord.z > clipCoord.w)
		{
			back++;
			continue;
		}
	}
	if (left == 8 || up == 8 || down == 8 || right == 8 || front == 8 || back == 8)
		return false;

	return true;
}