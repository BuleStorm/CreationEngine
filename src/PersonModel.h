#ifndef PERSONMODEL_H
#define PERSONMODEL_H

#include "Shader.h"
#include "TakeCubeModel.h"
#include "CreateWorld.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opencv2/opencv.hpp>

// 方块六面
constexpr int COUNT = 6;

// 人物模型类
class PersonModel
{
public:
	PersonModel(const char* textureStr);
	~PersonModel();

	// TakeCubeModel 类初始化
	static void takeCubeInit();
	void init();
	void draw(const glm::vec3& pos, bool isMy);
	// 人物放置消除动画
	void placeAnimtaion(bool &placeAnimation);
	// 人物手持切换动画
	void takeCubeAnimation(CubeType nowTakeCube);

	// 仰角
	float eyeRadians = 0;
	// 朝向
	float bodyRadians = 0;

	// 动画状态
	enum class Animation
	{
		GO,
		Return,
		End,
	};

	// 放置消除动画
	Animation placeAnimation = Animation::End;
	// 放置进度
	float placeValue = 0;
	// 手持切换动画
	Animation takeAnimation = Animation::End;
	// 切换进度
	float takeValue = 0;

	// 现在手持方块
	CubeType nowTakeCube = CubeType::MapLand;
	// 更换手持方块
	CubeType newTakeCube = CubeType::MapLand;

private:
	const char* textureStr;
	cv::Mat img;

	// 人物纹理
	unsigned int headTexture[COUNT]{ 0 };
	unsigned int bodyTexture[COUNT]{ 0 };
	unsigned int  armTexture[COUNT]{ 0 };
	unsigned int  legTexture[COUNT]{ 0 };

	static Shader shader;
	static unsigned int headVAO[COUNT];
	static unsigned int headVBO[COUNT];
	static unsigned int bodyVAO[COUNT];
	static unsigned int bodyVBO[COUNT];
	static unsigned int  armVAO[COUNT];
	static unsigned int  armVBO[COUNT];
	static unsigned int  legVAO[COUNT];
	static unsigned int  legVBO[COUNT];

	// 手持方块
	static TakeCubeModel takeCube1;
	static TakeCubeModel takeCube2;
	static TakeCubeModel takeCube3;
	static TakeCubeModel takeCube4;
	static TakeCubeModel takeCube5;
	static TakeCubeModel takeCube6;
	static TakeCubeModel takeCube7;
	static TakeCubeModel takeCube8;
	static TakeCubeModel takeCube9;

	static glm::mat4 projection;
	static glm::mat4 view;

	// 上次人物位置
	glm::vec3 oldPos;
	// 手臂摆动角度
	float limbRadians = 0.0f;
	// 手臂摆动方向
	bool isLimbDir = false;	

	// 第一人称手持方块绘制
	void firstPersonDrawTakeCube();

	// 第三人称人物绘制
	void thirdPersonDrawPerson(const glm::vec3& pos);
	// 第三人称手持方块绘制
	void thirdPersonDrawTakeCube(const glm::vec3& pos);

	// 放置进度改变
	void placeValueChange();
	// 切换进度改变
	void takeValueChange();

	// 渲染人物组成面
	void drawArrays(unsigned int VAO, unsigned int texture);
	// 跑步动画
	void runAnimation(const glm::vec3& pos);
	// 加载人物纹理
	unsigned int loadTexture(int rect, int i);
	// 绑定人物组成纹理
	void bindTexture(unsigned int *VAO, unsigned int *painterVBO_H, 
			const float (*vertex)[48], unsigned int *texture, int rect);
};


#endif // !PERSONMODEL_H