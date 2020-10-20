#ifndef CUBEMODEL_H
#define CUBEMODEL_H

#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

// 方块模型类
class CubeModel
{
public:
	CubeModel(const char* textureStrH, const char* textureStrV);
	~CubeModel();
	
	// 开始集中绘制
	static void drawStart();

	// OpenGL渲染初始化数据
	void init();
	// OpenGL Instantiate渲染方块
	void draw();
	// 相机可观察到的方块加入渲染集合
	void push(int x, int y, int z);
	// 渲染集合清空
	void clear();

private:

	static Shader shader;
	static glm::mat4 projection;
	static glm::mat4 view;

	// 垂直纹理
	const char* textureStrH;
	// 水平纹理
	const char* textureStrV;

	// 垂直
	unsigned int VAO_V = 0, VBO_V = 0, EBO_V = 0;
	// 水平
	unsigned int VAO_H = 0, VBO_H = 0, EBO_H = 0;
	// 纹理
	unsigned int textureH = 0, textureV = 0;
	// 需要渲染的个数
	int painterCount = 50000;
	// 顶点缓冲区对象
	unsigned int painterVBO_H = 0, painterVBO_V = 0;
	// OpenGL Instantiate 渲染集合
	std::vector<glm::mat4> painterVec;
};

#endif // !CUBEMODEL_H