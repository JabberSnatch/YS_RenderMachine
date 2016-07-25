#include <Windows.h>
#include <windowsx.h>

#include <iostream>
#include <memory>

#include "glew-2.0.0/include/GL/glew.h"
#include "glew-2.0.0/include/GL/wglew.h"


static const wchar_t	kAppName[] = { L"RenderMachine" };
static const wchar_t	kWinName[] = { L"YS_RenderMachine" };


LRESULT WINAPI 
WinProc(HWND	_hWnd,
		UINT	_msg,
		WPARAM	_wParam,
		LPARAM	_lParam)
{
	switch (_msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(_hWnd, _msg, _wParam, _lParam);
}


int	WINAPI
WinMain(HINSTANCE	_hInstance,
		HINSTANCE	_hPrevInstance,
		LPSTR		_lpCmdLine,
		int			_nCmdShow)
{
	// NOTE: wnd_class is used to create and destroy the window.
	WNDCLASS	wnd_class = { 0 };
	HWND		h_wnd;

	const int win_width = 1280;
	const int win_height = 720;
	// INITIALIZE WINDOW
	{
		{
			wnd_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wnd_class.lpfnWndProc = WinProc;
			wnd_class.hInstance = _hInstance;
			wnd_class.lpszClassName = kAppName;
			
			RegisterClass(&wnd_class);
		}

		RECT	canvas_rect = { 0, 0, win_width, win_height };
		DWORD	style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		AdjustWindowRect(&canvas_rect, style, FALSE);

		h_wnd = CreateWindowEx(0, kAppName, kWinName, style,
							   CW_USEDEFAULT, CW_USEDEFAULT,
							   canvas_rect.right - canvas_rect.left,
							   canvas_rect.bottom - canvas_rect.top,
							   0, 0, _hInstance, 0);

		if (!h_wnd)
			std::cout << "Window creation failed." << std::endl;
	}

	// CREATE CONTEXT
	{
		HDC		device_context = GetDC(h_wnd);

		PIXELFORMATDESCRIPTOR pixel_format_desc = { 0 };
		pixel_format_desc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pixel_format_desc.nVersion = 1;
		pixel_format_desc.dwFlags =
			PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
		pixel_format_desc.iPixelType = PFD_TYPE_RGBA;
		pixel_format_desc.cColorBits = 32;
		pixel_format_desc.cDepthBits = 24;
		pixel_format_desc.cStencilBits = 8;
		pixel_format_desc.iLayerType = PFD_MAIN_PLANE;

		int pixel_format =
			ChoosePixelFormat(device_context, &pixel_format_desc);
		SetPixelFormat(device_context, pixel_format, &pixel_format_desc);

		HGLRC	gl_render_context = wglCreateContext(device_context);
		wglMakeCurrent(device_context, gl_render_context);

		if (glewInit() != GLEW_OK)
			std::cout << "glew failed to initialize." << std::endl;

		int gl_attributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 5,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		if (wglewIsSupported("WGL_ARB_create_context") == 1)
		{
			HGLRC	intermediate = gl_render_context;
			gl_render_context =
				wglCreateContextAttribsARB(device_context, 0, gl_attributes);
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(intermediate);
			wglMakeCurrent(device_context, gl_render_context);
		}

		std::cout << "GL init complete : " << std::endl;
		std::cout << "OpenGL version : " << glGetString(GL_VERSION) << std::endl;
		std::cout << "Manufacturer : " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Drivers : " << glGetString(GL_RENDERER) << std::endl;
	}


	return 0;
}


int
main(int _argc, char** _argv)
{
	return WinMain(GetModuleHandle(nullptr), nullptr, nullptr, SW_SHOW);
}