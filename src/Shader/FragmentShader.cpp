#include "FragmentShader.hpp"

FragmentShader::FragmentShader()
{
    //Does Nothing
    this->ShaderID = 0;
}

FragmentShader::FragmentShader(string fileloc)
{
    this->ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    this->BuildShader(fileloc);
}

void FragmentShader::BuildShader(string fileloc)
{
    const char * vertex_file_path = fileloc.c_str();

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(vertex_file_path, std::ios::in);

	if(FragmentShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
		{
            FragmentShaderCode += "\n" + Line;
		}
		FragmentShaderStream.close();
	}
	else
	{
		printf("Cannot open %s. Please Make sure that the Shader is in the right folder\n", vertex_file_path);
		cout << "Shadler Failed to be Loaded" << endl;
		return;
	}

    GLint Result = GL_FALSE;
	int InfoLogLength;



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(ShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(ShaderID);

	// Check Fragment Shader
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}
}

FragmentShader::~FragmentShader()
{
    printf("Deleting Fragment Shader \n");
    //Removes the Shader
    glDeleteShader(ShaderID);
}

