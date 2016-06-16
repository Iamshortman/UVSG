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
	GLuint gPosition, gNormal, gColorSpec;
	GLuint rboDepth;

};

#endif //GBUFFER_HPP