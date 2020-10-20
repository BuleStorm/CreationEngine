#ifndef PLANTMODEL_H
#define PLANTMODEL_H

#include "Shader.h"

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 代码逻辑方式参照 CubeModel 类

// 植物模型类
class PlantModel
{
public:
	PlantModel(const char* textureStr);
	~PlantModel();

	static void drawStart();

	void init();
	void draw();
	void push(int x, int y, int z);
	void clear();

private:
	static Shader shader;
	static glm::mat4 projection;
	static glm::mat4 view;

	const char* textureStr;

	unsigned int VAO = 0, VBO = 0, EBO = 0;
	unsigned int texture = 0;

	int painterCount = 50000;
	unsigned int painterVBO = 0;
	std::vector<glm::mat4> painterVec;
};

#endif // !PLANTMODEL_H
