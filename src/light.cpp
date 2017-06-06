#include "../include/light.hpp"

Light::Light(tdogl::Program * shaderProgram, glm::vec3 position, glm::vec3 intensities)
{
	m_vPosition = position;
	m_vIntensities = intensities;
	m_pProgram = shaderProgram;
	setShaderUniforms();
}

void Light::updatePosition(glm::vec3 newPosition)
{
	m_vPosition = newPosition;
	m_pProgram->use();
	m_pProgram->setUniform("light.position", m_vPosition);
	m_pProgram->stopUsing();
}

void Light::updateIntensities(glm::vec3 newIntensities)
{
	m_vIntensities = newIntensities;
	m_pProgram->use();
	m_pProgram->setUniform("light.intensities", m_vIntensities);
	m_pProgram->stopUsing();
}

void Light::updateLight(glm::vec3 newPosition, glm::vec3 newIntensities)
{
	m_vPosition = newPosition;
	m_vIntensities = newIntensities;
	setShaderUniforms();
}

glm::vec3 Light::getPosition()
{
	return m_vPosition;
}

glm::vec3 Light::getIntensities()
{
	return m_vIntensities;
}

void Light::setShaderUniforms()
{
	m_pProgram->use();
	m_pProgram->setUniform("light.position", m_vPosition);
	m_pProgram->setUniform("light.intensities", m_vIntensities);
	m_pProgram->stopUsing();
}
