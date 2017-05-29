#pragma once
#include "model.hpp"
#include "objectLoader.hpp"

class Tree
{
public:
	/// loads the object and the billboard displaylist
	Tree(string fileName);
	/// empty
	~Tree();
	void drawTree();
	void drawBillBoard();

private:
	string m_fileName;
	
	float m_height;
	void getHeight(BoundingBox& boundingBox);

	float m_scale;
	/// vector of texture ID-s
	/// the last texture is the billboard image
	std::vector<GLuint> m_textures;
	GLuint loadTexture(string fileNames);

	void drawFace(Face& face) const;

	GLuint m_vertexVBO;
	GLuint m_vertexNormalVBO;
	GLuint m_vertexTextureVBO;
	void loadObjectDispList();

	GLuint m_billBoardDispList;
	void loadBillBoardDispList();
};