#include <iostream>
#include <fstream>

// glew.h�� gl.h�� �����ϱ� ���� �����ؾ��Ѵ�.
#include "GL/glew.h"
#include "GL/freeglut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


GLuint vaoHandle;
GLuint programHandle;
GLuint setShader(const char* shaderType, const char* shaderName);
char* loadShaderAsString(std::string fileName);


void init()
{

	// 1. ���̴� ������Ʈ�� �����Ѵ�.
	GLuint vertShader = setShader("vertex", "basic.vert");
	GLuint fragShader = setShader("fragment", "basic.frag");




	// ������Ʈ �������� ������ OpenGL pipeline�� �������� ���� ���̴��� ��� Ȥ�� ��ġ�ؾ��Ѵ�.

	// 1. ���� Program object�� �����Ѵ�.
	// �� ���α׷� ����
	programHandle = glCreateProgram();
	if (0 == programHandle)
	{
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}

	// 2. ���̴����� ���α׷��� ���δ�.
	glAttachShader(programHandle, vertShader);
	glAttachShader(programHandle, fragShader);


	/////////////////
	// VertexPosition �� VertexColor������ ���� �ε����� �����Ѵ�.
	// Bind index 0 to the shader input variable "VertexPosition"
	//glBindAttribLocation(programHandle, 0, "VertexPosition");
	// Bind index 1 to the shader input variable "VertexColor"
	//glBindAttribLocation(programHandle, 1, "VertexColor");
	///////////////////


	// 3. ���α׷��� ��ũ�Ѵ�. ����
	glLinkProgram(programHandle);

	// 4. ��ũ ���� Ȯ��
	GLint status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {
		fprintf(stderr, "Failed to link shader program!\n");
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH,
			&logLen);
		if (logLen > 0)
		{
			char* log = (char*)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen,
				&written, log);
			fprintf(stderr, "Program log: \n%s", log);
			free(log);
		}
	}
	// 5. ���� ��ũ�� �����ߴٸ� ���α׷��� OpenGL pipeline�� ��ġ �Ѵ�.
	else
	{
		glUseProgram(programHandle);
	}

	////////////////////////////////////////////////////////////////////////
	// Ȱ��ȭ�� ���ؽ� ��ǲ��� �ε�����ȣ�� �����ش�.
	// Getting a list of active vertex input attributesand indices
	GLint maxLength, nAttribs;
	glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES,	&nAttribs);
	glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,	&maxLength);
	GLchar* name = (GLchar*)malloc(maxLength);

	GLint written, size, location;
	GLenum type;
	printf(" Index | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nAttribs; i++) {
		glGetActiveAttrib(programHandle, i, maxLength, &written, &size, &type, name);
		location = glGetAttribLocation(programHandle, name);
		printf(" %-5d | %s\n", location, name);
	}
	free(name);
	////////////////////////////////////////////////////////////////////////
	// Ȱ��ȭ�� unifrom ������ �����ش�.
	//Getting a list of active uniform variables
	GLint nUniforms, maxLen;
	glGetProgramiv(programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH,	&maxLen);
	glGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS, &nUniforms);
	name = (GLchar*)malloc(maxLen);

	printf(" Location | Name\n");
	printf("------------------------------------------------\n");
	for (int i = 0; i < nUniforms; ++i) {
		glGetActiveUniform(programHandle, i, maxLen, &written,
			&size, &type, name);
		location = glGetUniformLocation(programHandle, name);
		printf(" %-8d | %s\n", location, name);
	}
	free(name);
	////////////////////////////////////////////////////////////////////////


	// vertex�� ������.
	float positionData[] = {
	-0.8f, -0.8f, 0.0f,
	0.8f, -0.8f, 0.0f,
	0.0f, 0.8f, 0.0f};

	// color data RGB
	float colorData[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f };


	// �����ǰ� ������ �����ϱ� ���� ���۸� �����Ѵ�.
	// Create the buffer objects
	GLuint vboHandles[2];
	// ���� �ΰ�����.
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];


	// �� ������ ����Ÿ�� ���, �ű��.
	// Populate the position buffer
	// ������ ���۸� ���ε� �Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	// ������ ���� ����Ѵ�.
	// �ι�° ���ڴ� �迭�� ������ �̴�. ����° ���ڴ� �ش� �迭
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

	// Populate the color buffer
	// ���� ���۸� ���ε��Ѵ�.
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);


	// ���� ������Ʈ�� �����Ͽ��� ������, �̰͵��� vertex array obejct(VAO) �� ���´�.
	// VAO �����Ѵ�. (���������� vaoHandle �ʿ�)
	// Create and set-up the vertex array object
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	// �� �Ӽ� �ε����� generic vertex attribute �� Ȱ��ȭ ��Ų��.
	// Enable the vertex attribute arrays
	glEnableVertexAttribArray(0); // for Vertex position
	glEnableVertexAttribArray(1); // for Vertex color


	// ���� ������Ʈ�� generic vertex attribute index���� �����Ų��.
	// Map index 0 to the position buffer
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	// ù��°���� - generic attribute index, �ι�°���� - �� vertex�� ��Ҽ�(���⼭�� 3)(1,2,3 or 4)
	// ����°���� - �� ����� ����Ÿ Ÿ��, �׹�°���� - normalize ����, �ټ���° ���� - byte offset
	// ������°���� - ������ ���� offset(���⼭�� �����ǰ��ۿ� ���⶧���� 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
	// Map index 1 to the color buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);


}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// glm ���̺귯���� �̿��Ͽ� rotate ��Ʈ������ �����.
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), 30.f/* angle */,
	                                       glm::vec3(0.0f, 0.0f, 1.0f));


	// ���α׷� ������Ʈ�� �̿��Ͽ� uniform ������ location�� �����´�.
	// ���� Ȱ��ȭ�� uniform ������ �ƴ϶�� -1�� �����Ѵ�.
	GLuint location = glGetUniformLocation(programHandle, "RotationMatrix");

	if (location >= 0)
	{
		// ��Ʈ���� ���� uniform ������ �Ҵ��Ѵ�.
		// ù��°���� - location, �ι�° ���� - ��Ʈ������ �� (uniform ������ �迭�� �� �� �ִ�.)
		// ����°���� - ��Ʈ������ �����Ҷ� transpose(�ڹٲ�)�Ǿ�� �ϴ���
		// �׹�°���� - ��Ʈ������ ������ uniform������ ����.
		glUniformMatrix4fv(location, 1, GL_FALSE, &rotationMatrix[0][0]);
	}
	glBindVertexArray(vaoHandle);
	// 3��° ���� - ���ý��� ��
	glDrawArrays(GL_TRIANGLES, 0, 3); glDrawArrays(GL_TRIANGLES, 0, 3);


	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);

	//������ ������ ����
	glutInitWindowSize(512, 512);

	// top-left corner �ʱ� ���������� �ʱ�ȭ
	glutInitWindowPosition(0, 0);

	// ������â ����
	glutCreateWindow("Hello World");


	// glew�� �ʱ�ȭ ���ش�. opengl�� ����ϱ����ؼ�
	GLenum err = glewInit();
	// glewInit()�� �����ν� ��� OpenGL���̺귯���� ã�� ��� ��밡���� �Լ������͸� �ʱ�ȭ�Ѵ�.

	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error initializing GLEW: %s\n",
			glewGetErrorString(err));
	}

	// to compile shader
	init();

	glutDisplayFunc(display);


	// glut�� �̺�Ʈ ���μ��� loop�� ����.
	glutMainLoop();

	return 0;
}



GLuint setShader(const char* shaderType, const char* shaderName)
{
	GLuint shaderObj;
	if(strcmp(shaderType, "vertex") == 0)
	{
		shaderObj = glCreateShader(GL_VERTEX_SHADER);
	}
	else if(strcmp(shaderType, "fragment") == 0)
	{
		shaderObj = glCreateShader(GL_FRAGMENT_SHADER);
	}


	if (0 == shaderObj)
	{
		fprintf(stderr, "Error creating shader obj.\n");
		exit(1);
	}


	// 2. ���̴� �ҽ��ڵ带 ���̴� ������Ʈ�� �����Ѵ�.
	const GLchar* shaderCode = loadShaderAsString(shaderName);
	// �ҽ� �迭�� ���� �ҽ��ڵ带 ���� �� �ִ�.
	// �迭�� �ҽ��ڵ带 ������.
	const GLchar* codeArray[] = { shaderCode };
	// vertShader object�� codeArray�� �����Ѵ�.
	// ù��° ���ڴ� ���̴� ������Ʈ, �ι�° ���ڴ� �ҽ��ڵ��� �� ���� ���⼭�� shaderCode �Ѱ��� ���� 1
	// ����° ���ڴ� �ڵ带 ���� �迭, �׹�° ���ڴ� �� �ҽ��ڵ��� ���̸� ���� int�迭�̴� ���⼭�� null character�� �־ �ڵ����� Ȯ�εǱ⶧���� NULL�� �־���.
	glShaderSource(shaderObj, 1, codeArray, NULL);

	// 3. ���̴��� ������ �Ѵ�.
	glCompileShader(shaderObj);


	// 4. ������ �Ϸ� Ȯ��.
	GLint result;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		fprintf(stderr, "shader compilation failed!\n");
		GLint logLen;
		glGetShaderiv(shaderObj, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char* log = (char*)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(shaderObj, logLen, &written, log);
			fprintf(stderr, "Shader log:\n%s", log);
			free(log);
		}
	}

	return shaderObj;
}

char* loadShaderAsString(std::string fileName)
{
	// Initialize input stream.
	std::ifstream inFile(fileName.c_str(), std::ios::binary);

	// Determine shader file length and reserve space to read it in.
	inFile.seekg(0, std::ios::end);
	int fileLength = inFile.tellg();
	char* fileContent = (char*)malloc((fileLength + 1) * sizeof(char));

	// Read in shader file, set last character to NUL, close input stream.
	inFile.seekg(0, std::ios::beg);
	inFile.read(fileContent, fileLength);
	fileContent[fileLength] = '\0';
	inFile.close();

	return fileContent;
}