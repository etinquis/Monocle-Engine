#ifdef MONOCLE_OPENGL_ES

#include "../Graphics.h"
#include "../Debug.h"
#include "../Platform.h"
#include "../TextureAsset.h"
#include "../Color.h"

#ifdef MONOCLE_ANDROID
	#define ANDROID_POWERED
	#include <errno.h>
	#include <android/log.h>
	#include "Android/AndroidPlatform.h"
	#undef LOGI
	#undef LOGW
	#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "MonocleGfx", __VA_ARGS__))
	#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "MonocleGfx", __VA_ARGS__))
#else
	#define LOGI(...) ;
	#define LOGW(...) ;
#endif

#include <EGL/egl.h>
#include <GLES/gl.h>


int engine_init_display(Monocle::AndroidPlatform *platform);
namespace Monocle
{
	pthread_cond_t DisplayInitCond = PTHREAD_COND_INITIALIZER;
	pthread_mutex_t DisplayInitMutex = PTHREAD_MUTEX_INITIALIZER;

	Graphics *Graphics::instance = NULL;

	static void checkGlError(const char* op) {
		for (GLint error = glGetError(); error; error = glGetError()) {
			LOGI("after %s() glError (0x%x)\n", op, error);
		}
	}


	Graphics::Graphics()
	{
		instance = this;
	}

	void Graphics::Init()
	{
		Debug::Log("Graphics::Init");

		pthread_mutex_lock(&DisplayInitMutex);
		pthread_cond_wait(&DisplayInitCond, &DisplayInitMutex);
		pthread_mutex_unlock(&DisplayInitMutex);

		engine_init_display(AndroidPlatform::instance);
		
		glDisable(GL_LIGHTING);
		checkGlError("glDisable(LIGHTING)");

		glEnable(GL_TEXTURE_2D);
		checkGlError("glEnable(TEXTURE_2D)");
		glEnable(GL_BLEND);
		checkGlError("glEnable(BLEND)");
		glBlendFunc(GL_ONE, GL_SRC_COLOR);
		checkGlError("glBlendFunc(BLEND)");

		glShadeModel(GL_SMOOTH);	
		checkGlError("glShadeModel");

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepthf(1.0f);
		glDisable(GL_DEPTH_TEST);
		checkGlError("glDisable(DEPTH_TEST)");
		//glDepthFunc(GL_LEQUAL); 
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		checkGlError("glHint(PERSPECTIVE)");

		Set2D(800,600);
	}

