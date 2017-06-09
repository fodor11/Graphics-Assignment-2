#include "../include/sphere.hpp"
#include <gl\glm\gtc\matrix_transform.hpp>

Sphere::Sphere(float radius, unsigned int longitude, unsigned int latitude)
{
	m_fRadius = radius;
	m_iLongitude = longitude;
	m_iLatitude = latitude;
	createVectors();
}

Sphere::~Sphere()
{
}

std::vector<glm::vec3>& Sphere::getVertices()
{
	return m_vVertices;
}

std::vector<glm::vec3>& Sphere::getNormals()
{
	return m_vNormals;
}

std::vector<glm::vec2>& Sphere::getTextureCoordinates()
{
	return m_vTextureCoords;
}

std::vector<glm::ivec3>& Sphere::getIndicies()
{
	return m_vIndices;
}

void Sphere::createVectors()
{
	glm::vec3 vertex = glm::vec3();
	glm::vec2 textureUV = glm::vec2();

	for (int i = 0; i <= m_iLatitude; i++)
	{
		float alpha = (float)(i * glm::pi<float>()) / m_iLatitude;
		vertex[1] = glm::cos(alpha);
		textureUV[1] = 1 - (i / m_iLatitude);
		for (int j = 0; j <= m_iLongitude; j++)
		{
			float beta = (float)(j * 2 * glm::pi<float>()) / m_iLongitude;
			vertex[0] = glm::sin(alpha) * glm::cos(beta);
			vertex[2] = glm::sin(alpha) * glm::sin(beta);

			m_vNormals.push_back(vertex);

			vertex = vertex * m_fRadius;
			m_vVertices.push_back(vertex);

			textureUV[0] = 1 - (j / m_iLongitude);
			m_vTextureCoords.push_back(textureUV);
		}
	}

	//indices
	// top-----top+1
	//  | \    |
	//  |  \   |
	//  |   \  |
	//  |    \ |
	// bot.----bot.+1
	// bot, bot+1, top;
	// top, bot+1, top+1
	for (int i = 0; i < m_iLatitude; i++)
	{
		for (int j = 0; j < m_iLongitude; j++)
		{
			int top = i * (m_iLongitude + 1) + j;
			int bottom = top + m_iLongitude;
			m_vIndices.push_back(glm::ivec3(bottom, bottom + 1, top));
			m_vIndices.push_back(glm::ivec3(top, bottom + 1, top +1));
		}
	}
}
