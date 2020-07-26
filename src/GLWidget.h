#ifndef INITGLWIDGET_H
#define INITGLWIDGET_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ´°Ìå¿í¸ß
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
#endif // INITGLWIDGET_H
