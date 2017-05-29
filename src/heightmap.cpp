#include "../include/texture.hpp"
#include "../include/heightmap.hpp"
#include <iostream>
#include <gl/glm/gtc/matrix_transform.hpp>

#ifdef __linux__
#include <cmath>
#endif

void HeightMapLoader::getHeightValues()
{
	m_pHeightValues = new float[m_width * m_height];

	for (int i = 0; i < m_width * m_height; i++)
	{
		m_pHeightValues[i] = pow((float)m_pImage[i] / 255.0f, 2);
		if (m_pHeightValues[i] > m_highestPoint)
		{
			m_highestPoint = m_pHeightValues[i];
		}
		else if (m_pHeightValues[i] < m_lowestPoint)
		{
			m_lowestPoint = m_pHeightValues[i];
		}
	}
}

void HeightMapLoader::determineColors()
{
	float lightness = 1.f;
	m_pColors = new vec3f[m_width * m_height];

	TextureLoader *biome = new TextureLoader();
	biome->loadImage("biome-smooth.png");

	for (int i = 0; i < m_width * m_height; i++)
	{
		float realtiveHeight = (m_pHeightValues[i] - m_lowestPoint) / (m_highestPoint - m_lowestPoint);
		float relativeMoisture = (m_pMoisture[i] - m_dryestPoint) / (m_wettestPoint - m_dryestPoint);
		m_pColors[i] = biome->getMoistureColor(relativeMoisture, realtiveHeight)*lightness;
	}
}

