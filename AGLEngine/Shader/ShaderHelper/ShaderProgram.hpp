#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <iostream>
#include <map>

#include "Shader.hpp"

using std::map;
using std::string;
using std::cout;
using std::endl;


class ShaderProgram
{
	private:
		GLuint programId;   // The unique ID / handle for the shader program
		GLuint shaderCount; // How many shaders are attached to the shader program

		// Map of attributes and their binding locations
		map<string,int> attributeLocList;

		// Map of uniforms and their binding locations
		map<string,int> uniformLocList;

	public:
		// Constructor
		ShaderProgram()
		{
			// Generate a unique Id / handle for the shader program
			// Note: We MUST have a valid rendering context before generating
			// the programId or it causes a segfault!
			programId = glCreateProgram();

			// Initially, we have zero shaders attached to the program
			shaderCount = 0;
		}


		// Destructor
		~ShaderProgram()
		{
			// Delete the shader program from the graphics card memory to
			// free all the resources it's been using
			glDeleteProgram(programId);
		}


		// Method to attach a shader to the shader program
		void attachShader(Shader shader)
		{
			// Attach the shader to the program
			// Note: We identify the shader by its unique Id value
			glAttachShader( programId, shader.getId() );

			// Increment the number of shaders we have associated with the program
			shaderCount++;
		}


		// Method to link the shader program and display the link status
		void linkProgram()
		{
			// If we have at least two shaders (like a vertex shader and a fragment shader)...
			if (shaderCount >= 2)
			{
				// Perform the linking process
				glLinkProgram(programId);

				// Check the status
				GLint linkStatus;
				glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
				if (GL_LINK_STATUS == GL_FALSE)
				{
					cout << "Shader program linking failed." << endl;
					//glfwTerminate();
				}
				else
				{
					cout << "Shader program linking OK." << endl;
				}
			}
			else
			{
				cout << "Can't link shaders - you need at least 2, but attached shader count is only: " << shaderCount << endl;
				//glfwTerminate();
			}
		}


		// Method to enable the shader program
		void use()
		{
			glUseProgram(programId);
		}


		// Method to disable the shader program
		void disable()
		{
			glUseProgram(0);
		}


		// Returns the bound location of a named attribute
		GLuint attribute(const string attribute)
		{
			// You could do this function with the single line:
			//
			//		return attributeLocList[attribute];
			//
			// BUT, if you did, and you asked it for a named attribute
			// which didn't exist, like, attributeLocList["ThisAttribDoesn'tExist!"]
			// then the method would return an invalid value which will likely cause
			// the program to segfault. So we're making sure the attribute asked
			// for exists, and if it doesn't we can alert the user and stop rather than bombing out later.

			// Create an iterator to look through our attribute map and try to find the named attribute
			map<string, int>::iterator it = attributeLocList.find(attribute);

			// Found it? Great -return the bound location! Didn't find it? Alert user and halt.
			if ( it != attributeLocList.end() )
			{
				return attributeLocList[attribute];
			}
			else
			{
				cout << "Could not find attribute in shader program: " << attribute << endl;
				exit(-1);
			}
		}


		// Method to returns the bound location of a named uniform
		GLuint uniform(const string uniform)
		{
			// Note: You could do this method with the single line:
			//
			// 		return uniformLocList[uniform];
			//
			// But we're not doing that. Explanation in the attribute() method above.

			// Create an iterator to look through our uniform map and try to find the named uniform
			static map<string, int>::iterator it = uniformLocList.find(uniform);

			// Found it? Great - pass it back! Didn't find it? Alert user and halt.
			if ( it != uniformLocList.end() )
			{
				return uniformLocList[uniform];
			}
			else
			{
				cout << "Could not find uniform in shader program: " << uniform << endl;
				exit(-1);
			}
		}


		// Method to add an attrbute to the shader and return the bound location
		int addAttribute(const string attributeName)
		{
			attributeLocList[attributeName] = glGetAttribLocation( programId, attributeName.c_str() );

			// Check to ensure that the shader contains an attribute with this name
			if (attributeLocList[attributeName] == -1)
			{
				cout << "Could not add attribute: " << attributeName << " - location returned -1!" << endl;
				exit(-1);
			}
			else
			{
				cout << "Attribute " << attributeName << " bound to location: " << attributeLocList[attributeName] << endl;
			}

			return attributeLocList[attributeName];
		}


		// Method to add a uniform to the shader and return the bound location
		int addUniform(const string uniformName)
		{
			uniformLocList[uniformName] = glGetUniformLocation( programId, uniformName.c_str() );

			// Check to ensure that the shader contains a uniform with this name
			if (uniformLocList[uniformName] == -1)
			{
				cout << "Could not add uniform: " << uniformName << " - location returned -1!" << endl;
				exit(-1);
			}
			else
			{
				cout << "Uniform " << uniformName << " bound to location: " << uniformLocList[uniformName] << endl;
			}

			return uniformLocList[uniformName];
		}

};

#endif // SHADER_PROGRAM_HPP
