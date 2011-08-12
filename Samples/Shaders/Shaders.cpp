#include "Shaders.h"

#include <Shader.h>

void ShaderScene::Begin()
{
	shader = Monocle::Assets::RequestShader("test.vert", "test.frag");

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	Monocle::Vector3 verts[8] = 
		{
			Monocle::Vector3(-.5,-.5,0),
			Monocle::Vector3(-.5,.5,0),
			Monocle::Vector3(.5,.5,0),
			Monocle::Vector3(.5,-.5,0)
		};
	int idx[36];
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
	
	GLuint ppos = glGetAttribLocation(shader->shaderProgram, "vertex");
	glEnableVertexAttribArray(ppos);
	glVertexAttribPointer(ppos, 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL + (0));
}

void ShaderScene::Update()
{
	if(Monocle::Input::IsKeyPressed(Monocle::KEY_Z))
	{
		shader->Reload();
	}
}

void ShaderScene::End()
{
	
}

void ShaderScene::Render()
{
	Scene::Render();
	shader->Use();
	
	std::cout << Monocle::timeSinceStart << std::endl;
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glDrawArrays(GL_QUADS, 0, 8);
	
	ShaderAsset::UseNone();
}
