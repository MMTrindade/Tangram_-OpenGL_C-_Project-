////////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// (c)2013-22 by Carlos Martinho
//
// INTRODUCES:
// GL PIPELINE, mglShader.hpp, mglConventions.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../mgl/mgl.hpp"
#include <array>
#include <vector>

///// MESH 

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

class Mesh {
public:

	void bind();
	void unbind();
	void draw();
	void initTriangle();
	void initSquare();
	void initParalelogram();
	void destroy();
	int indexTotal;
	GLfloat RGBA[4] = {0.0f ,0.0f,0.0f,0.0f };

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

	void draw(GLint ModelMatrixId , glm::mat4 ModelMatrix);
};


////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {

public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;

private:
  const GLuint POSITION = 0, COLOR = 1;
  GLuint VaoId, VboId[2];
  mgl::ShaderProgram *Shaders;
  GLint MatrixId;

  Mesh * triangle;
  Mesh * square;
  Mesh * paralelogram;
  //Mesh Square; 
  //Mesh Parelelogram;

  //Entity *Entities;

  Entity *triang1; 
  Entity *square1;
  Entity *paralelo1;
  
  void createShaderProgram();
  void createBufferObjects();
  void createBufferObjectSquare();
  void destroyBufferObjects();
  void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {

  Shaders = new mgl::ShaderProgram();
  Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addAttribute(mgl::COLOR_ATTRIBUTE, COLOR);
  Shaders->addUniform("Matrix");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs


void MyApp::createBufferObjects() {
	const Vertex Vertices[] = {
	{{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	{{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	{{0.50f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}} };

	const GLubyte Indices[] = { 0, 1, 2 };

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
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
                   GL_STATIC_DRAW);
    }
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(2, VboId);
}
void MyApp::createBufferObjectSquare() {
	
	const Vertex squareVert[] = {
	{{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},//0 left bot
	{{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},//1 right bot
	{{0.25f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},//2 left bot
	{{0.25f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},//2 left bot
	{{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},//1 right bot
	{{0.75f, 0.75f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } } };//3 right top

	const GLubyte squareI[] = { 0, 1, 2,4,5,6 };
	
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




void MyApp::destroyBufferObjects() {
  glBindVertexArray(VaoId);
  glDisableVertexAttribArray(POSITION);
  glDisableVertexAttribArray(COLOR);
  glDeleteVertexArrays(1, &VaoId);
  glBindVertexArray(0);

}

///////////////////////////////////////////////////////////////////////// Entity


void Mesh::initTriangle() {
	const Vertex Vertices[] = {
{{0.25f, 0.25f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},
{{0.75f, 0.25f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},
{{0.50f, 0.75f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]} } };

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
	{{0.25f, 0.25f, 0.0f, 1.0f},{RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//0 left bot
	{{0.75f, 0.25f, 0.0f, 1.0f},{RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//1 right bot
	{{0.25f, 0.75f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//2 left bot
	{{0.25f, 0.75f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//2 left bot
	{{0.75f, 0.25f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]}},//1 right bot
	{{0.75f, 0.75f, 0.0f, 1.0f}, {RGBA[0],RGBA[1],RGBA[2],RGBA[3]} } };//3 right top

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
	//     .25
	const Vertex Vertices[] = {
	{{0.25f, 0.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left bot A  0
	{{0.50f, 0.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// right bot A  1
	{{0.50f, 0.50f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// right top A  2
	{{0.50f, 0.50f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left top B   3
	{{0.50f, 0.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left bot B   4
	{{0.75f, 0.50f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},// right top B  5
	{{0.75f, 0.50f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},// right top C  6
	{{0.50f, 0.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left bot C   7
	{{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// right bot C  8
	{{0.75f, 0.50f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},// left top D   9
	{{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},// left bot D  10
	{{1.00f, 0.50f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},// right top D 11
	 };//3 right top

	const GLubyte Indexes[] = { 
		0, 1, 2,//A	
		3,4,5,  //B
		6,7,8,  //C
		9,10,11 //D
	};

	indexTotal = 12;

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

void Mesh::draw() {

	glDrawArrays(GL_TRIANGLES,0, indexTotal);

}

void Entity::draw( GLint ModelMatrixId, glm::mat4 ModelMatrix) {
	mesh->bind();
	Shader->bind();

	glUniformMatrix4fv(ModelMatrixId, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	mesh->draw();
	
	Shader->unbind();
	mesh->unbind();

}



////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 I = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
const glm::mat4 M = glm::translate(glm::vec3(-1.0f, 0.0f, 0.0f));
const glm::mat4 L = glm::translate(glm::vec3(-1.0f, -1.0f, 0.0f));

//const glm::mat4 test = glm::rotate(L, 0.0f,glm::vec3(0.0f,0.0f,0.0f) );
const glm::mat4 test = glm::scale(L, glm::vec3(1.5f, 1.5f, 1.5f));


void MyApp::drawScene() {

  // Drawing directly in clip space
	
	const glm::mat4 I = glm::translate(glm::vec3(0.0f, -1.0f, 0.0f));

	triang1->draw(MatrixId, I);
	
	const glm::mat4 S = glm::translate(glm::vec3(-1.0f, 0.0f, 0.0f));
	square1->draw(MatrixId, S);

	const glm::mat4 P = glm::translate(glm::vec3(-1.0f, -1.0f, 0.0f));
	paralelo1->draw(MatrixId, P);
	//glBindVertexArray(VaoId);
	//Shaders->bind();
	//triang1->mesh->bind();
	//triang1->Shader->bind();
	//const glm::mat4 I = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));
	//glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
	////glDrawArrays(GL_TRIANGLES,0, 6);
	//triang1->mesh->draw();
	//triang1->Shader->unbind();
	//triang1->mesh->bind();
	/*Shaders->unbind();
	glBindVertexArray(0);*/
//	

	//triang1->mesh->bind();
	
//	triang1->mesh->bind();
//	
//	triang1->Shader->bind();
//
//  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
//  triang1->mesh->draw();
//  
//  //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid *)0);
//
///*
//  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M));
//  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid *)0);
//
//  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(test));
//  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid *)0);*/
//
//  triang1->Shader->unbind();
//  triang1->mesh->unbind();
//  //glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
 // createBufferObjectSquare();
  //createShaderProgram();
 // 

	GLfloat Red [4] = { .9f, 0.0f, 0.0f, 1.0f };
	GLfloat Green[4] = { 0.0f, .9f, 0.0f, 1.0f };
	GLfloat Blue[4] = { 0.0f, 0.0f, 0.9f, 1.0f };

	triangle = new Mesh();
	triangle->SetColor(Red);
	triangle->initTriangle();

	triang1 = new Entity();
	createShaderProgram();
	triang1->mesh=triangle;
	triang1->Shader = Shaders;



	square = new Mesh();
	square->SetColor(Green);
	square->initSquare();

	square1 = new Entity();
	square1->mesh = square;
	square1->Shader = Shaders;

	paralelogram = new Mesh();

	paralelogram->initParalelogram();

	paralelo1 = new Entity();
	paralelo1->mesh = paralelogram;
	paralelo1->Shader = Shaders;



 //  std::vector<Vertex>  triangleVertex = {
	//{{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	//{{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	//{{0.50f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}} };
 //  std::vector<GLuint>  triangleIndices = { 0, 1, 2 };

 //  	 
 //  
 // /* int triangleLenght = sizeof(triangleVertex) / sizeof(triangleVertex[0]);*/
 // triangle = new Mesh(triangleVertex, triangleIndices);

 // triang1 = new Entity();


 // triangle->init();
 // createShaderProgram();
 // triang1->mesh=triangle;
 // triang1->Shader = Shaders;


 // std::vector<Vertex>  squareVertex = {
	//  {{0.0f, 0.707f, 0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } }, //0
 // { {0.0f, 0.0f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }, //1
 // { {0.707f, 0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } }, //2
 // { {0.707f, 0.0f, 0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } }, //2
 // { {0.707f, 0.707f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }, //3
 // { {0.0f, 0.707f, 0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } }}; //0
 // std::vector<GLuint>  squareIndices = { 0, 1, 2 };

 // square = new Mesh(squareVertex, squareIndices);
 // 
 // square1 = new Entity();

 // square->init();
 // square1->mesh = triangle;
 // square1->Shader = Shaders;

  //create 3 meshs 
  // create shader 
  //init meshs


  //create (for)  each entity 
		//entity =  new entity() 
		//entity -> shader  = Shader ; 
		//entity -> mesh =  triangle ;

}

void MyApp::windowCloseCallback(GLFWwindow *win) { 
	
	//Mesh destroy
	//entity destroy
	// shader destroy


	destroyBufferObjects(); 


}

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(600, 600, "Hello Modern 2D World", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
