#ifndef TEXTUREPOOL_HPP
#define TEXTUREPOOL_HPP

#include <map>
#include <string>
#include <iostream>

#include "OpenGL_Include.hpp"

using std::string;

//Loads and stores all textures using in game.
class TexturePool
{

public:
	TexturePool();
	~TexturePool();
	void loadTexture(string fileLoc);
	void unloadTexture(string fileLoc);
	bool bindTexture(string fileLoc);

private:
	std::map<string, GLuint> textures;
};


#endif //TEXTUREPOOL_HPP