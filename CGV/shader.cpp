#include "shader.h"

Shader::Shader(const char* vertexFile, const char* geometryFile, const char* fragmentFile){
	vertexID = loadShader(GL_VERTEX_SHADER, vertexFile);
	if (geometryFile != NULL) {
		geometryID = loadShader(GL_GEOMETRY_SHADER, geometryFile);
	} else {
		geometryID = 0;
	}
	fragmentID = loadShader(GL_FRAGMENT_SHADER, fragmentFile);
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexID);
	glAttachShader(shaderID, fragmentID);
	if (geometryFile != NULL){
        glAttachShader(shaderID, geometryID);
    } 
	glLinkProgram(shaderID);
    GLint infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shaderID, infologLength, &charsWritten, infoLog);
		std::cout << infoLog << std::endl;
		delete []infoLog;
	}
	glUseProgram(0);

	glDeleteShader(vertexID);
	glDeleteShader(geometryID);
	glDeleteShader(fragmentID);
}

Shader::~Shader(){
    /*glDetachShader(shaderID, vertexID);
	if (geometryID != 0){
		glDetachShader(shaderID, geometryID);
	}
	glDetachShader(shaderID, fragmentID);
	glDeleteShader(vertexID);
	if (geometryID != 0){
		glDeleteShader(geometryID);
	}
	glDeleteShader(fragmentID);*/
	glDeleteProgram(shaderID);
}

/*std::string Shader::readFile(const char* fileName){
    std::string tmp = "";
    std::string shade = "";
    std::ifstream input;

    input.open(fileName);
    if (input.is_open()){
        while (std::getline(input, tmp)){
            shade += tmp + "\n";
        }
    }
    else{
        std::cout << "ERROR. CANNOT OPEN: " << fileName << std::endl;
    }
    input.close();

    return shade;
}*/

char* Shader::readFile(const char* fileName) {
	int filesize;
	FILE* file;
	char* result;

#pragma warning(suppress : 4996) //Turn off an error in Visual Studio stemming from Microsoft not adhering to standards
	file = fopen(fileName, "rb");
	if (file != NULL) {
		fseek(file, 0, SEEK_END);
		filesize = ftell(file);
		fseek(file, 0, SEEK_SET);
		result = new char[filesize + 1];
#pragma warning(suppress : 6386) //Turn off an error in Visual Studio stemming from incorrent static code analysis
		int readsize = fread(result, 1, filesize, file);
		result[filesize] = 0;
		fclose(file);

		return result;
	}

	return NULL;

}

GLuint Shader::loadShader(GLenum shaderType, const char* fileName){
    GLuint shader = glCreateShader(shaderType);
    const GLchar* shaderSrc = readFile(fileName);
    //const GLchar* shaderSrc = src.c_str();
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    delete []shaderSrc;
    int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);
	if (infologLength > 1) {
		infoLog = new char[infologLength];
		glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
		std::cout << infoLog << std::endl;
		delete []infoLog;
	}
    return shader;
}

void Shader::use(){
    glUseProgram(this->shaderID);
}

void Shader::unuse(){
	glUseProgram(0);
}

void initShader(std::vector<Shader*> &shaders, const char* vertexFile, const char* geometryFile, const char* fragmentFile){
	shaders.push_back(new Shader(vertexFile, geometryFile, fragmentFile));
}

void freeShader(std::vector<Shader*> &shaders){
	for (auto*& i : shaders){
		delete i;
	}
}
void Shader::setMat4fv(glm::mat4 val, const GLchar* name){
	this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->shaderID, name), 1, GL_FALSE, glm::value_ptr(val));
	this->unuse();
}

void Shader::setVec3f(glm::fvec3 val, const GLchar* name){
	this->use();
	glUniform3fv(glGetUniformLocation(this->shaderID, name), 1, glm::value_ptr(val));
	this->unuse();
}

void Shader::setFloat(GLfloat val, const GLchar* name){
	this->use();
	glUniform1f(glGetUniformLocation(this->shaderID, name), val);
	this->unuse();
}