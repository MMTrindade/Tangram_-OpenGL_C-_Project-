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

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../mgl/mgl.hpp"
#include "../Entity.h"
#include <array>
#include <vector>

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
  GLint ModelMatrixId;
  GLint ModelMatrixId2;
  GLint ModelMatrixId3;
  GLint ModelMatrixId4;

  Mesh * triangle;
  Mesh * square;
  Mesh * paralelogram;

  Entity *triang1; 
  Entity *triang2; 
  Entity *triang3;
  Entity *square1;
  Entity *paralelo1;
  
  void createShaderProgram();
  void createBufferObjects();
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
  ModelMatrixId2 = Shaders->Uniforms["ModelMatrix2"].index;
  ModelMatrixId3 = Shaders->Uniforms["ModelMatrix3"].index;
  ModelMatrixId4 = Shaders->Uniforms["ModelMatrix4"].index;
  ModelMatrixId = Shaders->Uniforms[mgl::MODEL_MATRIX].index;
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


void MyApp::destroyBufferObjects() {
  glBindVertexArray(VaoId);
  glDisableVertexAttribArray(POSITION);
  glDisableVertexAttribArray(COLOR);
  glDeleteVertexArrays(1, &VaoId);
  glBindVertexArray(0);

}

///////////////////////////////////////////////////////////////////////// Transformations


//Model Matrix for triangle1 : First scale, then rotate, then translate
//bot right BIG triangle 
const glm::mat4 MatrixT1 = ((glm::translate(glm::vec3(-.0f, -.69f, 0.0f))) *
(glm::rotate(2.35619f, glm::vec3(0.0f, 0.0f, 1.0f))) * (glm::scale(glm::vec3(1.07f))));
//Model Matrix for triangle2: First scale, then rotate, then translate
//Top left "beak" triangle
const glm::mat4 MatrixT2 = ((glm::translate(glm::vec3(-.41f, 0.6f, 0.0f)) *
(glm::rotate(-2.35619f, glm::vec3(0.0f, 0.0f, 1.0f))))) * (glm::scale(glm::vec3(0.5f)));
//Model Matrix for triangle3: First scale, then rotate, then translate
//Bot Left triangle
const glm::mat4 MatrixT3 = ((glm::translate(glm::vec3(-.4f,-.5f, 0.0f))) *
(glm::rotate(1.5707f, glm::vec3(0.0f, 0.0f, 1.0f)))) * (glm::scale(glm::vec3(0.75f)));

//45 º 0.7853981634f  90º 1.5707f  135º 2.35619f

//Model Matrix for square: First scale , then rotate, then translate
//
const glm::mat4 MatrixSqr = ((glm::translate(glm::vec3(-.3f, 0.08f, 0.0f)) *
(glm::rotate(0.7853981634f, glm::vec3(0.0f, 0.0f, 1.0f))))) * (glm::scale(glm::vec3(0.35f)));

//Model Matrix for paralellogram: First scale, then rotate, then translate
const glm::mat4 MatrixPara = ((glm::translate(glm::vec3(-.17f, 0.50f, 0.0f)) *
(glm::rotate(1.5707f, glm::vec3(0.0f, 0.0f, 1.0f))))) * (glm::scale(glm::vec3(0.5f)));



const glm::mat4 simpleTranslate = glm::translate(glm::vec3(0.0f, -1.0f, 0.0f));



////////////////////////////////////////////////////////////////////////// SCENE


void MyApp::drawScene() {

  // Drawing directly in clip space

	triang1->draw(ModelMatrixId, MatrixT1);
	triang2->draw(ModelMatrixId, MatrixT2);
	triang3->draw(ModelMatrixId, MatrixT3);

	
	square1->draw(ModelMatrixId2, MatrixSqr);
	
	paralelo1->draw(ModelMatrixId3,MatrixPara);
	
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
 // createBufferObjectSquare();
  //createShaderProgram();
 // 

	GLfloat Red [4] = { .9f, 0.0f, 0.0f, 1.0f };
	GLfloat Green[4] = { 0.0f, .9f, 0.0f, 1.0f };
	GLfloat Blue[4] = { 0.0f, 0.0f, 0.9f, 1.0f };
	//set up Triangle Mesh

	triangle = new Mesh();
	triangle->SetColor(Red);
	triangle->initTriangle();
	createShaderProgram();
	//create Triangles Entities
	triang1 = new Entity();
	triang1->mesh=triangle;
	triang1->Shader = Shaders;
	
	triang2 = new Entity();
	triang2->mesh = triangle;
	triang2->Shader = Shaders;

	triang3 = new Entity();
	triang3->mesh = triangle;
	triang3->Shader = Shaders;

	//set Up Square Mesh 
	square = new Mesh();
	square->SetColor(Green);
	square->initSquare();
	//create Square Entities
	square1 = new Entity();
	square1->mesh = square;
	square1->Shader = Shaders;

	//create Paralelogram Mesh
	//will be rainbow colour because paralelogram is fancy
	paralelogram = new Mesh();
	paralelogram->initParalelogram();
	//set up Paralelogram Entities
	paralelo1 = new Entity();
	paralelo1->mesh = paralelogram;
	paralelo1->Shader = Shaders;



	/*  If we didnt had the issue with the Passsing of std:: vector and using creating the buffers  we would have 
		a similar method to this to Start our Entity : 
		- define vertex vector
		- define Indexes 
		- create New Mesh (vertex[],indexes[]) 
		- init/create Buffer Objects for this Mesh
		- create new Entity ()
		- assing Entity->mesh to Mesh created before
		- create a Shader program
		- assign Entity->Shader to Shader (or a individual Shader no need this time) 
		- 
	*/

 //  std::vector<Vertex>  triangleVertex = {
	//{{0.25f, 0.25f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
	//{{0.75f, 0.25f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
	//{{0.50f, 0.75f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}} };
 //  std::vector<GLuint>  triangleIndices = { 0, 1, 2 };

 //  	 
 //  
 // triangle = new Mesh(triangleVertex, triangleIndices);

 // triang1 = new Entity();


 // triangle->init();
 // createShaderProgram();
 // triang1->mesh=triangle;
 // triang1->Shader = Shaders;


  //create (for)  each entity 


}

void MyApp::windowCloseCallback(GLFWwindow *win) { 
	
	//Mesh destroy
	//entity destroy
	// shader destroy
	triangle->destroy();
	square->destroy();
	paralelogram->destroy();

	delete triang1, triang2, triang3, square1, paralelo1;
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
