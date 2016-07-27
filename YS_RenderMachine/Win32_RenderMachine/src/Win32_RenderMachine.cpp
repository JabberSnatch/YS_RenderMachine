#include <Windows.h>
#include <windowsx.h>

#include <iostream>
#include <memory>
#include <cassert>
#include <fstream>
#include <vector>

#include "glew-2.0.0/include/GL/glew.h"
#include "glew-2.0.0/include/GL/wglew.h"

#include "assimp-3.0/include/assimp/Importer.hpp"
#include "assimp-3.0/include/assimp/scene.h"
#include "assimp-3.0/include/assimp/anim.h"
#include "assimp-3.0/include/assimp/mesh.h"
#include "assimp-3.0/include/assimp/types.h"

// TODO:
// [ ] Model loading
// [ ] Basic scene display
// [ ] Shader loading (consider SPIR-V, subroutines)

// [ ] Proper alpha blending
// [ ] Shadow maps
// [ ] Normal maps
// [ ] Antialiasing
// [ ] Particles ?

#define YS_LOG_TO_FILE

#ifndef YS_LOG_TO_FILE
#define LOG(msg) std::cout << (msg) << std::endl
#else
std::fstream	log_file;
#define LOG(msg) log_file << (msg) << std::endl
#endif

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

	// TEST AREA
#ifdef YS_LOG_TO_FILE
	log_file.open("output", std::ios_base::out);
	log_file.clear();
#endif

	Assimp::Importer	main_importer;
	aiString			extensions;
	main_importer.GetExtensionList(extensions);
	std::cout << "Available extensions : " << extensions.C_Str() << std::endl;

	//std::string			path = "resource/SquallFFVIII/Squall.dae";
	std::string			path = "resource/Sora/Anti Sora.dae";
	assert(std::fstream(path).good());

	const aiScene* scene = main_importer.ReadFile(path, 0);
	if (!scene)
	{
		std::cout << main_importer.GetErrorString() << std::endl;
	}
	assert(scene);

	LOG("Mesh count : " + std::to_string(scene->mNumMeshes));
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		LOG("============================");

		aiMesh*			mesh = scene->mMeshes[i];
		LOG(std::to_string(i) + " : " + mesh->mName.C_Str());
		LOG("Poly count : " + std::to_string(mesh->mNumFaces));
		LOG("Vertex count : " + std::to_string(mesh->mNumVertices));
		LOG("Bones count : " + std::to_string(mesh->mNumBones));
		LOG("Material index : " + std::to_string(mesh->mMaterialIndex));
	}

	LOG("");
	LOG("Material count : " + std::to_string(scene->mNumMaterials));
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		LOG("============================");
		
		aiMaterial*		material = scene->mMaterials[i];
		aiString		mat_name;
		material->Get<aiString>(AI_MATKEY_NAME, &mat_name, nullptr);
		
		LOG(std::to_string(i) + " : " + mat_name.C_Str());
		LOG("Property count : " + std::to_string(material->mNumProperties));
		
		aiColor3D		color_value;
		aiString		string_value;
		int				int_value;

		material->Get<int>(AI_MATKEY_SHADING_MODEL, &int_value, nullptr);
		switch (int_value)
		{
		case aiShadingMode_Phong:
			LOG("Shading mode : phong");
			break;
		case aiShadingMode_Blinn:
			LOG("Shading mode : blinn_phong");
			break;
		default:
			LOG("Shading mode : " + std::to_string(int_value));
			break;
		}

		material->Get<aiColor3D>(AI_MATKEY_COLOR_DIFFUSE, &color_value, nullptr);
		LOG("Diffuse     : " + std::to_string(color_value.r) + "; " +
			std::to_string(color_value.g) + "; " +
			std::to_string(color_value.b));

		material->Get<aiColor3D>(AI_MATKEY_COLOR_SPECULAR, &color_value, nullptr);
		LOG("Specular    : " + std::to_string(color_value.r) + "; " +
			std::to_string(color_value.g) + "; " +
			std::to_string(color_value.b));

		material->Get<aiColor3D>(AI_MATKEY_COLOR_AMBIENT, &color_value, nullptr);
		LOG("Ambient     : " + std::to_string(color_value.r) + "; " +
			std::to_string(color_value.g) + "; " +
			std::to_string(color_value.b));

		material->Get<aiColor3D>(AI_MATKEY_COLOR_EMISSIVE, &color_value, nullptr);
		LOG("Emissive    : " + std::to_string(color_value.r) + "; " +
			std::to_string(color_value.g) + "; " +
			std::to_string(color_value.b));

		material->Get<aiColor3D>(AI_MATKEY_COLOR_TRANSPARENT, &color_value, nullptr);
		LOG("Transparent : " + std::to_string(color_value.r) + "; " +
			std::to_string(color_value.g) + "; " +
			std::to_string(color_value.b));

		LOG("Diffuse map count : " +
			std::to_string(material->GetTextureCount(aiTextureType_DIFFUSE)));
		material->GetTexture(aiTextureType_DIFFUSE, 0, &string_value);
		LOG(std::string("Diffuse map : ") + string_value.C_Str());
	}

	LOG("");
	LOG("Anim count : " + std::to_string(scene->mNumAnimations));
	for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
	{
		LOG("============================");

		aiAnimation*	anim = scene->mAnimations[i];
		LOG(std::to_string(i) + " : " + anim->mName.C_Str());
		LOG("Duration : " + std::to_string(anim->mDuration));
		LOG("Ticks per second : " + std::to_string(anim->mTicksPerSecond));
		LOG("Mesh channel count : " + std::to_string(anim->mNumMeshChannels));
		LOG("Channel count : " + std::to_string(anim->mNumChannels));
	}

	LOG("");
	LOG("Nodes : ");
	int						accum = 0;
	aiNode*					root = scene->mRootNode;
	std::vector<aiNode*>	history;
	history.push_back(root);
	while (history.size())
	{
		LOG("===========================");

		aiNode*				current_node = history.back();
		history.pop_back();

		LOG(std::to_string(accum) + " : " + current_node->mName.C_Str());
		if (current_node->mParent)
			LOG(std::string("Parent : ") + current_node->mParent->mName.C_Str());

		if (current_node->mNumMeshes)
		{
			LOG("Transformation : ");
			aiVector3D position, scaling;
			aiQuaternion rotation;
			current_node->mTransformation.Decompose(scaling, rotation, position);
			LOG("Position : " + std::to_string(position.x) + "; " +
				std::to_string(position.y) + "; " +
				std::to_string(position.z));
			LOG("Scaling  : " + std::to_string(scaling.x) + "; " +
				std::to_string(scaling.y) + "; " +
				std::to_string(scaling.z));
			LOG("Rotation : " + std::to_string(rotation.x) + "; " +
				std::to_string(rotation.y) + "; " +
				std::to_string(rotation.z) + "; " +
				std::to_string(rotation.w));
		}

		LOG("Mesh count : " + std::to_string(current_node->mNumMeshes));
		LOG("Child count : " + std::to_string(current_node->mNumChildren));
		for (int i = 0; i < current_node->mNumChildren; ++i)
		{
			LOG("Child " + std::to_string(i) + " : " + 
				current_node->mChildren[i]->mName.C_Str());
			history.push_back(current_node->mChildren[i]);
		}

		++accum;
	}

#ifdef YS_LOG_TO_FILE
	log_file.close();
#endif
	
	int i;
	std::cin >> i;
	return 0;
}


int
main(int _argc, char** _argv)
{
	return WinMain(GetModuleHandle(nullptr), nullptr, nullptr, SW_SHOW);
}