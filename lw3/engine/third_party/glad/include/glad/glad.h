#ifndef __glad_h_
#define __glad_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <KHR/khrplatform.h>

#ifndef GLAD_GL_API
#define GLAD_GL_API
#endif
#ifndef GLAD_API_CALL
#define GLAD_API_CALL
#endif
#ifndef GLAD_API_PTR
#define GLAD_API_PTR
#endif

#define GLAD_MAKE_VERSION(major, minor) (major * 10000 + minor)
#define GLAD_VERSION_MAJOR(version) (version / 10000)
#define GLAD_VERSION_MINOR(version) (version % 10000)

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef khronos_int8_t GLbyte;
typedef khronos_uint8_t GLubyte;
typedef khronos_int16_t GLshort;
typedef khronos_uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef khronos_int32_t GLclampx;
typedef int GLsizei;
typedef khronos_float_t GLfloat;
typedef khronos_float_t GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void *GLeglClientBufferEXT;
typedef void *GLeglImageOES;
typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef size_t GLsizeiptr;

#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000
#define GL_FALSE                           0
#define GL_TRUE                            1
#define GL_TRIANGLES                       0x0004
#define GL_TRIANGLE_FAN                    0x0006
#define GL_FRAGMENT_SHADER                 0x8B30
#define GL_VERTEX_SHADER                   0x8B31
#define GL_COMPILE_STATUS                  0x8B81
#define GL_LINK_STATUS                     0x8B82
#define GL_INFO_LOG_LENGTH                 0x8B84
#define GL_ARRAY_BUFFER                    0x8892
#define GL_STATIC_DRAW                     0x88E4
#define GL_FLOAT                           0x1406

typedef void* (GLAD_API_PTR *GLADloadproc)(const char *name);

GLAD_GL_API int gladLoadGLLoader(GLADloadproc load);

GLAD_GL_API void glClear(GLbitfield mask);
GLAD_GL_API void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAD_GL_API void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
GLAD_GL_API void glGenVertexArrays(GLsizei n, GLuint *arrays);
GLAD_GL_API void glBindVertexArray(GLuint array);
GLAD_GL_API void glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
GLAD_GL_API void glGenBuffers(GLsizei n, GLuint *buffers);
GLAD_GL_API void glBindBuffer(GLenum target, GLuint buffer);
GLAD_GL_API void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
GLAD_GL_API void glDeleteBuffers(GLsizei n, const GLuint *buffers);
GLAD_GL_API void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
GLAD_GL_API void glEnableVertexAttribArray(GLuint index);
GLAD_GL_API void glDrawArrays(GLenum mode, GLint first, GLsizei count);
GLAD_GL_API GLuint glCreateShader(GLenum type);
GLAD_GL_API void glShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
GLAD_GL_API void glCompileShader(GLuint shader);
GLAD_GL_API void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
GLAD_GL_API void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAD_GL_API void glDeleteShader(GLuint shader);
GLAD_GL_API GLuint glCreateProgram(void);
GLAD_GL_API void glAttachShader(GLuint program, GLuint shader);
GLAD_GL_API void glLinkProgram(GLuint program);
GLAD_GL_API void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
GLAD_GL_API void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
GLAD_GL_API void glDeleteProgram(GLuint program);
GLAD_GL_API void glUseProgram(GLuint program);
GLAD_GL_API GLint glGetUniformLocation(GLuint program, const GLchar *name);
GLAD_GL_API void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAD_GL_API void glUniform4fv(GLint location, GLsizei count, const GLfloat *value);

#ifdef __cplusplus
}
#endif

#endif
