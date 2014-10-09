#include "VertexShader.hpp"

VertexShader::VertexShader()
{
    //Does Nothing
    this->ShaderID = 0;
}

VertexShader::VertexShader(string fileloc)
{
    this->ShaderID = glCreateShader(GL_VERTEX_SHADER);
    this->BuildShader(fileloc);
}

void VertexShader::BuildShader(string fileloc)
{
    const char * vertex_file_path = fileloc.c_str();

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);

	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
		{
            VertexShaderCode += "\n" + Line;
		}
		VertexShaderStream.close();
	}
	else
	{
		printf("Cannot open %s. Please Make sure that the Shader is in the right folder\n", vertex_file_path);
		cout << "Shadler Failed to be Loaded" << endl;
		return;
	}

    GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(ShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(ShaderID);

	// Check Vertex Shader
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
}

VertexShader::~VertexShader()
{
    printf("Deleting Vertex Shader \n");
    //Removes the Shader
    glDeleteShader(ShaderID);
}

