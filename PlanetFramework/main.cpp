#include "stdafx.h"

#include "Scene.h"

//**************************************
//Functions for debugging
//**************************************
static void sdl_die(const char * message)
{
	fprintf(stderr, "%s: %s\n", message, SDL_GetError());
	exit(2);
}
static void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	(void)source; (void)type; (void)id;
	(void)severity; (void)length; (void)userParam;
	std::cout << std::endl;
	fprintf(stderr, "%s\n", message);
	if (severity == GL_DEBUG_SEVERITY_HIGH)
	{
		fprintf(stderr, "Aborting...\n");
		abort();
	}
	std::cout << std::endl;
}
void SetDebuggingOptions()
{
	//notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Enable run-time memory leak check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	typedef HRESULT(__stdcall *fPtr)(const IID&, void**);

	//_CrtSetBreakAlloc(249);
#endif
}

//**************************************
//Main
//**************************************
int main(int argc, char *argv[])
{
	UNREFERENCED_PARAMETER(argv);
	UNREFERENCED_PARAMETER(argc);

	//Catch memory leaks etc
	SetDebuggingOptions();

	//Initialize SDL OpenGL and GLAD
	//*******************************

	//SDL init
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		sdl_die("Couldn't initialize SDL");
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);

	//request opengl context
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);

	//Buffers
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Request a debug context.
	#if defined(DEBUG) | defined(_DEBUG)
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	#endif
	
	//Create window
	Settings* pSettings = Settings::GetInstance();//Initialize Game Settings
	if (pSettings->Window.Fullscreen)
	{
		pSettings->Window.pWindow = SDL_CreateWindow(pSettings->Window.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pSettings->Window.Width, pSettings->Window.Height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else
	{
		pSettings->Window.pWindow = SDL_CreateWindow(pSettings->Window.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, pSettings->Window.Width, pSettings->Window.Height, SDL_WINDOW_OPENGL);
	}
	if (pSettings->Window.pWindow == NULL)sdl_die("Couldn't set video mode");
	
	//OpenGL context creation
	SDL_GLContext context = SDL_GL_CreateContext(pSettings->Window.pWindow);
	if (context == NULL)sdl_die("Failed to create OpenGL context");
	// Use v-sync
	SDL_GL_SetSwapInterval(pSettings->Window.VSyncEnabled);
	
	//Bind OpenGL
	std::cout << "OpenGL loaded" << std::endl << std::endl;
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	std::cout << "Vendor: \t" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer:\t" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version:\t" << glGetString(GL_VERSION) << std::endl << std::endl;

	//Let OpenGL Log errors
	#ifdef _DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(openglCallbackFunction, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
	#endif




	//INIT
	//***************

	Context *pContext = Context::GetInstance(); //Initialize scene context
	InputManager* pInput = InputManager::GetInstance();//init input manager
	pInput->Init();

	//Make a scene :D
	Scene *pScene = new Scene();
	pScene->Init();
	

	glEnable(GL_DEPTH_TEST);
	//Main Loop
	while (true)
	{
		//UPDATE
		//**********

		//user input
		pInput->UpdateEvents();
		if (pInput->IsExitRequested())break;

		//scene update
		pScene->Update();

		//RENDER
		//******

		//Clear the previous content
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		//Let scene draw. 
		//custom swapchain with framebuffers will be handled here
		pScene->Draw();

		//Swap front and back buffer
		SDL_GL_SwapWindow(pSettings->Window.pWindow);
	}
	
	SafeDelete(pScene);

	pInput->DestroyInstance();
	pSettings->DestroyInstance();
	pContext->DestroyInstance();

	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}
