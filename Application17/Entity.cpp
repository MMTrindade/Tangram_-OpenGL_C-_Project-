
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "mgl/mgl.hpp"
#include "mgl/mglShader.hpp"

#include <array>
#include <vector>

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

class Mesh {
public:

	void bind();
	void unbind();
	void draw();
	void  initAnyMesh(); 
	void initTriangle();
	void initSquare();
	void initParalelogram();
	void destroy();
	int indexTotal;
	GLfloat RGBA[4] = { 0.0f ,0.0f,0.0f,0.0f };

	void SetColor(GLfloat Color[4]) {

		RGBA[0] = Color[0];
		RGBA[1] = Color[1];
		RGBA[2] = Color[2];
		RGBA[3] = Color[3];
	}

	//std::vector< Vertex> vertices;
	//std::vector< GLuint>   indexes;
	//Mesh(const std::vector<Vertex> &ObjectVertices, const std::vector<GLuint>  indices):vertices(ObjectVertices),indexes(indices) {}
	GLuint VaoId, VboId[2];
private:

	const GLuint POSITION = 0, COLOR = 1;


};

class Entity {
public:
	Mesh * mesh;
	mgl::ShaderProgram *Shader;

	GLint MatrixId;

	void draw(GLint ModelMatrixId, glm::mat4 ModelMatrix);
};


void Mesh::initTriangle() {
	const Vertex Vertices[] = {
{{-.5f, -.25f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},
{{.5f, -.25f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},
{{0.0f, .25f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}} };

	const GLubyte Indices[] = { 0, 1, 2 };
	indexTotal = 3;
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * (vertices.size()), &vertices[0], GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(POSITION);
			glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLOR);
			glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(GLvoid *)sizeof(Vertices[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* (indexes.size()), &indexes[0],GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
				GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, VboId);

}

void Mesh::initSquare() {



	const Vertex squareVert[] = {
	{{-.5f, -.5f, 0.0f, 1.0f},{RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//0 left bot
	{{0.5f, -.5f, 0.0f, 1.0f},{RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//1 right bot
	{{-.5f, 0.5f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//2 left top
	{{-.5f, 0.5f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//2 left top
	{{0.5f, -.5f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//1 right bot
	{{0.5f, 0.5f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]} } };//3 right top

	const GLubyte squareI[] = { 0, 1, 2,3,4,5 };

	indexTotal = 6;

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{

			glBufferData(GL_ARRAY_BUFFER, sizeof(squareVert), squareVert, GL_STATIC_DRAW);
			glEnableVertexAttribArray(POSITION);
			glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLOR);
			glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(GLvoid *)sizeof(squareVert[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareI), squareI,
				GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, VboId);
}


void Mesh::initParalelogram() {
	//      / 2|3--5/6|9  11/   
	//     /   | B /  |    /
	//    / A  |  /  C| D /
	//   0/___1|4/7__8|10/
	//     .5
	//const Vertex Vertices[] = {
	//{{-.75f, -.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left bot A  0
	//{{-.25f, -.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// right bot A  1
	//{{-.25f, .25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// right top A  2
	//{{-.25f, .25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left top B   3
	//{{-.25f, -.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left bot B   4
	//{{.25f, .25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},// right top B  5
	//{{.25f, .25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},// right top C  6
	//{{-.25f, -.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left bot C   7
	//{{.25f, -.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// right bot C  8
	//{{.25f, .25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},// left top D   9
	//{{.25f, -.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left bot D  10
	//{{.75f, .25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},// right top D 11
	//};//3 right top

	//const GLubyte Indexes[] = {
	//	0, 1, 2,//A	
	//	3,4,5,  //B
	//	6,7,8,  //C
	//	9,10,11 //D
	//};

	//improved less Triangles 
	//      2/\3--11/   
	//      /  \ B / 
	//     / A  \ / 
	//   0/____10/8
	//      .75


	const Vertex Vertices[] = {
		{{-.75f, -.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left bot A  0
		{{.25f, -.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// right bot A  10
		{{-.25f, .25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// right top A  2
		{{-.25f, .25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left top B   3
		{{.25f, -.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},//  bot B  8
		{{.75f, .25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}}// right top B 11
	};
	const GLubyte Indexes[] = {
		0,1,2,
		3,4,5
	};
	indexTotal = 6;

	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{

			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(POSITION);
			glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLOR);
			glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				(GLvoid *)sizeof(Vertices[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indexes), Indexes,
				GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, VboId);
}

void Mesh::destroy() {
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(POSITION);
	glDisableVertexAttribArray(COLOR);
	glDeleteVertexArrays(1, &VaoId);
	glBindVertexArray(0);
}

void Mesh::bind() {
	glBindVertexArray(VaoId);
}
void Mesh::unbind() {
	glBindVertexArray(0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////

void Mesh::draw() {

	glDrawArrays(GL_TRIANGLES, 0, indexTotal);

}


void Entity::draw(GLint ModelMatrixId, glm::mat4 ModelMatrix) {
	mesh->bind();
	Shader->bind();

	glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	mesh->draw();

	Shader->unbind();
	mesh->unbind();

}

/*
Mesh::initAnyMesh

there was an attempt to make it so any mesh could be created by constructing a Mesh Object with a std::vector <Vertex> vertices
and the accompanying std::vector<unassigned int>Indexes indices 
and create the appropriate Buffers, with the deadline incoming and with a needle in a strawpile type of error we've decided to 
implement a more hard coded version.
we know the issue came from memory counting and pointing 

The version bellow WAS NOT the furthest we were able to go on this, but unfortunately it was lost and this version was recreated to 
somewhat our attempt and logic
this problem consumed 80-90% of our dev time and we werent able even with the help of Professor Luis, Professor Ricardo, and other faculty
to locate the actual error.

Although this error gave quite a bit of clarity on the inner works of the creation of buffers and overall inner works of the system we werent 
able to display what we ve learn in this exercise.
*/

//void Mesh::initAnyMesh() {
//
//	indexTotal = 6;
//
//	glGenVertexArrays(1, &VaoId);
//	glBindVertexArray(VaoId);
//	{
//		glGenBuffers(2, VboId);
//
//		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
//		{
//
//			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//			glEnableVertexAttribArray(POSITION);
//			glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//			glEnableVertexAttribArray(COLOR);
//			glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
//				(GLvoid *)sizeof(vertices[0].XYZW));
//		}
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
//		{
//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
//				&indices[0], 
//				GL_STATIC_DRAW);
//		}
//	}
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glDeleteBuffers(2, VboId);
//
//}



