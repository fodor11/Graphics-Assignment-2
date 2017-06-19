#include "../include/object.hpp"
#include <gl/glm/gtc/matrix_transform.hpp>

GLuint Tree::loadTexture(string fileName)
{
	TextureLoader *txtrLoader = new TextureLoader();
	txtrLoader->loadMipMappedTexture(fileName);
	m_textures.push_back(txtrLoader->textureID());
	txtrLoader->~TextureLoader();
	return m_textures.back();
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

	const std::vector<glm::vec3>& vertices = objLoader->getVertices();
	const std::vector<glm::vec3>& vertNormals = objLoader->getVertexNormals();
	const std::vector<glm::vec2>& textureCoords = objLoader->getTextureCoords();
	const std::map<std::string, std::vector<Face>>& faceLists = objLoader->getFaceLists();

	getHeight(objLoader->getBoundingBox());

	//temporary
	Face face;
	int v_index, vt_index, vn_index;

	m_mModel = glm::rotate(m_mModel, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	m_scale = 0.05; 
	m_mModel = glm::scale(m_mModel, glm::vec3(m_scale, m_scale, m_scale));
	
	for (auto map = faceLists.begin(); map != faceLists.end(); map++)
	{
		GLuint textureId = loadTexture(map->first);
		m_iTextureIDs.push_back(textureId);
		std::vector<glm::vec3> verticesVBO;
		std::vector<glm::vec3> vertexNormalsVBO;
		std::vector<glm::vec2> textureCoordsVBO;
		const std::vector<Face>& curr_faceList = map->second;
		for (int i = 0; i < curr_faceList.size(); i++)
		{
			face = curr_faceList[i];
			//1 vertex
			for (int j = 0; j < 3; j++)
			{
				//get indices
				v_index = face[j][0] - 1;
				vt_index = face[j][1] - 1;
				vn_index = face[j][2] - 1;

				//get values
				verticesVBO.push_back(vertices[v_index]);
				textureCoordsVBO.push_back(textureCoords[vt_index]);
				vertexNormalsVBO.push_back(vertNormals[vn_index]);
			}
		}
		GLuint treeVAO;
		glGenVertexArrays(1, &treeVAO);
		glBindVertexArray(treeVAO);
		m_iTreeVAOs.push_back(treeVAO);

		// create and bind the VBO for vertices
		glGenBuffers(1, &m_iTreeVBO);
		m_vVBOs.push_back(m_iTreeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_iTreeVBO);
		glBufferData(GL_ARRAY_BUFFER, verticesVBO.size() * sizeof(glm::vec3), &verticesVBO.front(), GL_STATIC_DRAW);
		// connect the xyz to the "vert" attribute of the vertex shader
		glEnableVertexAttribArray(m_pProgram->attrib("vert"));
		glVertexAttribPointer(m_pProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

		// create and bind the VBO for texture coordinates
		glGenBuffers(1, &m_iTreeVBO);
		m_vVBOs.push_back(m_iTreeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_iTreeVBO);
		glBufferData(GL_ARRAY_BUFFER, textureCoordsVBO.size() * sizeof(glm::vec2), &textureCoordsVBO.front(), GL_STATIC_DRAW);
		// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
		glEnableVertexAttribArray(m_pProgram->attrib("vertTexCoord"));
		glVertexAttribPointer(m_pProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, NULL);

		// create and bind the VBO for normals
		glGenBuffers(1, &m_iTreeVBO);
		m_vVBOs.push_back(m_iTreeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_iTreeVBO);
		glBufferData(GL_ARRAY_BUFFER, vertexNormalsVBO.size() * sizeof(glm::vec3), &vertexNormalsVBO.front(), GL_STATIC_DRAW);
		// connect the normal to the "vertNorm" attribute of the vertex shader
		glEnableVertexAttribArray(m_pProgram->attrib("vertNorm"));
		glVertexAttribPointer(m_pProgram->attrib("vertNorm"), 3, GL_FLOAT, GL_TRUE, 0, NULL);

		glBindVertexArray(0);
		m_vNumberOfVertices.push_back(verticesVBO.size());
	}	
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


Tree::Tree(string fileName, tdogl::Program* shaderProgram)
{
	m_fileName = fileName;
	m_pProgram = shaderProgram;
	loadObjectDispList();
	loadBillBoardDispList();
}

Tree::~Tree()
{
	glDeleteBuffers(m_vVBOs.size(), &m_vVBOs.front());
}

void Tree::drawTree()
{
	glm::vec3 specular( 0.f, 0.f, 0.f);
	glm::vec3 ambient( 0.1f, 0.1f, 0.1f);
	glm::vec3 diffuse( 0.3f, 0.3f, 0.3f);
	for (int i = 0; i < m_iTreeVAOs.size(); i++)
	{
		// bind the program (the shaders)
		m_pProgram->use();

		// set the "model" uniform in the vertex shader
		glm::mat4 tmpModel = glm::translate(glm::mat4(), m_vTranslationVector);
		tmpModel *= m_mModel;
		m_pProgram->setUniform("model", tmpModel);

		// set colors
		m_pProgram->setUniform("ambientColor", ambient);
		m_pProgram->setUniform("diffuseColor", diffuse);
		m_pProgram->setUniform("specularColor", specular);

		// set the "normalMatrix" uniform in the vertex shader
		m_pProgram->setUniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(tmpModel))));

		// bind the texture and set the "tex" uniform in the fragment shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_iTextureIDs[i]);
		m_pProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0

		// bind the VAO 
		glBindVertexArray(m_iTreeVAOs[i]);
		
		glDrawArrays(GL_TRIANGLES, 0, m_vNumberOfVertices[i]);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_pProgram->stopUsing();
	}
}

void Tree::drawBillBoard()
{
}

void Tree::translate(glm::vec3 vector)
{
	m_vTranslationVector = vector;
}
