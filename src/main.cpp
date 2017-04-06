#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>

inline void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR){
        printf("OpenGL error %08x, at %s:%i - for %s.\n", err, fname, line, stmt);
        exit(1);
    }
}

// helper macro that checks for GL errors.
#define GL_C(stmt) do {					\
	stmt;						\
	CheckOpenGLError(#stmt, __FILE__, __LINE__);	\
    } while (0)

inline char* GetShaderLogInfo(GLuint shader) {
    GLint len;
    GLsizei actualLen;
    GL_C(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len));
    char* infoLog = new char[len];
    GL_C(glGetShaderInfoLog(shader, len, &actualLen, infoLog));
    return infoLog;
}

inline GLuint CreateShaderFromString(const std::string& shaderSource, const GLenum shaderType) {
    GLuint shader;

    GL_C(shader = glCreateShader(shaderType));
    const char *c_str = shaderSource.c_str();
    GL_C(glShaderSource(shader, 1, &c_str, NULL));
    GL_C(glCompileShader(shader));

    GLint compileStatus;
    GL_C(glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus != GL_TRUE) {
        printf("Could not compile shader\n\n%s \n\n%s\n", shaderSource.c_str(),
               GetShaderLogInfo(shader));
        exit(1);
    }

    return shader;
}

/*
  Load shader with only vertex and fragment shader.
*/
inline GLuint LoadNormalShader(const std::string& vsSource, const std::string& fsShader){
    GLuint vs = CreateShaderFromString(vsSource, GL_VERTEX_SHADER);
    GLuint fs = CreateShaderFromString(fsShader, GL_FRAGMENT_SHADER);

    GLuint shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);

    GLint Result;
    glGetProgramiv(shader, GL_LINK_STATUS, &Result);
    if (Result == GL_FALSE) {
        printf("Could not link shader \n\n%s\n", GetShaderLogInfo(shader));
        exit(1);
    }

    glDetachShader(shader, vs);
    glDetachShader(shader, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return shader;
}

GLuint vao;
GLFWwindow* window;

void InitGlfw() {
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	const int WINDOW_WIDTH = 1497;
	const int WINDOW_HEIGHT = 1014;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Demo", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // load GLAD.
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Bind and create VAO, otherwise, we can't do anything in OpenGL.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

inline std::string LoadFile(const char* path) {

	//
	//  Open file
	//
	FILE* fp = fopen(path, "rb");

	if (!fp) {
		printf("Could not open %s", path);
		exit(1);
	}

	//
	// Get file size.
	//

	// seek to end of file.
	fseek(fp, 0L, SEEK_END);
	long fileSize = ftell(fp);
	// reset file pointer
	fseek(fp, 0L, SEEK_SET);

	//
	// Read file.
	//

	char* buffer = new char[fileSize];
	fread(buffer, sizeof(char), (size_t)fileSize, fp);
	std::string str(buffer, fileSize);
	free(buffer);

	return str;
}

int main(int argc, char** argv) {
    InitGlfw();

	GLuint  shader = LoadNormalShader(
        LoadFile("vert.glsl"),
		LoadFile("frag.glsl")
        );

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
