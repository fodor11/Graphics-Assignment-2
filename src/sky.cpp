#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include "../include/sky.hpp"

Sky::Sky()
{
}

Sky::~Sky()
{
}

void Sky::initialize(tdogl::Program *shaderProgram)
{
	//TODO: skysphere vao

	//TODO: set up moon

	// Light
	glm::vec3 moonLight_ambient = glm::vec3(0.5f, 0.5f, 0.6f);
	glm::vec3 moonLight_diffuse = glm::vec3(0.5f, 0.5f, 0.6f);
	glm::vec3 moonLight_specular = glm::vec3(0.5f, 0.5f, 0.5f);
	m_pProgram = shaderProgram;
	m_pLight = new Light(shaderProgram, m_LightPosition, moonLight_ambient, moonLight_diffuse, moonLight_specular);

	loadTextures();
}

void Sky::updateSky(float cameraX, float cameraY, float cameraZ, float elapsedTime)
{
	//moveMoon(elapsedTime);
	//drawMoon();
	//glTranslatef(cameraX, cameraY, cameraZ);
	//drawSky();

	//reset position of the lightsource
	m_pLight->updatePosition(m_LightPosition);
}

void Sky::toggleMoonLight()
{
	if (m_bMoonlight)
	{
		//glDisable(m_MoonLightId);
		m_bMoonlight = false;
	}
	else
	{
		//glEnable(m_MoonLightId);
		m_bMoonlight = true;
	}
}

void Sky::loadTextures()
{
	cout << "Loading sky textures..." << endl;
	TextureLoader *txtrLoaderObj = new TextureLoader();
	//sky
	txtrLoaderObj->loadTexture("sky5.png");
	m_uiSkyTextureId = txtrLoaderObj->textureID();
	//moon
	txtrLoaderObj->loadMipMappedTexture("moon2.jpg");
	m_uiMoonTextureId = txtrLoaderObj->textureID();
	delete txtrLoaderObj;
}

void Sky::drawMoon()
{
	// TODO: draw vao
	//glBindTexture(GL_TEXTURE_2D, m_uiMoonTextureId);

	//glTranslatef(m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]);
	//glRotatef(-75.f, 0.f, 1.f, 0.f);
	//glRotatef(45.f, 0.f, 0.f, 1.f);
	//glRotatef(45.f, 1.f, 0.f, 0.f);
	
}

void Sky::drawSky()
{
	// TODO: draw vao
	glBindTexture(GL_TEXTURE_2D, m_uiSkyTextureId);
	
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_SkyMaterial);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, m_SkyMaterial);
	//glMaterialfv(GL_FRONT, GL_EMISSION, m_SkyEmissive);
}

void Sky::moveMoon(float elapsedTime)
{
	m_fMoonCurvePosition += m_fMoonSpeed * elapsedTime;
	if (m_fMoonCurvePosition > M_PI)
	{
		m_fMoonCurvePosition = 0.0f;
	}
	//y coordinate
	m_LightPosition[1] = m_fSemiAxleY * sin(m_fMoonCurvePosition);
	//z coordinate
	m_LightPosition[2] = m_fSemiAxleX * cos(m_fMoonCurvePosition);
}
