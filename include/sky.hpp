#pragma once
#include <gl/glew.h>
#include <gl/freeglut.h>

#include "texture.hpp"

class Sky {
public:
	/// empty
	Sky();
	/// empty
	~Sky();
	/// init spheres, light
	void initialize();
	/// updates datas, draws sky and moon, replaces moonlight
	void updateSky(float cameraX, float cameraY, float cameraZ, float elapsedTime);
	void toggleMoonLight();

private:
	unsigned int m_uiSkyTextureId = 0;
	GLUquadric *m_pSkySphere;

	unsigned int m_uiMoonTextureId = 0;
	GLUquadric *m_pMoonSphere;
	float m_fMoonSpeed = 0.001f;
	float m_fMoonCurvePosition = 1.5f;
	float m_fSemiAxleX = 300.f;
	float m_fSemiAxleY = 200.f;

	GLenum m_MoonLightId = GL_LIGHT1;
	bool m_bMoonlight = true;
	GLfloat m_LightPosition[4] = { -50.f, 450.f, 120.f , 1.f };

	GLfloat m_MoonEmissive[4] = { 0.7f, 0.7f, 0.75f, 1.0f };
	GLfloat m_MoonMaterial[4] = { 0.f, 0.f, 0.f, 1.f };

	GLfloat m_SkyMaterial[4] = { 0.5f, 0.5f, 0.5f, 1.f };
	GLfloat m_SkyEmissive[4] = { 0.f, 0.f, 0.f, 1.f };

	void loadTextures();
	void drawMoon();
	void drawSky();
	void moveMoon(float elapsedTime);
};
