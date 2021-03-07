#include "common.h"
#include "Image.h"
#include "Player.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

Image map;
Point map_pos;
Point starting_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};
bool reach_exit = false;

struct InputState
{
  bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
  GLfloat lastX = 400, lastY = 300; //исходное положение мыши
  bool firstMouse = true;
  bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
  bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
  case GLFW_KEY_1:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    break;
  case GLFW_KEY_2:
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    break;
	default:
		if (action == GLFW_PRESS)
      Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
      Input.keys[key] = false;
	}
}

void processPlayerMovement(Player &player)
{
  if ((Input.keys[GLFW_KEY_W]) || (Input.keys[GLFW_KEY_UP]))
    player.ProcessInput(MovementDir::UP);
  else if ((Input.keys[GLFW_KEY_S]) || (Input.keys[GLFW_KEY_DOWN]))
    player.ProcessInput(MovementDir::DOWN);
  if ((Input.keys[GLFW_KEY_A]) || (Input.keys[GLFW_KEY_LEFT]))
    player.ProcessInput(MovementDir::LEFT);
  else if ((Input.keys[GLFW_KEY_D]) || (Input.keys[GLFW_KEY_RIGHT]))
    player.ProcessInput(MovementDir::RIGHT);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    Input.captureMouse = !Input.captureMouse;

  if (Input.captureMouse)
  {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Input.capturedMouseJustNow = true;
  }
  else
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
  if (Input.firstMouse)
  {
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
    Input.firstMouse = false;
  }

  GLfloat xoffset = float(xpos) - Input.lastX;
  GLfloat yoffset = Input.lastY - float(ypos);

  Input.lastX = float(xpos);
  Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
  // ...
}


int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  std::cout << "Controls: "<< std::endl;
  std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
  std::cout << "W, A, S, D - movement  "<< std::endl;
  std::cout << "press ESC to exit" << std::endl;

	return 0;
}

Image * ReadMap (std::string fileName) {
  std::ifstream file;
  int width = 0, height = 1;
  Image floor (FLOOR_PATH);
  Image walls (WALLS_PATH);
  Image exit (EXIT_PATH);
  Point cur_pos{0, tileSize};
  file.open(fileName);
  if (!file.is_open()) {
    std::cout << "ERROR\n";
    //error
  }
  std::string buf;
  while(getline(file, buf)) {
    width = std::max(width, (int)buf.length());
    height++;
  }
  // height++;
  std::cout << width << " | " << height << "\n";
  file.close();
  Image *img = new Image(width * tileSize, height * tileSize, 4);
  char sym;
  std::cout << "!\n";
  file.open(fileName);
  if (!file.is_open()) {
    std::cout << "ERROR\n";
    //error
  }
  //file.seekg(0, file.beg);
  std::cout << (int)file.tellg() << "\n";
  //file >> sym;
  std::cout << file.eof() << "\n";
  while (file.get(sym)) {
    //std::cout << "4";
    //sym = 0;
    //file >> sym;
    std::cout << sym << "|";
    switch (sym) {
      case ' ':
        // do nothing
        break;
      case '#':
        //std::cout << "1";
        img->PutTile(cur_pos.x, cur_pos.y, walls, WALL);
        break;
      case '.':
        //std::cout << "2\n";
        img->PutTile(cur_pos.x, cur_pos.y, floor, FLOOR);
        break;
      case '@':
        map_pos.x = starting_pos.x - cur_pos.x;
        map_pos.y = starting_pos.y - cur_pos.y;
        //std::cout << map_pos.x << " | " << map_pos.y << "\n";
        img->PutTile(cur_pos.x, cur_pos.y, floor, FLOOR);
        break;
      case 'x':
        img->PutTile(cur_pos.x, cur_pos.y, floor, EXIT);
        for (int y = 0; y < tileSize; y++) {
          for (int x = 0; x < tileSize; x++) {
            img->MixPixels(cur_pos.x + x, cur_pos.y + y, exit.GetPixel(x, tileSize - y - 1));
          }
        }
        break;
      case '\n':
        break;
      default:
        //std::cout << "3";
        break;
    }
    if (sym != '\n') {
      cur_pos.x += tileSize;
    } else {
      cur_pos.y += tileSize;
      cur_pos.x = 0;
    }
  }
  file.close();
  return img;
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window); 

	glfwSetKeyCallback        (window, OnKeyboardPressed);  
	glfwSetCursorPosCallback  (window, OnMouseMove); 
  glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback     (window, OnMouseScroll);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

	Player player{starting_pos};
  Image player_tile (PLAYER_PATH);
  player.SetTile(player_tile);

	map = *ReadMap("./resources/map.txt");
  std::cout << map.Height() / tileSize << "\n";

	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
  std::cout << "kok\n";

  Image exit_msg (EXIT_MSG_PATH);
  Point start {.x = (screenBuffer.Width() - exit_msg.Width()) / 2,
               .y = (screenBuffer.Height() - exit_msg.Height()) / 2};
  std::cout << start.x << " | " << start.y << "\n";

  //game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
    glfwPollEvents();
    
    if (!reach_exit) {
      screenBuffer.Clean();
      processPlayerMovement(player);

      for (int y = 0; (y < map.Height()) && (y + map_pos.y < screenBuffer.Height()); y++) {
        if (y + map_pos.y < 0) {
          continue;
        }
        for (int x = 0; (x < map.Width()) && (x + map_pos.x < screenBuffer.Width()); x++) {
          //std::cout << y << " ";
          if (x + map_pos.x < 0) {
            continue;
          }
          screenBuffer.PutPixel(x + map_pos.x, y + map_pos.y, map.GetPixel(x, y));
        }
        //std::cout << y << "\n";
      }

      player.Draw(screenBuffer);
    } else {
      std::cout << "here";
      for (int y = 0; (y < exit_msg.Height()) && (y + start.y < screenBuffer.Height()); y++) {
        if (y + start.y < 0) {
          continue;
        }
        for (int x = 0; (x < exit_msg.Width()) && (x + start.x < screenBuffer.Width()); x++) {
          if (x + start.x < 0) {
            continue;
          }
          screenBuffer.MixPixels(x + start.x, y + start.y, exit_msg.GetPixel(x, y));
        }
      }
    }

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
    glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
