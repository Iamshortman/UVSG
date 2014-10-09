#include "ShaderProgram.hpp"

ShaderProgram::ShaderProgram(string vertexLoc, string fragmentLoc)
:
vertexShader(vertexLoc),
fragmentShader(fragmentLoc)
{

    // Link the program
	printf("Linking program\n");
	this->ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertexShader.ShaderID);
	glAttachShader(ShaderProgramID, fragmentShader.ShaderID);
	glLinkProgram(ShaderProgramID);


	GLint Result = GL_FALSE;
	int InfoLogLength;
		// Check the program
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ShaderProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 )
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ShaderProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
}

void ShaderProgram::setActiveProgram()
{
    //printf("Setting Active Program \n");
    glUseProgram(this->ShaderProgramID);
}

ShaderProgram:: ~ShaderProgram()
{
    printf("Deleting Shader Program \n");
	glDetachShader(ShaderProgramID, vertexShader.ShaderID);
	glDetachShader(ShaderProgramID, fragmentShader.ShaderID);
	glDeleteProgram(ShaderProgramID);

}
