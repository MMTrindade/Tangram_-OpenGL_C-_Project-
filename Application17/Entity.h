
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
