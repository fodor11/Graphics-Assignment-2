#include "../include/environment.hpp"

Environment::Environment()
{
}

Environment::~Environment()
{
	delete m_pFog;
	delete m_pForest;
	delete m_pRain;
	delete m_pTree1;
	delete m_pTree2;
	delete m_pTree3;
	delete m_pSky;
}

void Environment::initialize(HeightMapLoader* heightMap, Camera* camera, tdogl::Program* skyShaderProgram, tdogl::Program* treeShaderProgram, std::vector<tdogl::Program*>& allShaders)
{
	m_pCamera = camera;
	m_pHeightmap = heightMap;
	m_vAllShaders = allShaders;
	m_pSkyShader = skyShaderProgram;
	m_pTreeShader = treeShaderProgram;
	//m_pFog = new DynamicFog(camera);
	//m_pRain = new Rain(camera, heightMap);

	m_pSky = new Sky();
	m_pSky->initialize(m_pSkyShader, m_vAllShaders);

	m_pForest = new Forest(m_sForestFileName);
	m_pForest->initialize(m_pHeightmap);

	m_pCamera->setObstacles(m_pForest->getPositionsMap());

	m_pTree1 = new Tree(m_sTree1FileName, m_pTreeShader);
	m_pTree2 = new Tree(m_sTree2FileName, m_pTreeShader);
	m_pTree3 = new Tree(m_sTree3FileName, m_pTreeShader);
}

void Environment::update()
{
	//m_pFog->updateFog();

	//m_pRain->update();

	m_pSky->updateSky(m_pCamera->getX(), m_pCamera->getY(), m_pCamera->getZ(), m_pCamera->getElapsedTime());

	std::map<std::string, std::vector<vec3f>>& positions = *m_pForest->getPositionsMap();
	std::string tmpTreeId;
	vec3f tmpPosition;
	Tree* tmpTree = nullptr;
	for (auto map = positions.begin(); map != positions.end(); map++)
	{
		tmpTreeId = map->first;
		std::vector<vec3f>& actualPositions = map->second;
		for (int j = 0; j < actualPositions.size(); j++)
		{
			tmpPosition = actualPositions[j];
			tmpTree = determineTree(tmpTreeId);
			tmpTree->translate(glm::vec3(tmpPosition[0], tmpPosition[1], tmpPosition[2]));
			drawTree(tmpTree, calcDistanceToCamera(tmpPosition));
		}
	}
}

void Environment::changeAmbientLight(float value)
{
	m_fOverallLightness += value;
	if (m_fOverallLightness > 1.0f)
	{
		m_fOverallLightness = 1.0f;
	}
	else if (m_fOverallLightness < 0.0f)
	{
		m_fOverallLightness = 0.0f;
	}
	m_vLightModelAmbient[0] = m_fOverallLightness;
	m_vLightModelAmbient[1] = m_fOverallLightness;
	m_vLightModelAmbient[2] = m_fOverallLightness;
}

void Environment::toggleMoonlight()
{
	m_pSky->toggleMoonLight();
}

void Environment::toggleFog()
{
	m_pFog->toggleFog();
	m_pRain->toggleRain();
}

float Environment::calcDistanceToCamera(vec3f position)
{
	vec3f camPos(m_pCamera->getX(), m_pCamera->getY(), m_pCamera->getZ());
	vec3f dist = camPos - position;
	return dist.length();
}

void Environment::drawTree(Tree* tree, float distanceToCamera)
{
	if (distanceToCamera < m_fVisualRange)
	{
		tree->drawTree();
	}
	else
	{
		tree->drawBillBoard();
	}
}

Tree * Environment::determineTree(std::string treeId)
{
	if (strcmp(treeId.c_str(), "tree1") == 0)
	{
		return m_pTree1;
	}
	else if (strcmp(treeId.c_str(), "tree2") == 0)
	{
		return m_pTree2;
	}
	else
	{
		return m_pTree3;
	}
}
