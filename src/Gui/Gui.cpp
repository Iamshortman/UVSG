#include "Gui.hpp"

#include "Rendering/openGL.hpp"

Gui::Gui()
{

}

Gui::~Gui()
{

}

void Gui::renderGui(int windowWidth, int windowHeight)
{
	float width = 150;
	float height = 509;

	float midPointx = windowWidth / 2.0f;
	float midPointy = windowHeight / 2.0f;

	glMatrixMode(GL_MODELVIEW);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowWidth, windowHeight, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glUseProgram(0);

	glPushMatrix();

	//glTranslatef(midPointx, midPointy, 0.0f);

	float scale = 2.0f;
	glScalef(scale, scale, scale);

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glTexCoord2f(1, 0);

	glVertex2f(width, 0);
	glTexCoord2f(1, 1);

	glVertex2f(width, height);
	glTexCoord2f(0, 1);

	glVertex2f(0, height);
	glTexCoord2f(0, 0);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Gui::AddComponent(std::string name, GuiComponent* component)
{
	//TODO
}