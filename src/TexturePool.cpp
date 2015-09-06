#include "TexturePool.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TexturePool::TexturePool()
{

}

void TexturePool::loadTexture(string fileLoc)
{
	//if the map contains the file already
	if (textures.count(fileLoc))
	{
		return;
	}

	int width, height, numComponents;
	unsigned char* data = stbi_load((fileLoc).c_str(), &width, &height, &numComponents, 4);

	if (data == NULL)
		std::cerr << "Can't Load the given texture: " << fileLoc << std::endl;

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	//Add the info to the map
	textures.insert(std::pair<string, GLuint>(fileLoc, textureId));
}

bool TexturePool::bindTexture(string fileLoc)
{
	//if the map contains the file already
	if (textures.count(fileLoc))
	{
		glBindTexture(GL_TEXTURE_2D, textures.at(fileLoc));

		return true;
	}


	return false;
}

void TexturePool::unloadTexture(string fileLoc)
{

}


TexturePool::~TexturePool()
{

}