#ifndef INTERFACE_H
#define INTERFACE_H

#include "Shader.h"
#include "GLWidget.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <string>

using namespace std;

// 界面类
class Interface
{
public:
	static Interface* getInstance();

	void init();
	// 画准心
	void draw();
	// 画FPS
	void drawDebug(int fps,int time);
	// 画文字
	void drawText(const char* text);
private:
	Interface();
	~Interface();
	Interface(const Interface&) = delete;
	Interface& operator = (const Interface&) = delete;

	unsigned int  VAO_C = 0, VBO_C = 0, EBO_C = 0;
	unsigned int  VAO_F = 0, VBO_F = 0;
	unsigned int textureC = 0, textureF = 0;

	Shader shaderC = Shader("./resource/Shader/ui/ui.vs", "./resource/Shader/ui/ui.fs");
	Shader shaderF = Shader("./resource/Shader/text/text.vs", "./resource/Shader/text/text.fs");

	struct Character 
	{
		GLuint textureID;
		glm::ivec2 size;
		glm::ivec2 offset;
		GLuint advance;
	};

	std::map<GLchar, Character> Characters;

	void initCross();
	void initFont();

	void drawCross();
	void drawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

#endif // !INTERFACE_H

