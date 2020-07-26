#include "GLWidget.h"
#include "Control.h"

#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

void windowResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLWidget* GLWidget::getInstance()
{
	static GLWidget widget;
	return &widget;
}

GLWidget::GLWidget()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 初始化窗口
	setScreenFull(false);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	// 初始化 GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetFramebufferSizeCallback(window, windowResize);
	glfwSetCursorPosCallback(window, mouseMove);
	glfwSetMouseButtonCallback(window, mousePress);

	// 深度测试
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, Width, Height);
}

GLWidget::~GLWidget()
{
	glfwTerminate();
}

GLFWwindow* GLWidget::getWindow()
{
	return window;
}

void GLWidget::setScreenFull(bool is_full)
{
	if (is_full)
	{
		int monitorCount;
		GLFWmonitor** pMonitor = glfwGetMonitors(&monitorCount);

		int select_screen = 0;
		for (int i = 0; i < monitorCount; i++)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(pMonitor[i]);
			Width = mode->width;
			Height = mode->height;
			if (Width == 1920 && Height == 1080)
				select_screen = i;
		}

		window = glfwCreateWindow(Width, Height, "My World", pMonitor[select_screen], nullptr);
	}
	else
	{
		window = glfwCreateWindow(Width, Height, "My World", nullptr, nullptr);
	}
}
