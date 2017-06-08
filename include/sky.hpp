#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/glm/glm.hpp>
#include "texture.hpp"
#include "light.hpp"
#include "../src/tdogl/Program.h"

class Sky {
public:
	/// empty
	Sky();
	/// empty
	~Sky();
	/// init spheres, light
	void initialize(tdogl::Program *shaderProgram);
	/// updates datas, draws sky and moon, replaces moonlight
	void updateSky(float cameraX, float cameraY, float cameraZ, float elapsedTime);
	void toggleMoonLight();

private:
	unsigned int m_uiSkyTextureId = 0;

	unsigned int m_uiMoonTextureId = 0;
	float m_fMoonSpeed = 0.001f;
	float m_fMoonCurvePosition = 1.5f;
	float m_fSemiAxleX = 300.f;
	float m_fSemiAxleY = 200.f;

	Light *m_pLight;
	tdogl::Program *m_pProgram;
	bool m_bMoonlight = true;
	glm::vec3 m_LightPosition = glm::vec3(-50.f, 450.f, 120.f);

	glm::vec3 m_MoonMaterial = glm::vec3(0.f, 0.f, 0.f);

	glm::vec3 m_SkyMaterial = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 m_SkyEmissive = glm::vec3(0.f, 0.f, 0.f);

	void loadTextures();
	void drawMoon();
	void drawSky();
	void moveMoon(float elapsedTime);
};
