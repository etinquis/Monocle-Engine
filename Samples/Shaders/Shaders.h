#pragma once

#include <Monocle.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

using namespace Monocle;

namespace Monocle
{
	class ShaderAsset;
}

class ShaderScene : public Monocle::Scene
{
public:
	void Begin();
	void End();
	
	void Update();
	
	void Render();
private:
	Monocle::ShaderAsset *shader;
	
	GLuint vbo;
	GLuint vao;
	GLuint ptime;
};
