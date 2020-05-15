#include "Shader.h"

using namespace std;
using namespace glm;

Shader::Shader()
{
	this->program = 0;
}

bool Shader::load(std::string fileName, GLenum shaderType)
{
	// TODO: check shader type
	if (shaderType == GL_VERTEX_SHADER)
		cout << "GL_VERTEX_SHADER\n";
	if (shaderType == GL_FRAGMENT_SHADER)
		cout << "GL_FRAGMENT_SHADER\n";

	// Open the file
	ifstream file(fileName);

	// Read from the file
	if (!file.is_open())
	{
		cerr << "error: couldn't open the file.\n";
		return false;
	}
	const int buf_size = 10000;
	char buffer[buf_size];
	memset(buffer, 0, buf_size);
	file.read(buffer, buf_size);
	GLint size = strlen(buffer);
	
	// Close the file
	file.close();
	
	if (!program)					 // Program object was not created
		program = glCreateProgram(); // Create the object

	shader_paths[shaderType] = fileName;
	//ShaderManager& manager = ShaderManager::getInstance();
	//manager.AddShader(this);
	
	// Working with shader
	glUseProgram(0);
	GLuint shader = glCreateShader(shaderType);
	char* source[1] = { buffer };
	glShaderSource(shader, 1, source, &size);	// Load shader source
	glCompileShader(shader);
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE)	// Shader was not compiled
	{
		if (compileStatus == GL_INVALID_VALUE)
			cout << "SHADER ERROR: GL_INVALID_VALUE\n";
		if (compileStatus == GL_INVALID_OPERATION)
			"SHADER ERROR: GL_INVALID_OPERATION\n";
		cout << compileStatus << endl;
		glGetShaderInfoLog(shader, buf_size, nullptr, buffer);
		cout << buffer; // Out an error string
		return false;
	}

	// Check if shader is already attached and attach it
	auto itAttachedShader = attachedShaders.find(shaderType);
	if (itAttachedShader != attachedShaders.end())
	{
		// cout << "Reattaching shader...\n";
		glDetachShader(program, itAttachedShader->second);
		//uniforms.clear();
		// cout << "Shader was reattached succesfully!\n";
	}
	glAttachShader(program, shader); // Attach it to the program
	attachedShaders[shaderType] = shader;

	// Link the program
	glLinkProgram(program);
	GLint link_status;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (link_status != GL_TRUE)	// Program was not linked
	{
		glGetProgramInfoLog(program, buf_size, nullptr, buffer); // Out an error string
		cout << buffer;
		return false;
	}
	
	glDeleteShader(shader);
	return true;
}

void Shader::activate()
{
	glUseProgram(program);
}

void Shader::deactivate()
{
	glUseProgram(0);
}

void Shader::setUniform(const std::string& name, const glm::vec4& value)
{
	uniforms[name] = getUniformLocation(name);
	glUniform4fv(uniforms[name], 1, value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat4& value)
{
	uniforms[name] = getUniformLocation(name);
	glUniformMatrix4fv(uniforms[name], 1, GL_FALSE, value_ptr(value)); // Error may be here
}

void Shader::setUniform(const std::string& name, const glm::mat4x2& value)
{
	uniforms[name] = getUniformLocation(name);
	glUniformMatrix4x2fv(uniforms[name], 1, GL_FALSE, value_ptr(value)); // Error may be here
}

void Shader::setUniform(const std::string& name, const glm::vec2& value)
{
	uniforms[name] = getUniformLocation(name);
	glUniformMatrix2fv(uniforms[name], 1, GL_FALSE, value_ptr(value)); // Error may be here
}

void Shader::setUniform(const std::string& name, std::vector<glm::mat4>& value)
{
	uniforms[name] = getUniformLocation(name);
	glUniformMatrix4fv(uniforms[name], value.size(), GL_FALSE, &value[0][0][0]);
}

void Shader::setUniform(const std::string& name, float value)
{
	uniforms[name] = getUniformLocation(name);
	glUniform1f(uniforms[name], value);
}


std::map<GLuint, std::string>* Shader::getPaths()
{

	return &shader_paths;
}

std::map<GLuint, INT64>* Shader::getLastTimes()
{
	return &last_times;
}

GLuint Shader::getProgram()
{
	return program;
}


GLuint Shader::getUniformLocation(const std::string& name)
{
	return glGetUniformLocation(program, name.c_str());
}