#pragma once
#include "sky.hpp"
#include "object.hpp"
#include "forest.hpp"
#include "camera.hpp"
#include "dynamicFog.hpp"
#include "rain.hpp"
#include "light.hpp"
#include "../src/tdogl/Program.h"

class Environment
{
public:
	/// empty
	Environment();
	/// empty
	~Environment();
	/// reads and prepares objects and textures (sky, forest, trees, terrain)
	void initialize(HeightMapLoader* heightMap, Camera* camera, tdogl::Program* shaderProgram);
	/// draws everything
	void update();
	/// adds the value to the overall lightness
	void changeAmbientLight(float value);
	/// toggles moonlight on and off
	void toggleMoonlight();
	/// toggles fog on and off
	void toggleFog();
private:
	DynamicFog *m_pFog;
	Rain *m_pRain;
	Sky *m_pSky;
	tdogl::Program *m_pProgram;

	Tree *m_pTree1;
	std::string m_sTree1FileName = "pine1";
	Tree *m_pTree2;
	std::string m_sTree2FileName = "pine2";
	Tree *m_pTree3;
	std::string m_sTree3FileName = "pine3";
	Forest* m_pForest;
	std::string m_sForestFileName = "population.png";

	float m_fVisualRange = 6.5f;

	float m_fOverallLightness = 0.1;
	glm::vec3 m_vLightModelAmbient = glm::vec3( m_fOverallLightness, m_fOverallLightness, m_fOverallLightness);

	HeightMapLoader* m_pHeightmap;
	Camera* m_pCamera;

	float calcDistanceToCamera(vec3f position);
	void drawTree(Tree* tree, float distanceToCamera);
	Tree* determineTree(std::string treeId);
};