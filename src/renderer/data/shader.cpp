#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <regex>
#include <unordered_set>
#include <filesystem>

void create_shader(Shader* rpShader, const char* rVertexShaderFile, const char* rFragmentShaderFile)
{
	int  success;
	char infoLog[512];

	// vvv Vertex shader --------------
	// Read vertex shader file
	std::string vertexStr;
	if (read_shader_from_file(vertexStr, rVertexShaderFile))
	{
		std::cout << "Error when creating vertex shader!" << std::endl;
	}
	const char* vertexChars = vertexStr.c_str();

	// Create vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexChars, nullptr);
	glCompileShader(vertexShader);

	// Check for compilation errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		std::string filenameOnly = std::filesystem::path(rVertexShaderFile).filename().string();
		std::string finalPath = "shaders_out/" + filenameOnly;
		
		std::cout << "Writing processed shader in " << finalPath << std::endl;

		std::filesystem::create_directories("shaders_out");
		std::ofstream out(finalPath);
		out << vertexStr;
		out << "\n############################################\n" << infoLog;
		out.close();
	} 
	// ^^^ ----------------------------

	// vvv Fragment shader ------------
	
	std::string fragStr;
	if (read_shader_from_file(fragStr, rFragmentShaderFile))
	{
		std::cout << "Error when creating fragment shader!" << std::endl;
	}
	const char* fragChars = fragStr.c_str();

	// Create Fragment shader
	unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragChars, nullptr);
	glCompileShader(fragShader);

	// Check for compilation errors
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ^^^ ----------------------------

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING FAILED\n" << infoLog << std::endl;

		std::string filenameOnly = std::filesystem::path(rFragmentShaderFile).filename().string();
		std::string finalPath = "shaders_out/" + filenameOnly;

		std::cout << "Writing processed shader in " << finalPath << std::endl;

		std::filesystem::create_directories("shaders_out");
		std::ofstream out(finalPath);
		out << fragStr;
		out << "\n############################################\n" << infoLog;
		out.close();
	}

	// Delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	rpShader->ShaderProgram = shaderProgram;
	rpShader->ModelMatrixUniform = glGetUniformLocation(rpShader->ShaderProgram, "Model");
	rpShader->ViewMatrixUniform = glGetUniformLocation(rpShader->ShaderProgram, "View");
	rpShader->ProjectionMatrixUniform = glGetUniformLocation(rpShader->ShaderProgram, "Projection");
}

int process_shader_includes(std::string& rStr, const std::string& rPath, std::unordered_set<std::string>& includedFiles);

int read_shader_from_file(std::string& rStr, const char* rPath)
{
	std::unordered_set<std::string> includedFiles;
	return process_shader_includes(rStr, rPath, includedFiles);
}

int process_shader_includes(std::string& rStr, const std::string& rPath, std::unordered_set<std::string>& includedFiles)
{
	if (includedFiles.count(rPath))
	{
		std::cerr << "WARNING::SHADER::RECURSIVE_INCLUDE_SKIPPED for file: " << rPath << std::endl;
		return 0;
	}

	includedFiles.insert(rPath);

	std::ifstream file;

	try
	{
		file.open(rPath);
		std::stringstream shaderStream;
		std::string line;

		while (std::getline(file, line))
		{
			std::smatch match;
			std::regex includeRegex("^\\s*#include\\s+\"([^\"]+)\"\\s*");

				if (std::regex_match(line, match, includeRegex))
				{
					std::string includePath = match[1].str();
					std::string includeContent;
					int result = process_shader_includes(includeContent, includePath, includedFiles);
					if (result != 0)
					{
						return result;
					}
					shaderStream << "// Begin include: " << includePath << "\n";
					shaderStream << includeContent << "\n";
					shaderStream << "// End include: " << includePath << "\n";
				}
				else
				{
					shaderStream << line << "\n";
				}
		}

		file.close();
		rStr = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << rPath << ", " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
