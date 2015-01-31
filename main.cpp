#include <Windows.h>
#include "Application.h"
#include "OpenGLWindow.h"

#include <GL\glew.h>

#include <string>
#include <vector>

const char* vshaderSrc =
"#version 400\n"
"layout(location = 0) in vec4 inPosition;"
"layout(location = 1) in vec4 inNormal;"
"layout(location = 2) in vec4 inColor;"
"out vec4 color;"
"void main () {"
"  color = inColor;"
"  gl_Position = inPosition;"
"}";

const char* fshaderSrc =
"#version 400\n"
"in vec4 color;"
"out vec4 fragmentColor;"
"void main () {"
"  fragmentColor = color;"
"}";

struct Vec4f
{
	explicit Vec4f(GLfloat initX = 0.0f, GLfloat initY = 0.0f, GLfloat initZ = 0.0f, GLfloat initW = 1.0f) : x(initX), y(initY), z(initZ), w(initW) {
	}
	Vec4f(const Vec4f& ref):x(ref.x), y(ref.y), z(ref.z), w(ref.w) {
	}

	Vec4f& operator =(const Vec4f& ref) {
		x = ref.x;
		y = ref.y;
		z = ref.z;
		w = ref.w;
		return *this;
	}

	bool operator ==(const Vec4f& ref) const {
		return x == ref.x && y == ref.y && z == ref.z && w == ref.w;
	}

	bool operator !=(const Vec4f& ref) const {
		return x != ref.x || y != ref.y || z != ref.z || w != ref.w;
	}

	GLfloat x, y, z, w;
};

struct Vec4d
{
	explicit Vec4d(GLdouble initX = 0.0, GLdouble initY = 0.0, GLdouble initZ = 0.0, GLdouble initW = 1.0) : x(initX), y(initY), z(initZ), w(initW) {
	}
	Vec4d(const Vec4d& ref) :x(ref.x), y(ref.y), z(ref.z), w(ref.w) {
	}

	Vec4d& operator =(const Vec4d& ref) {
		x = ref.x;
		y = ref.y;
		z = ref.z;
		w = ref.w;
		return *this;
	}

	bool operator ==(const Vec4d& ref) const {
		return x == ref.x && y == ref.y && z == ref.z && w == ref.w;
	}

	bool operator !=(const Vec4d& ref) const {
		return x != ref.x || y != ref.y || z != ref.z || w != ref.w;
	}

	GLdouble x, y, z, w;
};

struct Index {
	explicit Index(GLuint initA, GLuint initB, GLuint initC): a(initA), b(initB), c(initC) {
	}
	Index(const Index& ref): a(ref.a), b(ref.b), c(ref.c) {
	}

	Index& operator =(const Index& ref) {
		a = ref.a;
		b = ref.b;
		c = ref.c;
		return *this;
	}

	bool operator ==(const Index& ref) const {
		return a == ref.a && b == ref.b && c == ref.c;
	}

	bool operator !=(const Index& ref) const {
		return a != ref.a || b != ref.b || c != ref.c;
	}

	GLuint a, b, c;
};

struct Vertex {
	explicit Vertex(const Vec4d& initPosition = Vec4d(), const Vec4d& initNormal = Vec4d(), const Vec4d& initColor = Vec4d()) :position(initPosition), normal(initNormal), color(initColor) {
	}
	Vertex(const Vertex& ref):position(ref.position), normal(ref.position), color(ref.color) {
	}

	Vertex& operator =(const Vertex& ref) {
		this->position = ref.position;
		this->normal = ref.normal;
		this->color = ref.color;
		return *this;
	}

	Vec4d position, normal, color;
};

class WorkbenchWindow : public OpenGLWindow
{
	std::string glVender;
	std::string glRenderer;
	std::string glVersion;

	std::vector<Vertex> vertexBuffer;
	std::vector<Index> indexBuffer;

	GLuint vao, vbo, ibo, vshader, gshader, fshader, program;
public:
	explicit WorkbenchWindow(Window* initParent = 0) : OpenGLWindow(initParent)
	{
	}
	~WorkbenchWindow()
	{
	}
protected:
	void initialize()
	{
		glewInit();

		vertexBuffer.push_back(Vertex(Vec4d(0.0, 0.5, 0.0), Vec4d(), Vec4d(1.0, 0.0, 0.0)));
		vertexBuffer.push_back(Vertex(Vec4d(0.5, -0.5, 0.0), Vec4d(), Vec4d(0.0, 1.0, 0.0)));
		vertexBuffer.push_back(Vertex(Vec4d(-0.5, -0.5, 0.0), Vec4d(), Vec4d(0.0, 0.0, 1.0)));

		indexBuffer.push_back(Index(0, 1, 2));

		glVender = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		glRenderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
		glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

		GLenum error = glGetError();

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);
		
		error = glGetError();

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size()*sizeof(Vertex), vertexBuffer.data(), GL_STATIC_DRAW);

		error = glGetError();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size()*sizeof(Index), indexBuffer.data(), GL_STATIC_DRAW);

		error = glGetError();

		glBindVertexArray(vao);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glVertexAttribPointer(0, 4, GL_DOUBLE, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, position)));
		glVertexAttribPointer(1, 4, GL_DOUBLE, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
		glVertexAttribPointer(2, 4, GL_DOUBLE, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, color)));

		error = glGetError();

		vshader = glCreateShader(GL_VERTEX_SHADER);
		fshader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vshader, 1, &vshaderSrc, NULL);
		glCompileShader(vshader);

		error = glGetError();

		glShaderSource(fshader, 1, &fshaderSrc, NULL);
		glCompileShader(fshader);

		error = glGetError();

		program = glCreateProgram();
		glAttachShader(program, vshader);
		glAttachShader(program, fshader);
		glLinkProgram(program);

		error = glGetError();

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_DEPTH_TEST);
	}

	void render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program);

		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexBuffer.size()*3, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));

		GLenum error = glGetError();
	}
};

int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	) {
	Application application;
	WorkbenchWindow window;
	window.show();
	return application.execute();
}
