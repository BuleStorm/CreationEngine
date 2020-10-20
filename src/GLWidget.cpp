#include "GLWidget.h"
#include "Control.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

		window = glfwCreateWindow(Width, Height, "CreationEngine", pMonitor[select_screen], nullptr);
	}
	else
	{
		window = glfwCreateWindow(Width, Height, "CreationEngine", nullptr, nullptr);
	}
}
unsigned int loadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = 3;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	}

	stbi_image_free(data);
	return textureID;
}