void HeightMapLoader::createVAO()
{
	TextureLoader texLoader;
	texLoader.loadMipMappedTexture("ground_soil.jpg");
	m_iTextureID = (GLuint) texLoader.textureID();
	texLoader.~TextureLoader();

	// create and bind the VAO
	glGenVertexArrays(1, &m_iTerrainVAO);
	glBindVertexArray(m_iTerrainVAO);

	// create and bind the VBO
	glGenBuffers(1, &m_iTerrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_iTerrainVBO);

	
	// x,y,z + u,v   * 4 vertices per square
	int vertexCoordsCount = 5 * 4;
	int verticesDataCount = m_width * m_height * vertexCoordsCount;
	GLfloat* terrainData = new GLfloat[verticesDataCount];	
	for (int i = 0; i < (m_width - 1); i++)
	{
		for (int j = 0; j < (m_height - 1); j++)
		{
			int actualPosition = i * vertexCoordsCount * m_width + j * vertexCoordsCount;
			//vertex
			terrainData[actualPosition + 0] = i + 1;
			terrainData[actualPosition + 1] = getHeight(i + 1, j);
			terrainData[actualPosition + 2] = j;
			//txture u,v
			terrainData[actualPosition + 3] = i;
			terrainData[actualPosition + 4] = j;

			//vertex
			terrainData[actualPosition + 5] = i;
			terrainData[actualPosition + 6] = getHeight(i, j);
			terrainData[actualPosition + 7] = j;
			//txture u,v
			terrainData[actualPosition + 8] = i;
			terrainData[actualPosition + 9] = j + 1;

			//vertex
			terrainData[actualPosition + 10] = i + 1;
			terrainData[actualPosition + 11] = getHeight(i + 1, j + 1);
			terrainData[actualPosition + 12] = j + 1;
			//txture u,v
			terrainData[actualPosition + 13] = i + 1;
			terrainData[actualPosition + 14] = j;

			//vertex
			terrainData[actualPosition + 15] = i;
			terrainData[actualPosition + 16] = getHeight(i, j + 1);
			terrainData[actualPosition + 17] = j + 1;
			//txture u,v
			terrainData[actualPosition + 18] = i + 1;
			terrainData[actualPosition + 19] = j + 1;
		}
	}

	glBufferData(GL_ARRAY_BUFFER, verticesDataCount * sizeof(GLfloat), terrainData, GL_STATIC_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(m_pProgram->attrib("vert"));
	glVertexAttribPointer(m_pProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(m_pProgram->attrib("vertTexCoord"));
	glVertexAttribPointer(m_pProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	// unbind the VAO
	glBindVertexArray(0);
}

void HeightMapLoader::getMoistValues()
{
	m_fileName = "moisture.png";
	LoadingGreyScaleImage();
	m_pMoisture = new float[m_width*m_height];
	for (int i = 0; i < m_width*m_height; i++)
	{
		m_pMoisture[i] = (float)m_pImage[i] / 255.0f;
		if (m_pMoisture[i] > m_wettestPoint)
		{
			m_wettestPoint = m_pMoisture[i];
		}
		else if (m_pMoisture[i] < m_dryestPoint)
		{
			m_dryestPoint = m_pMoisture[i];
		}
	}
}

void HeightMapLoader::LoadingGreyScaleImage()
{
	m_pImage = SOIL_load_image(m_fileName.c_str(), &m_width, &m_height, &m_channels, SOIL_LOAD_L);
	if (m_pImage == nullptr)
	{
		std::cout << m_fileName.c_str() << " could not be loaded!" << std::endl;
	}
	else
	{
		std::cout << m_fileName.c_str() << " loaded! resolution: " << m_width << "x" << m_height << std::endl;
	}
}

bool HeightMapLoader::existingCoord(int x, int z)
{
	if (x >= 0 && z >= 0 && x < m_width && z < m_height)
	{
		return true;
	}
	else
	{
		return false;
	}
}
vec3f calcNormalFromPoints(vec3f& p0, vec3f& p1, vec3f& p2)
{
	vec3f v1 = p1 - p0;
	vec3f v2 = p2 - p0;
	return v1*v2;
}
void HeightMapLoader::calcVertexNormals()
{
	std::cout << "Generating normals...\t\t";
	m_pVertexNormals = new vec3f[m_width*m_height];
	vec3f p0(0, 0, 0);
	vec3f p1(0, 0, 0);
	vec3f p2(0, 0, 0);
	int triangles;
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			vec3f& actualNormal = m_pVertexNormals[i*m_width + j];
			actualNormal = vec3f(0, 0, 0);
			p0 = vec3f(i, getUnitHeight(i, j), j);
			triangles = 0;
			//second
			if (existingCoord(i - 1, j))
			{
				if (existingCoord(i - 1, j + 1))
				{
					p1 = vec3f(i - 1, getUnitHeight(i - 1, j), j);
					p2 = vec3f(i - 1, getUnitHeight(i - 1, j + 1), j + 1);
					triangles++;
					actualNormal += calcNormalFromPoints(p0, p1, p2);
				}
			}
			//third - if 3rd exists, then 4th exists too
			if (existingCoord(i - 1, j + 1))
			{
				p1 = vec3f(i - 1, getUnitHeight(i - 1, j + 1), j + 1);
				p2 = vec3f(i, getUnitHeight(i, j + 1), j + 1);
				triangles++;
				actualNormal += calcNormalFromPoints(p0, p1, p2);
				//check if 5th exists => p1=4th, p2=5th
				if (existingCoord(i + 1, j))
				{
					p1 = vec3f(i, getUnitHeight(i, j + 1), j + 1);
					p2 = vec3f(i + 1, getUnitHeight(i + 1, j), j);
					triangles++;
					actualNormal += calcNormalFromPoints(p0, p1, p2);
				}
			}
			//fifth
			if (existingCoord(i + 1, j))
			{
				if (existingCoord(i + 1, j - 1))
				{
					p1 = vec3f(i + 1, getUnitHeight(i + 1, j), j);
					p2 = vec3f(i + 1, getUnitHeight(i + 1, j - 1), j - 1);
					triangles++;
					actualNormal += calcNormalFromPoints(p0, p1, p2);
				}
			}
			//sixth => if 6th exists, then 1st exists too
			if (existingCoord(i + 1, j - 1))
			{
				p1 = vec3f(i + 1, getUnitHeight(i + 1, j - 1), j - 1);
				p2 = vec3f(i, getUnitHeight(i, j - 1), j - 1);
				triangles++;
				actualNormal += calcNormalFromPoints(p0, p1, p2);
				//check if 2nd exists => p1=1st p2=2nd
				if (existingCoord(i - 1, j))
				{
					p1 = vec3f(i, getUnitHeight(i, j - 1), j - 1);
					p2 = vec3f(i - 1, getUnitHeight(i - 1, j), j);
					triangles++;
					actualNormal += calcNormalFromPoints(p0, p1, p2);
				}
			}

			actualNormal /= triangles;
			actualNormal.normalize();
		}
	}
	std::cout << "Done\n";
}

HeightMapLoader::HeightMapLoader(std::string fileName, tdogl::Program* program)
{
	m_pProgram = program;
	//moisture
	getMoistValues();
	SOIL_free_image_data(m_pImage);
	//height
	m_fileName = fileName;
	LoadingGreyScaleImage();
	getHeightValues();
	calcVertexNormals();
	SOIL_free_image_data(m_pImage);
	//color
	determineColors();
	//VAO
	createVAO();
}

void HeightMapLoader::drawTerrain() const
{
	// bind the program (the shaders)
	m_pProgram->use();

	// set the "model" uniform in the vertex shader
	m_pProgram->setUniform("model", glm::mat4());

	// bind the texture and set the "tex" uniform in the fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_iTextureID);
	m_pProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0

	// bind the VAO (the triangle)
	glBindVertexArray(m_iTerrainVAO);

	// draw the VAO
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (m_width - 1)*(m_height - 1) * 4);

	// unbind the VAO, the program and the texture
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_pProgram->stopUsing();
}

