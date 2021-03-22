#if ( (defined(__MACH__)) && (defined(__APPLE__)) )
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <OpenGL/glext.h>
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#else
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif
