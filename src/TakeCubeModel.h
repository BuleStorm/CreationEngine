#ifndef TAKECUBEMODEL_H
#define TAKECUBEMODEL_H

#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// OpenGL 基本渲染方式

// 手持方块类
class TakeCubeModel
{
public:
	TakeCubeModel(const char* textureStr);
	~TakeCubeModel();

	void init();
	void firstPersonDraw(float eyeRadians, float bodyRadians, float limbRadians, float placeValue, float takeValue);
	void thirdPersonDraw(const glm::vec3& pos, float eyeRadians, float bodyRadians, float limbRadians, float placeValue, float takeValue);

private:
	const char* textureStr;

	unsigned int texture = 0;

	static Shader shader;
	// 第一人称数据
	static unsigned int firstPersonVAO;
	static unsigned int firstPersonVBO;

	// 第三人称数据
	static unsigned int thirdPersonVAO;
	static unsigned int thirdPersonVBO;

	static glm::mat4 projection;
	static glm::mat4 view;
};

#endif // !TAKECUBEMODEL_H