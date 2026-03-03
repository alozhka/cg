#include <glad/glad.h>
#include <stddef.h>
#include <stdint.h>

static GLADloadproc glad_gl_loader = NULL;

#define GLAD_GL_MAX(x, y) (x) > (y) ? (x) : (y)

typedef void (*GLAD_GL_PFNglClear)(GLbitfield);
typedef void (*GLAD_GL_PFNglClearColor)(GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (*GLAD_GL_PFNglViewport)(GLint, GLint, GLsizei, GLsizei);
typedef void (*GLAD_GL_PFNglGenVertexArrays)(GLsizei, GLuint*);
typedef void (*GLAD_GL_PFNglBindVertexArray)(GLuint);
typedef void (*GLAD_GL_PFNglDeleteVertexArrays)(GLsizei, const GLuint*);
typedef void (*GLAD_GL_PFNglGenBuffers)(GLsizei, GLuint*);
typedef void (*GLAD_GL_PFNglBindBuffer)(GLenum, GLuint);
typedef void (*GLAD_GL_PFNglBufferData)(GLenum, GLsizeiptr, const void*, GLenum);
typedef void (*GLAD_GL_PFNglDeleteBuffers)(GLsizei, const GLuint*);
typedef void (*GLAD_GL_PFNglVertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*);
typedef void (*GLAD_GL_PFNglEnableVertexAttribArray)(GLuint);
typedef void (*GLAD_GL_PFNglDrawArrays)(GLenum, GLint, GLsizei);
typedef GLuint (*GLAD_GL_PFNglCreateShader)(GLenum);
typedef void (*GLAD_GL_PFNglShaderSource)(GLuint, GLsizei, const GLchar* const*, const GLint*);
typedef void (*GLAD_GL_PFNglCompileShader)(GLuint);
typedef void (*GLAD_GL_PFNglGetShaderiv)(GLuint, GLenum, GLint*);
typedef void (*GLAD_GL_PFNglGetShaderInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (*GLAD_GL_PFNglDeleteShader)(GLuint);
typedef GLuint (*GLAD_GL_PFNglCreateProgram)(void);
typedef void (*GLAD_GL_PFNglAttachShader)(GLuint, GLuint);
typedef void (*GLAD_GL_PFNglLinkProgram)(GLuint);
typedef void (*GLAD_GL_PFNglGetProgramiv)(GLuint, GLenum, GLint*);
typedef void (*GLAD_GL_PFNglGetProgramInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*);
typedef void (*GLAD_GL_PFNglDeleteProgram)(GLuint);
typedef void (*GLAD_GL_PFNglUseProgram)(GLuint);
typedef GLint (*GLAD_GL_PFNglGetUniformLocation)(GLuint, const GLchar*);
typedef void (*GLAD_GL_PFNglUniformMatrix3fv)(GLint, GLsizei, GLboolean, const GLfloat*);
typedef void (*GLAD_GL_PFNglUniform4fv)(GLint, GLsizei, const GLfloat*);

static GLAD_GL_PFNglClear glad_gl_Clear;
static GLAD_GL_PFNglClearColor glad_gl_ClearColor;
static GLAD_GL_PFNglViewport glad_gl_Viewport;
static GLAD_GL_PFNglGenVertexArrays glad_gl_GenVertexArrays;
static GLAD_GL_PFNglBindVertexArray glad_gl_BindVertexArray;
static GLAD_GL_PFNglDeleteVertexArrays glad_gl_DeleteVertexArrays;
static GLAD_GL_PFNglGenBuffers glad_gl_GenBuffers;
static GLAD_GL_PFNglBindBuffer glad_gl_BindBuffer;
static GLAD_GL_PFNglBufferData glad_gl_BufferData;
static GLAD_GL_PFNglDeleteBuffers glad_gl_DeleteBuffers;
static GLAD_GL_PFNglVertexAttribPointer glad_gl_VertexAttribPointer;
static GLAD_GL_PFNglEnableVertexAttribArray glad_gl_EnableVertexAttribArray;
static GLAD_GL_PFNglDrawArrays glad_gl_DrawArrays;
static GLAD_GL_PFNglCreateShader glad_gl_CreateShader;
static GLAD_GL_PFNglShaderSource glad_gl_ShaderSource;
static GLAD_GL_PFNglCompileShader glad_gl_CompileShader;
static GLAD_GL_PFNglGetShaderiv glad_gl_GetShaderiv;
static GLAD_GL_PFNglGetShaderInfoLog glad_gl_GetShaderInfoLog;
static GLAD_GL_PFNglDeleteShader glad_gl_DeleteShader;
static GLAD_GL_PFNglCreateProgram glad_gl_CreateProgram;
static GLAD_GL_PFNglAttachShader glad_gl_AttachShader;
static GLAD_GL_PFNglLinkProgram glad_gl_LinkProgram;
static GLAD_GL_PFNglGetProgramiv glad_gl_GetProgramiv;
static GLAD_GL_PFNglGetProgramInfoLog glad_gl_GetProgramInfoLog;
static GLAD_GL_PFNglDeleteProgram glad_gl_DeleteProgram;
static GLAD_GL_PFNglUseProgram glad_gl_UseProgram;
static GLAD_GL_PFNglGetUniformLocation glad_gl_GetUniformLocation;
static GLAD_GL_PFNglUniformMatrix3fv glad_gl_UniformMatrix3fv;
static GLAD_GL_PFNglUniform4fv glad_gl_Uniform4fv;

static void* load(const char* name) {
    return (void*)glad_gl_loader(name);
}

int gladLoadGLLoader(GLADloadproc load) {
    glad_gl_loader = load;
    glad_gl_Clear = (GLAD_GL_PFNglClear)load("glClear");
    glad_gl_ClearColor = (GLAD_GL_PFNglClearColor)load("glClearColor");
    glad_gl_Viewport = (GLAD_GL_PFNglViewport)load("glViewport");
    glad_gl_GenVertexArrays = (GLAD_GL_PFNglGenVertexArrays)load("glGenVertexArrays");
    glad_gl_BindVertexArray = (GLAD_GL_PFNglBindVertexArray)load("glBindVertexArray");
    glad_gl_DeleteVertexArrays = (GLAD_GL_PFNglDeleteVertexArrays)load("glDeleteVertexArrays");
    glad_gl_GenBuffers = (GLAD_GL_PFNglGenBuffers)load("glGenBuffers");
    glad_gl_BindBuffer = (GLAD_GL_PFNglBindBuffer)load("glBindBuffer");
    glad_gl_BufferData = (GLAD_GL_PFNglBufferData)load("glBufferData");
    glad_gl_DeleteBuffers = (GLAD_GL_PFNglDeleteBuffers)load("glDeleteBuffers");
    glad_gl_VertexAttribPointer = (GLAD_GL_PFNglVertexAttribPointer)load("glVertexAttribPointer");
    glad_gl_EnableVertexAttribArray = (GLAD_GL_PFNglEnableVertexAttribArray)load("glEnableVertexAttribArray");
    glad_gl_DrawArrays = (GLAD_GL_PFNglDrawArrays)load("glDrawArrays");
    glad_gl_CreateShader = (GLAD_GL_PFNglCreateShader)load("glCreateShader");
    glad_gl_ShaderSource = (GLAD_GL_PFNglShaderSource)load("glShaderSource");
    glad_gl_CompileShader = (GLAD_GL_PFNglCompileShader)load("glCompileShader");
    glad_gl_GetShaderiv = (GLAD_GL_PFNglGetShaderiv)load("glGetShaderiv");
    glad_gl_GetShaderInfoLog = (GLAD_GL_PFNglGetShaderInfoLog)load("glGetShaderInfoLog");
    glad_gl_DeleteShader = (GLAD_GL_PFNglDeleteShader)load("glDeleteShader");
    glad_gl_CreateProgram = (GLAD_GL_PFNglCreateProgram)load("glCreateProgram");
    glad_gl_AttachShader = (GLAD_GL_PFNglAttachShader)load("glAttachShader");
    glad_gl_LinkProgram = (GLAD_GL_PFNglLinkProgram)load("glLinkProgram");
    glad_gl_GetProgramiv = (GLAD_GL_PFNglGetProgramiv)load("glGetProgramiv");
    glad_gl_GetProgramInfoLog = (GLAD_GL_PFNglGetProgramInfoLog)load("glGetProgramInfoLog");
    glad_gl_DeleteProgram = (GLAD_GL_PFNglDeleteProgram)load("glDeleteProgram");
    glad_gl_UseProgram = (GLAD_GL_PFNglUseProgram)load("glUseProgram");
    glad_gl_GetUniformLocation = (GLAD_GL_PFNglGetUniformLocation)load("glGetUniformLocation");
    glad_gl_UniformMatrix3fv = (GLAD_GL_PFNglUniformMatrix3fv)load("glUniformMatrix3fv");
    glad_gl_Uniform4fv = (GLAD_GL_PFNglUniform4fv)load("glUniform4fv");
    return 1;
}

void glClear(GLbitfield mask) { glad_gl_Clear(mask); }
void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) { glad_gl_ClearColor(red, green, blue, alpha); }
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) { glad_gl_Viewport(x, y, width, height); }
void glGenVertexArrays(GLsizei n, GLuint *arrays) { glad_gl_GenVertexArrays(n, arrays); }
void glBindVertexArray(GLuint array) { glad_gl_BindVertexArray(array); }
void glDeleteVertexArrays(GLsizei n, const GLuint *arrays) { glad_gl_DeleteVertexArrays(n, arrays); }
void glGenBuffers(GLsizei n, GLuint *buffers) { glad_gl_GenBuffers(n, buffers); }
void glBindBuffer(GLenum target, GLuint buffer) { glad_gl_BindBuffer(target, buffer); }
void glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage) { glad_gl_BufferData(target, size, data, usage); }
void glDeleteBuffers(GLsizei n, const GLuint *buffers) { glad_gl_DeleteBuffers(n, buffers); }
void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer) { glad_gl_VertexAttribPointer(index, size, type, normalized, stride, pointer); }
void glEnableVertexAttribArray(GLuint index) { glad_gl_EnableVertexAttribArray(index); }
void glDrawArrays(GLenum mode, GLint first, GLsizei count) { glad_gl_DrawArrays(mode, first, count); }
GLuint glCreateShader(GLenum type) { return glad_gl_CreateShader(type); }
void glShaderSource(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length) { glad_gl_ShaderSource(shader, count, string, length); }
void glCompileShader(GLuint shader) { glad_gl_CompileShader(shader); }
void glGetShaderiv(GLuint shader, GLenum pname, GLint *params) { glad_gl_GetShaderiv(shader, pname, params); }
void glGetShaderInfoLog(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) { glad_gl_GetShaderInfoLog(shader, bufSize, length, infoLog); }
void glDeleteShader(GLuint shader) { glad_gl_DeleteShader(shader); }
GLuint glCreateProgram(void) { return glad_gl_CreateProgram(); }
void glAttachShader(GLuint program, GLuint shader) { glad_gl_AttachShader(program, shader); }
void glLinkProgram(GLuint program) { glad_gl_LinkProgram(program); }
void glGetProgramiv(GLuint program, GLenum pname, GLint *params) { glad_gl_GetProgramiv(program, pname, params); }
void glGetProgramInfoLog(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) { glad_gl_GetProgramInfoLog(program, bufSize, length, infoLog); }
void glDeleteProgram(GLuint program) { glad_gl_DeleteProgram(program); }
void glUseProgram(GLuint program) { glad_gl_UseProgram(program); }
GLint glGetUniformLocation(GLuint program, const GLchar *name) { return glad_gl_GetUniformLocation(program, name); }
void glUniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) { glad_gl_UniformMatrix3fv(location, count, transpose, value); }
void glUniform4fv(GLint location, GLsizei count, const GLfloat *value) { glad_gl_Uniform4fv(location, count, value); }
