#ifdef MONOCLE_ANDROID

#include "../Debug.h"
#include "../Game.h"
#include "../Graphics.h"

#include "AndroidPlatform.h"
#include <time.h>

// keyboard code based on SDL http://www.libsdl.org/

#ifndef VK_0
#define VK_0	'0'
#define VK_1	'1'
#define VK_2	'2'
#define VK_3	'3'
#define VK_4	'4'
#define VK_5	'5'
#define VK_6	'6'
#define VK_7	'7'
#define VK_8	'8'
#define VK_9	'9'
#define VK_A	'A'
#define VK_B	'B'
#define VK_C	'C'
#define VK_D	'D'
#define VK_E	'E'
#define VK_F	'F'
#define VK_G	'G'
#define VK_H	'H'
#define VK_I	'I'
#define VK_J	'J'
#define VK_K	'K'
#define VK_L	'L'
#define VK_M	'M'
#define VK_N	'N'
#define VK_O	'O'
#define VK_P	'P'
#define VK_Q	'Q'
#define VK_R	'R'
#define VK_S	'S'
#define VK_T	'T'
#define VK_U	'U'
#define VK_V	'V'
#define VK_W	'W'
#define VK_X	'X'
#define VK_Y	'Y'
#define VK_Z	'Z'
#endif /* VK_0 */

#define VK_SEMICOLON		0xBA
#define VK_EQUALS		0xBB
#define VK_COMMA			0xBC
#define VK_MINUS			0xBD
#define VK_PERIOD		0xBE
#define VK_SLASH		0xBF
#define VK_GRAVE		0xC0
#define VK_LBRACKET		0xDB
#define VK_BACKSLASH	0xDC
#define VK_RBRACKET		0xDD
#define VK_APOSTROPHE	0xDE
#define VK_BACKTICK		0xDF
#define VK_OEM_102		0xE2

#define REPEATED_KEYMASK	(1<<30)
#define EXTENDED_KEYMASK	(1<<24)


namespace Monocle
{	
	AndroidPlatform *AndroidPlatform::instance = NULL;

	void AndroidPlatform::GetDesktopSize(int *width, int *height)
	{
		AndroidPlatform *platform = instance;
		if (platform->display != NULL) {
			eglQuerySurface(platform->display, platform->surface, EGL_WIDTH, width);
			eglQuerySurface(platform->display, platform->surface, EGL_HEIGHT, height);
		}
	}

	void AndroidPlatform::KillPlatformWindow()								// Properly Kill The Window
	{
		ANativeActivity_finish(instance->state->activity);
	}

	Platform *Platform::instance = NULL;

	bool Platform::keys[KEY_MAX];
	bool Platform::mouseButtons[3];
	Vector2 Platform::mousePosition;
	int Platform::mouseScroll = 0;

	Platform::Platform()
	{
		AndroidPlatform::instance = new AndroidPlatform();
		instance = this;
		AndroidPlatform::instance->platform = this;

		AndroidPlatform::instance->display = NULL;
		AndroidPlatform::instance->surface = NULL;

		for (int i = 0; i < KEY_MAX; i++)
		{
			keys[i] = false;
			localKeymap[i] = KEY_UNDEFINED;
		}

	}

	void Platform::Init(const std::string &name, int w, int h, int bits, bool fullscreen)
	{
		//AndroidPlatform::instance->CreatePlatformWindow("Title", w, h, bits, fullscreen);
		//TEMP: hack
		//width = w;
		//height = h;
	}
	
	std::string Platform::GetDefaultContentPath()
	{
		return "";
	}
	
	bool Platform::IsActive() {
		return AndroidPlatform::instance->active;
	}

	int Graphics::GetVirtualWidth()
	{
		return instance->virtualWidth;
	}

	int Graphics::GetVirtualHeight()
	{
		return instance->virtualHeight;
	}

	void Platform::Update()
	{
		/*
		printf("mouse (%d, %d)\n", mouse.x, mouse.y);
		printf("desktop (%d, %d)\n", desktopWidth, desktopHeight);
		printf("mousePosition (%f, %f)\n", mousePosition.x, mousePosition.y);
		printf("mouseButtons: %d, %d, %d", mouseButtons[0], mouseButtons[1], mouseButtons[2]);
		*/
	}

	void Platform::SetMouseButton(int button, bool on)
	{
		mouseButtons[button] = on;
	}

	long Platform::GetMilliseconds()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec*1000. + tv.tv_usec/1000.;
	}

	bool Platform::IsKeyPressed(KeyCode keyCode)
	{
		return instance->keys[(int)keyCode];
	}

	void Platform::ShowBuffer()
	{
		if (!AndroidPlatform::instance->active) {
			return;
		}
		glFlush();
		if (AndroidPlatform::instance->display != NULL && AndroidPlatform::instance->surface != NULL) {
			unsigned int rc1 = eglWaitGL();
			if (rc1 == EGL_FALSE) {
				int e = eglGetError();
				LOGW("eglWaitGL error %d (0x%x)",e,e);
				return;
			}
			unsigned int rc = eglSwapBuffers(AndroidPlatform::instance->display, AndroidPlatform::instance->surface);
			if (rc == EGL_FALSE) {
				int e = eglGetError();
				LOGW("eglSwapBuffers error %d (0x%x)",e, e);
			}
		}
	}

	int Platform::GetWidth()
	{
		return instance->width;
	}

	int Platform::GetHeight()
	{
		return instance->height;
	}

	void Platform::WindowSizeChanged(int w, int h)
	{
		LOGI("Changing screen size to %dx%d", w, h);
		instance->width = w;
		instance->height = h;
		Graphics::Resize(w, h);
	}

	void Platform::SetLocalKey(int key, bool on)
	{
		KeyCode keyCode = (KeyCode)instance->localKeymap[key];
		if (keyCode == KEY_UNDEFINED)
		{
			Debug::Log("Received undefined KeyCode");
			Debug::Log(key);
		}
		else
		{
			instance->keys[instance->localKeymap[key]] = on;			
		}
	}
}

#endif
