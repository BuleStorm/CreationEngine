#include "Interface.h"
#include "GLWidget.h"

#include <ft2build.h>
#include FT_FREETYPE_H

static const GLfloat firstPersonVertices[] =
{
	 0.03f,  0.04f, 0.0f,  1.0f, 1.0f,
	 0.03f, -0.04f, 0.0f,  1.0f, 0.0f,
	-0.03f, -0.04f, 0.0f,  0.0f, 0.0f,
	-0.03f,  0.04f, 0.0f,  0.0f, 1.0f
};

static const GLuint indicesV[] =
{
	0, 1, 3,
	1, 2, 3
};


Interface::~Interface()
{
	glDeleteVertexArrays(1, &VAO_C);
	glDeleteBuffers(1, &VBO_C);
	glDeleteBuffers(1, &EBO_C);
}

Interface* Interface::getInstance()
{
	static Interface interface;
	return &interface;
}

void Interface::init()
{
    initCross();
    initFont();
}

void Interface::draw()
{
    drawCross();

    drawText("Open source URL: https://github.com/BuleStorm", Width / 4, 40, 0.5f, glm::vec3(0.22, 0.37f, 0.80f));
}

void Interface::drawFPS(int fps)
{
    string fpsText = "FPS:";
    fpsText.append(to_string(fps));
    drawText(fpsText, Width - 110, Height - 40, 0.5f, glm::vec3(0.22, 0.37f, 0.80f));
}

void Interface::drawText(const char* text)
{
    drawText(text, Width / 2 - 150, Height / 2, 1.0f, glm::vec3(0.22, 0.37f, 0.80f));
}

Interface::Interface()
{
}

void Interface::initCross()
{
    glGenVertexArrays(1, &VAO_C);
    glGenBuffers(1, &VBO_C);
    glGenBuffers(1, &EBO_C);

    glBindVertexArray(VAO_C);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstPersonVertices), firstPersonVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_C);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesV), indicesV, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    textureC = loadTexture("./Textures/Ð±Ê®×Ö.png");
}

void Interface::initFont()
{
    shaderF.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(Width), 0.0f, static_cast<GLfloat>(Height));
    glUniformMatrix4fv(glGetUniformLocation(shaderF.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "Other/arial.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        glGenTextures(1, &textureF);
        glBindTexture(GL_TEXTURE_2D, textureF);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
            face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = { textureF,glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),face->glyph->advance.x };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO_F);
    glGenBuffers(1, &VBO_F);
    glBindVertexArray(VAO_F);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_F);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Interface::drawCross()
{
    shaderC.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureC);
    glUniform1i(glGetUniformLocation(shaderC.ID, "texture1"), 0);

    glBindVertexArray(VAO_C);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Interface::drawText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    shaderF.use();
    glUniform3f(glGetUniformLocation(shaderF.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureF);
    glBindVertexArray(VAO_F);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.offset.x * scale;
        GLfloat ypos = y - (ch.size.y - ch.offset.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        GLfloat vertices[6][4] = 
        {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_F);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6)* scale;
    }
}

unsigned int Interface::loadTexture(const char* path)
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	stbi_image_free(data);
	return textureID;
}
