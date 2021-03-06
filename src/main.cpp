#include "OpenGL\renderer.h"
#include "OpenGL\camera.h"

#include "game.h"

using std::vector;

// function declarations ------------------------------------------------------
void framebuffer_size_callback(GLFWwindow*, int, int);
void key_callback(GLFWwindow*, int, int, int, int);

// global variables -----------------------------------------------------------
const int screenWidth = 600, screenHeight = 800;

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

std::unique_ptr<Game> spaceInvaders;

int main() {
	// glfw: initialize and configure --------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw: window creation and setup -------------------------------------------
	GLFWwindow* window = glfwCreateWindow
		(screenWidth,  screenHeight, "OpenGL Invaders", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// glfw: setup
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	//// glfw: icon (need an img parser to handle stb linking weirdness)
	//GLFWimage images[1];
	//int imgWidth, imgHeight, nrChannels;
	//unsigned char* data
	//	= stbi_load("icon.ico", &imgWidth, &imgHeight, &nrChannels, 0);
	//if (data == nullptr) {
	//	const char* failLog = stbi_failure_reason();
	//	std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_ICON"
	//		<< "\n" << failLog << std::endl;
	//}
	//stbi_image_free(data);

	//images[0].width = imgWidth;
	//images[0].height = imgHeight;
	//images[0].pixels = data;

	//glfwSetWindowIcon(window, 1, images);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// initialization before rendering -------------------------------------------
	spaceInvaders.reset(new Game(screenWidth, screenHeight));

	// render loop (happens every frame) -----------------------------------------
	while (!glfwWindowShouldClose(window)) {
		// -> frame time tracker
		Renderer::FrameTimeTracker();

		// -> rendering commands and configuration
		spaceInvaders->ProcessInput(Renderer::GetDeltaTime());
		spaceInvaders->Update(Renderer::GetDeltaTime());
		Renderer::RenderConfig(1.0f, 1.0f, 1.0f, 1.0f);

		// ---> space configurations and rendering
		spaceInvaders->Render();

		// -> check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			spaceInvaders->keys[key] = true;
		else if (action == GLFW_RELEASE) {
			spaceInvaders->keys[key] = false;
			spaceInvaders->keysProcessed[key] = false;
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}