HeightMapLoader::~HeightMapLoader()
{
	delete[]  m_pVertexNormals;
	delete  m_pHeightValues;
	delete m_pMoisture;
	delete[] m_pColors;
	std::cout << "heightmap destroyed" << std::endl;
}

int HeightMapLoader::getImageWidth() const
{
	return m_width;
}

int HeightMapLoader::getImageHeight() const
{
	return m_height;
}

float HeightMapLoader::linearInterpolation(float pointA, float pointB, float weightB)
{
	float weightA = 1 - weightB;
	float ret = pointA * weightA + pointB * weightB;
	return ret;
}
float HeightMapLoader::getUnitHeight(float x, float z)
{
	if (x < (m_width - 1) && z < (m_height - 1) && x >= 0 && z >= 0)
	{
		float xf1, zf1;
		float weight1 = modff(x, &xf1);
		float weight2 = modff(z, &zf1);
		int x1 = (int)xf1;
		int z1 = (int)zf1;
		float lin1 = linearInterpolation(m_pHeightValues[z1 * m_width + x1], m_pHeightValues[z1 * m_width + (x1 + 1)], weight1);
		float lin2 = linearInterpolation(m_pHeightValues[(z1 + 1) * m_width + x1], m_pHeightValues[(z1 + 1) * m_width + (x1 + 1)], weight1);
		float ret = linearInterpolation(lin1, lin2, weight2);
		return ret;
	}
	else if (x == (m_width - 1) || z == (m_height - 1))
	{
		return m_pHeightValues[(int)z * m_width + (int)x];
	}
	else
	{
		return 1.0f;
	}
}

float HeightMapLoader::getHeight(float x, float z)
{
	return getUnitHeight(x, z) * m_maxHeight;
}

vec3f HeightMapLoader::getNormal(int x, int z) const
{
	if (x < m_width && z < m_height && x >= 0 && z >= 0)
	{
		return m_pVertexNormals[x*m_width + z];
	}
	else
	{
		return vec3f(0, 1, 0);
	}
}

vec3f HeightMapLoader::getColor(int x, int z) const
{
	return m_pColors[z*m_width + x];
}

float HeightMapLoader::getScale() const
{
	return m_scale;
}

float HeightMapLoader::getMaxHeight() const
{
	return m_maxHeight;
}
