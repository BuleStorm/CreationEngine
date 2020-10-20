#ifndef INITGLWIDGET_H
#define INITGLWIDGET_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 窗体宽高
static float Width = 1000;
static float Height = 560;


class GLWidget
{
public:
	static GLWidget* getInstance();
	GLFWwindow* getWindow();
	void setScreenFull(bool is_full);
	~GLWidget();
private:
	GLWidget();
	GLWidget(const GLWidget&) = delete;
	GLWidget& operator = (const GLWidget&) = delete;

	GLFWwindow* window = nullptr;
};

unsigned int loadTexture(const char* path);
#endif // INITGLWIDGET_H
