#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

int main() {
  // start GL context and O/S window using GLFW
  if (!glfwInit()) {
    fprintf (stderr, "ERROR: could not start GLFW3\n");
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 

  GLFWwindow* window = glfwCreateWindow (640, 480, "Hello Triangle", NULL, NULL);

  if (!window) {
    fprintf(stderr, "ERROR: could not open window with GLFW3\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent (window);

  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit();

  // get version info
  const GLubyte* renderer = glGetString (GL_RENDERER);
  const GLubyte* version = glGetString (GL_VERSION);
  printf ("Renderer: %s\n", renderer);
  printf ("Opengl version supperted %s\n", version);

  // teel GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS);    // depth-testing interprets a smaller value as "closer"

  /* OTHER STUFF GOES HERE NEXT */

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}
