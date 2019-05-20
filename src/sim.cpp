#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Camera.h"
#include "model.h"

using namespace std;


static GLuint load_shader(char *filepath, GLenum type);


int main()
{
  //init settings
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(1920, 1080, "Electric Field Simulator", nullptr, nullptr); // Windowed

  glfwMakeContextCurrent(window);
  
  //init OpenGL and link functions in a dynamic way
  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  
  
    // check OpenGL error
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    cerr << "OpenGL error: " << err << endl;
  }

  
  Camera cam;
  cam = Camera();
  cam.view = glm::lookAt(
    glm::vec3(1.0f, 3.2f, 5.2f), // position
    glm::vec3(0.0f, 0.0f, 0.0f), // camera center
    glm::vec3(0.0f, 0.0f, -1.0f) // up axis
    );
  
  Model charge;

  charge = Model();

  charge.loadFromObj("assets/sphere.obj", 0);
  
  float lastTime;
  // Get mouse position
  double xpos, ypos;
  glm::vec3 position = glm::vec3( 0, 0, 5 );
  float speed = 3.0f; // 3 units / second
  float mouseSpeed = 0.3f;
  float horizontalAngle = 3.14f;
  float verticalAngle = 0.0f;
  
  //main loop
  while(!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    glfwPollEvents();

    double currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;

    glfwGetCursorPos(window,&xpos, &ypos);
    glfwSetCursorPos(window,800/2, 600/2);
    
    horizontalAngle += mouseSpeed * deltaTime * float(800/2 - xpos );
    verticalAngle   += mouseSpeed * deltaTime * float( 600/2 - ypos );

    glm::vec3 direction(
      cos(verticalAngle) * sin(horizontalAngle),
      sin(verticalAngle),
      cos(verticalAngle) * cos(horizontalAngle)
      );

    glm::vec3 right = glm::vec3(
      sin(horizontalAngle - 3.14f/2.0f),
      0,
      cos(horizontalAngle - 3.14f/2.0f)
      );
    glm::vec3 up = glm::cross( right, direction );

    if (glfwGetKey(window,GLFW_KEY_W ) == GLFW_PRESS){
      position += direction * deltaTime * speed;
    }
    if (glfwGetKey(window,GLFW_KEY_S ) == GLFW_PRESS){
      position -= direction * deltaTime * speed;
    }
    if (glfwGetKey(window,GLFW_KEY_D ) == GLFW_PRESS){
      position += right * deltaTime * speed;
    }
    if (glfwGetKey(window,GLFW_KEY_A ) == GLFW_PRESS){
      position -= right * deltaTime * speed;
    }



    cam.view = glm::lookAt(
      position, // position
      position+direction, // camera center
      up // up axis
    );
    //draw code
    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //DRAW HERE
    charge.render(cam, 1.0f, 0.0f, 0.0f, 1.0f);
    
    lastTime = currentTime;
  }
  
  glfwTerminate();

  return 0;
}

static GLuint load_shader(char *filepath, GLenum type)
{
  FILE *file = fopen(filepath, "rb");
  if(!file) {
    return 0;
  }
 
  long len;
  if(fseek(file, 0, SEEK_END) != 0 ||
     (len = ftell(file)) == -1L) {
    fclose(file);
    return 0;
  }
  rewind(file);
 
  char *buffer = (char *) malloc(len);
  if(fread(buffer, 1, len, file) != len) {
    fclose(file);
    free(buffer);
    return 0;
  }
  fclose(file);
 
  GLuint shader = glCreateShader(type);
  if(shader == 0) {
    free(buffer);
    return 0;
  }
 
  glShaderSource(shader, 1,(const char * const *) &buffer, (GLint *) &len);
  free(buffer);
  return shader;
}
