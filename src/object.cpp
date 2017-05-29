#include "../include/object.hpp"

GLuint Tree::loadTexture(string fileName)
{
	TextureLoader *txtrLoader = new TextureLoader();
	txtrLoader->loadMipMappedTexture(fileName);
	m_textures.push_back(txtrLoader->textureID());
	txtrLoader->~TextureLoader();
	return m_textures.back();
}

void Tree::drawFace(Face & face) const
{

}

void Tree::getHeight(BoundingBox& boundingBox)
{
	m_height = boundingBox.getXdistance();
	if (m_height < boundingBox.getYdistance())
	{
		m_height = boundingBox.getYdistance();
	}
	if (m_height < boundingBox.getZdistance())
	{
		m_height = boundingBox.getZdistance();
	}
}
void Tree::loadObjectDispList()
{
	ObjectLoader* objLoader = new ObjectLoader();
	objLoader->loadObjFile(m_fileName + ".obj");

	const std::vector<vec3f>& vertices = objLoader->getVertices();
	const std::vector<vec3f>& vertNormals = objLoader->getVertexNormals();
	const std::vector<std::pair<float, float>>& textureCoords = objLoader->getTextureCoords();
	const std::map<std::string, std::vector<Face>>& faceLists = objLoader->getFaceLists();

	getHeight(objLoader->getBoundingBox());

	//temporary
	Face face;
	int v_index, vt_index, vn_index;
	vec3f vertex, vertexNormal;
	float u, v;

	m_vertexVBO = glGenLists(1);
	//glGenBuffers(1, &m_vertexVBO);

	glNewList(m_vertexVBO, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	//tree material
	GLfloat specular[] = { 0.f, 0.f, 0.f, 1.0f };
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

		glRotatef(-90, 1, 0, 0);
		m_scale = 0.05;
		glScalef(m_scale, m_scale, m_scale);
		for (auto map = faceLists.begin(); map != faceLists.end(); map++)
		{
			//if (map->first.find("ned") != std::string::npos)
			//{
			//	glDepthMask(GL_FALSE);
			//}
			GLuint textureId = loadTexture(map->first);
			glBindTexture(GL_TEXTURE_2D, textureId);
			//std::cout << "bound texture: " << map->first << endl;
			const std::vector<Face>& curr_faceList = map->second;
			for (int i = 0; i < curr_faceList.size(); i++)
			{
				face = curr_faceList[i];
				glBegin(GL_TRIANGLES);
				for (int j = 0; j < 3; j++)
				{
					//get indexes
					v_index = face[j][0] - 1;
					vt_index = face[j][1] - 1;
					vn_index = face[j][2] - 1;

					//get values
					vertex = vertices[v_index];
					vertexNormal = vertNormals[vn_index];
					u = textureCoords[vt_index].first;
					v = textureCoords[vt_index].second;

					glTexCoord2f(u, 1 - v);
					glNormal3f(vertexNormal[0], vertexNormal[1], vertexNormal[2]);
					glVertex3f(vertex[0], vertex[1], vertex[2]);
				}
				glEnd();
			}
			//if (map->first.find("ned") != std::string::npos)
			//{
			//	glDepthMask(GL_TRUE);
			//}

		}	
	//glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEndList();

	objLoader->~ObjectLoader();
}

void Tree::loadBillBoardDispList()
{
	loadTexture(m_fileName + ".png");
	float height = m_height * m_scale;

	m_billBoardDispList = glGenLists(1);
	glNewList(m_billBoardDispList, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_textures.back());

	GLfloat specular[] = { 0.f, 0.f, 0.f, 1.0f };
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat diffuse[] = { 0.13f, 0.13f, 0.13f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

	//glDepthMask(GL_FALSE);
	glTranslatef(- height / 2, 0.f, 0.f);	//to put the middle of the picture to the right place
	for (int i = 0; i < 2; i++)
	{
		glTranslatef(height / 2, 0.f, 0.f);
		glRotatef(90.f * i, 0.f, 1.f, 0.f);
		glTranslatef(- height / 2, 0.f, 0.f);
		glBegin(GL_TRIANGLE_STRIP);

		glTexCoord2f(0.f, 1.f);
		glVertex3f(0.f, 0.f, 0.f);

		glTexCoord2f(1.f, 1.f);
		glVertex3f(height, 0.f, 0.f);

		glTexCoord2f(0.f, 0.f);
		glVertex3f(0.f, height, 0.f);

		glTexCoord2f(1.f, 0.f);
		glVertex3f(height, height, 0.f);

		glEnd();
	}
	//glDepthMask(GL_TRUE);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEndList();
}


Tree::Tree(string fileName)
{
	m_fileName = fileName;
	loadObjectDispList();
	loadBillBoardDispList();
}

Tree::~Tree()
{
}

void Tree::drawTree()
{
	glCallList(m_vertexVBO);
}

void Tree::drawBillBoard()
{
	glCallList(m_billBoardDispList);
}
