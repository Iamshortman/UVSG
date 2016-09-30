#ifndef GBUFFER_HPP
#define GBUFFER_HPP

#include "Rendering/OpenGL_Include.hpp"

class GBuffer
{
public:
	GBuffer(int width, int height, bool Multisample = false);
	~GBuffer();

	void BindGBuffer();
	void SetActiveTextures();

	GLuint gBuffer;

private:
	GLuint gColor, gPosition, gNormal;
	GLuint rboDepth;

};

#endif //GBUFFER_HPP