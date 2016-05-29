#ifndef GBUFFER_HPP
#define GBUFFER_HPP

#include "Rendering/openGL.hpp"

class GBuffer
{
public:
	GBuffer(int width, int height);
	~GBuffer();

	void BindGBuffer();
	void SetActiveTextures();

private:
	GLuint gBuffer;
	GLuint gPosition, gNormal, gColorSpec;
	GLuint rboDepth;

};

#endif //GBUFFER_HPP