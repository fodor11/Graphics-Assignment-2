#include "../include/light.hpp"

Light::Light(tdogl::Program * shaderProgram, glm::vec3 position, glm::vec3 ambientIntensities, glm::vec3 diffuseIntensities, glm::vec3 specularIntensities)
{
	m_vPosition = position;
	m_pProgram = shaderProgram;
	m_vAmbientIntensities = ambientIntensities;
	m_vDiffuseIntensities = diffuseIntensities;
	m_vSpecularIntensities = specularIntensities;
	setShaderUniforms();
}

void Light::updatePosition(glm::vec3 newPosition)
{
	m_vPosition = newPosition;
	m_pProgram->use();
	m_pProgram->setUniform("light.position", m_vPosition);
	m_pProgram->stopUsing();
}

void Light::updateAmbient(glm::vec3 newIntensities)
{
	m_vAmbientIntensities = newIntensities;
	m_pProgram->use();
	m_pProgram->setUniform("light.ambientIntensities", m_vAmbientIntensities);
	m_pProgram->stopUsing();
}
void Light::updateDiffuse(glm::vec3 newIntensities)
{
	m_vDiffuseIntensities = newIntensities;
	m_pProgram->use();
	m_pProgram->setUniform("light.diffuseIntensities", m_vDiffuseIntensities);
	m_pProgram->stopUsing();
}
void Light::updateSpecular(glm::vec3 newIntensities)
{
	m_vSpecularIntensities = newIntensities;
	m_pProgram->use();
	m_pProgram->setUniform("light.specularIntensities", m_vSpecularIntensities);
	m_pProgram->stopUsing();
}

void Light::updateLight(glm::vec3 newPosition, glm::vec3 ambientIntensities, glm::vec3 diffuseIntensities, glm::vec3 specularIntensities)
{
	m_vPosition = newPosition; 
	m_vAmbientIntensities = ambientIntensities;
	m_vDiffuseIntensities = diffuseIntensities;
	m_vSpecularIntensities = specularIntensities;
	setShaderUniforms();
}

glm::vec3 Light::getPosition()
{
	return m_vPosition;
}

glm::vec3 Light::getAmbientIntensities()
{
	return m_vAmbientIntensities;
}
glm::vec3 Light::getDiffuseIntensities()
{
	return m_vDiffuseIntensities;
}
glm::vec3 Light::getSpecularIntensities()
{
	return m_vSpecularIntensities;
}

void Light::setShaderUniforms()
{
	m_pProgram->use();
	m_pProgram->setUniform("light.position", m_vPosition);
	m_pProgram->setUniform("light.ambientIntensities", m_vAmbientIntensities);
	m_pProgram->setUniform("light.diffuseIntensities", m_vDiffuseIntensities);
	m_pProgram->setUniform("light.specularIntensities", m_vSpecularIntensities);
	m_pProgram->stopUsing();
}
