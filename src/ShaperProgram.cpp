#include "ShaderProgram.hpp"


ShaderProgram::ShaderProgram()
{
	//Do nothing until real constructor is called.
	this->programID = 0;
}

ShaderProgram::ShaderProgram(string VertexShader, string FragmentShader, vector<AttributeLocation> attributeLocation)
{
	programID = glCreateProgram();
	vertexID = buildShader(VertexShader, GL_VERTEX_SHADER);
	fragmentID = buildShader(FragmentShader, GL_FRAGMENT_SHADER);
	if (vertexID == 0 || fragmentID == 0)
	{
		printf("Error: Shaders did not compile");
	}

	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);

	//add Attribute Locations before linking the program due to Intel driver problems
	for (int i = 0; i < attributeLocation.size(); i++)
	{
		glBindAttribLocation(programID, attributeLocation[i].index, attributeLocation[i].name.c_str());
	}

	glLinkProgram(programID);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);

	printf("Shader building is finished!!!\n");
}

void ShaderProgram::setActiveProgram()
{
	glUseProgram(this->programID);
}

void ShaderProgram::deactivateProgram()
{
	glUseProgram(0);
}

ShaderProgram::~ShaderProgram()
{

}

void ShaderProgram::deleteProgram()
{
	//If they are all not zero, delete them.
	if (programID && vertexID && fragmentID)
	{
		glDetachShader(programID, vertexID);
		glDetachShader(programID, fragmentID);
		glDeleteProgram(programID);
	}
}

void ShaderProgram::setUniform(string name, matrix4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}
void ShaderProgram::setUniform(string name, matrix3& matrix)
{
	glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
}
void ShaderProgram::setUniform(string name, vector3& vec)
{
	glUniform3f(glGetUniformLocation(programID, name.c_str()), vec.x, vec.y, vec.z);
}
void ShaderProgram::setUniform(string name, vector2& vec)
{
	glUniform2f(glGetUniformLocation(programID, name.c_str()), vec.x, vec.y);
}
void ShaderProgram::setUniform(string name, quaternion& quat)
{
	//glUniform4f(glGetUniformLocation(programID, name.c_str()), quat.x, quat.y, quat.z);
}

GLuint ShaderProgram::buildShader(string location, GLuint type)
{
	GLuint ShaderID = glCreateShader(type);

	const char * file_path = location.c_str();

	// Read the  Shader code from the file
	std::string ShaderCode;
	std::ifstream ShaderStream(file_path, std::ios::in);

	if (ShaderStream.is_open())
	{
		std::string Line = "";
		while (getline(ShaderStream, Line))
		{
			ShaderCode += "\n" + Line;
		}
		ShaderStream.close();
	}
	else
	{
		printf("Error %s File not found\n", file_path);
		cout << "Shadler Failed to be Loaded\n" << endl;
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile  Shader
	printf("Compiling shader : %s\n", file_path);
	char const * SourcePointer = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &SourcePointer, NULL);
	glCompileShader(ShaderID);

	// Check  Shader
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		printf("%s\n", &ShaderErrorMessage[0]);
	}

	return ShaderID;
}