	Color Graphics::GetBackgroundColor()
	{
		GLfloat *clearColor = new GLfloat[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
		Color color(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		delete[] clearColor;
		return color;
	}

	void Graphics::SetBackgroundColor(const Color &color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	bool Graphics::SetResolution(int w, int h, int bits, bool full)
	{
		//can't change screen
		return false;
	}

	void Graphics::ResolutionMatrix()
	{
		glLoadIdentity();
		glScalef(instance->resolutionScale.x, instance->resolutionScale.y, 0.0f);
		//glTranslatef(instance->screenCenter.x, instance->screenCenter.y, 0.0f);
	}
	
	void Graphics::DefaultMatrix()
	{
		glLoadIdentity();
		glScalef(instance->resolutionScale.x, instance->resolutionScale.y, 0.0f);
		glTranslatef(instance->screenCenter.x, instance->screenCenter.y, 0.0f);
	}
	
	void Graphics::RenderPathMesh(const std::vector<Node*> &nodes, int cells, float size, bool flipX, bool flipY)
	{
		
	}

	Vector2 Graphics::GetMatrixPosition()
	{
		float m[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, m);
		float x = m[12];
		float y = m[13];
		//float z = m[14];
		return Vector2(x, y);
	}
	
	const Vector2 &Graphics::GetResolutionScale()
	{
		return instance->resolutionScale;
	}

	void Graphics::SetBlend(BlendType blend)
	{
		if (blend != instance->currentBlend)
		{
			switch (blend)
			{
			case BLEND_ALPHA:
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				break;
			case BLEND_ADDITIVE:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case BLEND_MULTIPLY:
				glBlendFunc(GL_ZERO, GL_SRC_COLOR);
				break;
			}
			instance->currentBlend = blend;
		}
	}

	void Graphics::Set2D(int virtualWidth, int virtualHeight)
	{
		instance->virtualWidth = virtualWidth;
		instance->virtualHeight = virtualHeight;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		checkGlError("before glOrthof");
		glOrthof(0.0f, Platform::GetWidth(), Platform::GetHeight(), 0.0f, -1.0, 1.0);
		checkGlError("glOrthof");
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		instance->resolutionScale = Vector2(float(Platform::GetWidth())/virtualWidth, float(Platform::GetHeight())/virtualHeight);
		Debug::Log("Set2d: resScale:");
		Debug::Log(instance->resolutionScale);
		Debug::Log("Window:WidthHeight");
		Debug::Log(Platform::GetWidth());
		Debug::Log(Platform::GetHeight());
		//Debug::log("Set2D: resScale: (%f, %f)\n window (%d, %d)\n", instance->resolutionScale.x, instance->resolutionScale.y, Platform::GetWidth(), Platform::GetHeight());
		instance->screenCenter.x = virtualWidth/2;
		instance->screenCenter.y = virtualHeight/2;
	}

	static void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
	{
		GLfloat xmin, xmax, ymin, ymax;

		ymax = zNear * (GLfloat)tan(fovy * M_PI / 180);
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;

		glFrustumx((GLfixed)(xmin * 65536), (GLfixed)(xmax * 65536),
				(GLfixed)(ymin * 65536), (GLfixed)(ymax * 65536),
				(GLfixed)(zNear * 65536), (GLfixed)(zFar * 65536));
		LOGI("Perspective(fov %f, aspect %f) %f %f %f %f %f %f",fovy, aspect, xmin, xmax, ymin, ymax, zNear, zFar);
		checkGlError("glFrustumx");
	}

	void Graphics::Resize(int width, int height)
	{
		if (height==0)										// Prevent A Divide By Zero By
		{
			height=1;										// Making Height Equal One
		}

		glViewport(0,0,width,height);						// Reset The Current Viewport
		checkGlError("glViewPort");

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window

		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.01f,100.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix

		Set2D(instance->virtualWidth,instance->virtualHeight);
	}

	void Graphics::Translate(float x, float y, float z)
	{
		glTranslatef(x, y, z);
	}
	
	Vector2 Graphics::GetScreenCenter()
	{
		return instance->screenCenter;
	}
	
	void Graphics::IdentityMatrix()
	{
		glLoadIdentity();
	}

	void Graphics::Translate(const Vector2 &pos)
	{
		glTranslatef(pos.x, pos.y, 0.0f);
	}

	void Graphics::PushMatrix()
	{
		glPushMatrix();
	}

	void Graphics::PopMatrix()
	{
		glPopMatrix();
	}

	void Graphics::Rotate(float ax, float ay, float az, float r)
	{
		glRotatef(ax, ay, az, r);
	}

	void Graphics::RenderTriangle(float size)
	{
		float halfSize = size*0.5f;
		GLfloat vertices[] = {0.0f, halfSize, //TOP
								-halfSize,-halfSize, // Bottom Left
								 halfSize,-halfSize,}; // Bottom Right

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	/*
	void Graphics::RenderQuad(float size)
	{
		float halfSize = size*0.5f;

		glBegin(GL_QUADS);
			glVertex3f(-halfSize, -halfSize, 0.0f);
			glTexCoord2f(1, 1);
			glVertex3f(halfSize, -halfSize, 0.0f);
			glTexCoord2f(1, 0);
			glVertex3f(halfSize, halfSize, 0.0f);
			glTexCoord2f(0, 0);
			glVertex3f(-halfSize, halfSize, 0.0f);
			glTexCoord2f(0, 1);
		glEnd();
	}
	*/

	void Graphics::Scale(const Vector2 &scale)
	{
		glScalef(scale.x, scale.y, 1.0f);
	}

	void Graphics::RenderLine(const Vector2 &pos1, const Vector2 &pos2)
	{
		GLfloat vertices[] = {
								pos1.x, pos1.y,
								pos2.x, pos2.y
							};
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_LINES, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Graphics::RenderLineRect(float x, float y, float w, float h)
	{
		float hw = w*0.5f;
		float hh = h*0.5f;

		GLfloat vertices[] = {
								x-hw, y-hh, 
								x+hw, y-hh, 

								x+hw, y-hh, 
								x+hw, y+hh, 

								x+hw, y+hh, 
								x-hw, y+hh, 

								x-hw, y+hh, 
								x-hw, y-hh,
							};

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_LINES, 0, 8);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void Graphics::SetColor(const Color &color)
	{
		glColor4f(color.r, color.g, color.b, color.a);
	}

	void Graphics::RenderQuad(float width, float height, const Vector2 &textureOffset, const Vector2 &textureScale, const Vector2 &position)
	{
		float halfWidth = width*0.5f;
		float halfHeight = height*0.5f;

		GLfloat vertices[] = {  -halfWidth + position.x, -halfHeight + position.y,
								halfWidth + position.x, -halfHeight + position.y,
								halfWidth + position.x, halfHeight + position.y,
								-halfWidth + position.x, halfHeight + position.y
							};
		GLfloat texcords[] = {	textureOffset.x, textureOffset.y,
								textureOffset.x + textureScale.x, textureOffset.y,
								textureOffset.x + textureScale.x, textureOffset.y + textureScale.y,
								textureOffset.x, textureOffset.y + textureScale.y
							};
		GLshort indicies[] = {	0, 1, 2, 
								0, 2, 3};

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glVertexPointer(2, GL_FLOAT, 0, vertices);

		glTexCoordPointer(2, GL_FLOAT, 0, texcords);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indicies);

		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	void Graphics::BeginFrame()
	{
		if(bgReset)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix

		//glTranslatef(cameraPosition.x - screenCenter.x, cameraPosition.y - screenCenter.y, 0.0f);
		//glScalef(resolutionScale.x, resolutionScale.y, 0.0f);
	}

	void Graphics::EndFrame()
	{
	}

	void Graphics::ShowBuffer()
	{
		Platform::ShowBuffer();
	}

	void Graphics::BindTexture(TextureAsset* textureAsset)
	{
		if (textureAsset != NULL)
		{
			//Debug::Log("bound texture");
			//Debug::Log((int)textureAsset->texID);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureAsset->texID);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, NULL);
		}
	}
}

#endif

