#ifndef GBUFFER_HPP
#define GBUFFER_HPP

#include "Rendering/openGL.hpp"

class GBuffer
{
public:
	GBuffer(int width, int height);
	~GBuffer();

private:
	GLuint gBuffer;
	GLuint gPosition, gNormal, gColorSpec;

};

#endif //GBUFFER_HPP