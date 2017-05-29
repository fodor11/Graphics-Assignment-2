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

void Sky::initialize()
{
	//create skysphere
	m_pSkySphere = gluNewQuadric();
	gluQuadricTexture(m_pSkySphere, GL_TRUE);

	//set up moon (sphere + light)
	m_pMoonSphere = gluNewQuadric();
	gluQuadricTexture(m_pMoonSphere, GL_TRUE);
	GLfloat moonLight_ambient[] = { 0.5f, 0.5f, 0.6f, 1.0f };
	GLfloat moonLight_diffuse[] = { 0.5f, 0.5f, 0.6f, 1.0f };
	GLfloat moonLight_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(m_MoonLightId, GL_POSITION, m_LightPosition);
	glLightfv(m_MoonLightId, GL_AMBIENT, moonLight_ambient);
	glLightfv(m_MoonLightId, GL_DIFFUSE, moonLight_diffuse);
	glLightfv(m_MoonLightId, GL_SPECULAR, moonLight_specular);
	glEnable(m_MoonLightId);

	glEnable(GL_LIGHTING);

	loadTextures();
}

void Sky::updateSky(float cameraX, float cameraY, float cameraZ, float elapsedTime)
{
	glPushMatrix();
	moveMoon(elapsedTime);
	drawMoon();
	glTranslatef(cameraX, cameraY, cameraZ);
	drawSky();
	//reset position of the lightsource
	glLightfv(m_MoonLightId, GL_POSITION, m_LightPosition);
	glPopMatrix();
}

void Sky::toggleMoonLight()
{
	if (m_bMoonlight)
	{
		glDisable(m_MoonLightId);
		m_bMoonlight = false;
	}
	else
	{
		glEnable(m_MoonLightId);
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

/// Temporary
void drawAxisB(float nullX, float nullY, float nullZ)
{
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glTranslatef(nullX, nullY, nullZ);
	glLineWidth(10.0f);
	glBegin(GL_LINES);
	//x axis RED
	GLfloat diffuse_and_ambient[] = { 1,0,0,1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse_and_ambient);
	glColor3f(1.0f, 0.f, 0.f);  glVertex3f(0.f, 0.f, 0.f);	glVertex3f(3.0f, 0.f, 0.f);
	//y axis GREEN
	diffuse_and_ambient[0] = 0;
	diffuse_and_ambient[1] = 1;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse_and_ambient);
	glColor3f(0.0f, 1.0f, 0.f);	glVertex3f(0.f, 0.f, 0.f);	glVertex3f(0.0f, 3.0f, 0.f);
	//z axis BLUE
	diffuse_and_ambient[1] = 0;
	diffuse_and_ambient[2] = 1;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse_and_ambient);
	glColor3f(0.0f, 0.f, 1.f);	glVertex3f(0.f, 0.f, 0.f);	glVertex3f(0.0f, 0.f, 3.f);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}
/// Temporary

void Sky::drawMoon()
{
	glBindTexture(GL_TEXTURE_2D, m_uiMoonTextureId);
	glPushMatrix();
	glTranslatef(m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]);
	glRotatef(-75.f, 0.f, 1.f, 0.f);
	glRotatef(45.f, 0.f, 0.f, 1.f);
	glRotatef(45.f, 1.f, 0.f, 0.f);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_MoonMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_MoonMaterial);
	glMaterialfv(GL_FRONT, GL_EMISSION, m_MoonEmissive);
	gluSphere(m_pMoonSphere, 30.0f, 36, 72);
	glPopMatrix();

}

void Sky::drawSky()
{
	glBindTexture(GL_TEXTURE_2D, m_uiSkyTextureId);
	glPushMatrix();
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_SkyMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_SkyMaterial);
	glMaterialfv(GL_FRONT, GL_EMISSION, m_SkyEmissive);
	gluQuadricOrientation(m_pSkySphere, GLU_INSIDE);
	gluSphere(m_pSkySphere, 1500.0, 36, 72);
	glPopMatrix();
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
