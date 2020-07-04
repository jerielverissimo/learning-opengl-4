#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdbool.h>

#define GL_LOG_FILE "gl.log"

bool restart_gl_log () {
  FILE* file = fopen (GL_LOG_FILE, "w");

  if (!file) {
    fprintf (stderr, 
             "ERROR: could not open GL_LOG_FILE  log file %s for writing\n",
             GL_LOG_FILE);
    return false;
  }

  time_t now = time (NULL);
  char* date = ctime (&now);

  fprintf (file, "GL_LOG_FILE log. local time %s\n", date);
  fclose (file);
  return true;
}

bool gl_log (const char* message, ...) {
  va_list argptr;

  FILE* file = fopen (GL_LOG_FILE, "a");

  if (!file) {
    fprintf (stderr, 
             "ERROR: could not open GL_LOG_FILE  log file %s for writing\n",
             GL_LOG_FILE);
    return false;
  }

  va_start (argptr, message);
  vfprintf (file, message, argptr);
  va_end (argptr);
  fclose(file);
  return true;
}

bool gl_log_err (const char* message, ...) {
  va_list argptr;

  FILE* file = fopen (GL_LOG_FILE, "a");

  if (!file) {
    fprintf (stderr, 
             "ERROR: could not open GL_LOG_FILE  log file %s for writing\n",
             GL_LOG_FILE);
    return false;
  }

  va_start (argptr, message);
  vfprintf (file, message, argptr);
  va_end (argptr);

  va_start (argptr, message);
  vfprintf (stderr, message, argptr);
  va_end (argptr);

  fclose (file);
  
  return true;
}

void gflw_error_callback (int error, const char* description) {
  gl_log_err ("GLFW ERROR: code %i msg: %s\n", error, description);
}

int main() {

  if (!restart_gl_log ()) { /* quit? */ }

  // start GL context and O/S window using GLFW helper library
  gl_log ("starting GLFW\n%s\n", glfwGetVersionString());
  // register the error call-back function that we wrote, above
  glfwSetErrorCallback (gflw_error_callback);
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

  GLfloat points[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
  };

  GLuint vbo = 0;
  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof (points), points, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);
  glEnableVertexAttribArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  const char* vertex_shader = "#version 410\n"
    "in vec3 vp;"
    "void main () {"
    "  gl_Position = vec4 (vp, 1.0);"
    "}";

  const char* fragment_shader = "#version 410\n"
    "out vec4 frag_colour;"
    "void main () {"
    "  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
    "}";

  GLuint vs = glCreateShader (GL_VERTEX_SHADER);
  glShaderSource (vs, 1, &vertex_shader, NULL);
  glCompileShader (vs);

  GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
  glShaderSource (fs, 1, &fragment_shader, NULL);
  glCompileShader (fs);

  GLuint shader_program = glCreateProgram ();
  glAttachShader (shader_program, fs);
  glAttachShader (shader_program, vs);
  glLinkProgram (shader_program);

  while (!glfwWindowShouldClose (window)) {
    // wipe the drawing surface clear
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram (shader_program);
    glBindVertexArray (vao);

    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays (GL_TRIANGLES, 0, 3);
    // update other events like input handling
    glfwPollEvents ();
    // put the stuff we've been drawing onto the display 
    glfwSwapBuffers (window);
  }

  // close GL context and any other GLFW resources
  glfwTerminate();
  return 0;
}
