#include "Shader.h"

Shader::Shader(const char* const &vertexPath,const char* const &fragmentPath){

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream VertexFile;
    std::ifstream FragmentFile;

    VertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    FragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{

        VertexFile.open(vertexPath);
        FragmentFile.open(fragmentPath);

        std::stringstream vShaderStream,fShaderStream;

        vShaderStream << VertexFile.rdbuf();
        fShaderStream << FragmentFile.rdbuf();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }catch(std::ifstream::failure e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* const vShaderCode = vertexCode.c_str();
    const char* const fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex,1,&vShaderCode,NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(vertex,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::FAILED_TO_COMPILE\n" <<
                    infoLog << std::endl;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment,1,&fShaderCode,NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment,GL_COMPILE_STATUS,&success);
    if(!success){
        glGetShaderInfoLog(fragment,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::FAILED_TO_COMPILE\n" <<
                    infoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID,vertex);
    glAttachShader(ID,fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID,GL_LINK_STATUS,&success);
    if(!success){
        glGetProgramInfoLog(ID,512,NULL,infoLog);
        std::cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" <<
                    infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID,name.c_str()),value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID,name.c_str()),value);
}
