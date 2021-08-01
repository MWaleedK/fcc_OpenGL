//This file has the shaders and vertices inside it. All it will need are glad and glfw to run
#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


//Vertex Shader Source Code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader Source Code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.8f,0.3f,0.02f,1.0f);\n"
"}\n\0";

int main()
{	//Initialize GLFW
	glfwInit();

	//Give hits to GLFW so that it understands the version and Profile used
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Profile used: Core, using modern OPENGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	//vertecies co-ordinates
	GLfloat vertices[] =
	{
		-0.5f,-0.5f*float(sqrt(3))/3,0.0f,//lower left
		0.5f,-0.5f * float(sqrt(3)) / 3,0.0f,//lower right
		0.0f,0.5f * float(sqrt(3))*2 / 3,0.0f,//Top center
		-0.5f/2,0.5f * float(sqrt(3)) / 6,0.0f,//Inner left
		0.5f/2,0.5f * float(sqrt(3)) / 6,0.0f,//Inner right
		0.0f,-0.5f * float(sqrt(3))/ 3,0.0f,//Inner down

	};

	GLuint indices[] = {
		0,3,5,//lower left triangle
		3,2,4,//lower right triangle
		5,4,1//upper right triangle
	};


	//Create a window 800x800 named myWindow which is not full screen
	GLFWwindow* window = glfwCreateWindow(800, 800, "myWindow", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//select 'window' as the current work area
	glfwMakeContextCurrent(window);

	//load glad and create a work areaon whic to put colors
	gladLoadGL();
	glad_glViewport(0,0,800,800);//work area is set to windowSize
	


	//Create Vertex shader oject and get reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	//Attach Vertex Shader source to the Vertex shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compile vertex shader into machine code for the GPU to understand
	glCompileShader(vertexShader);

	//Create a fragment shader Object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	//Attach fragment shader Source to fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//Compile fragment shader into machine code for the GPU
	glCompileShader(fragmentShader);

	//create shader program reference and get its reference
	GLuint shaderProgram = glCreateProgram();

	//Attach vertex and fragment shader to shader program
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//Wrap-up/link all the shader programs together into the Shader Program
	glLinkProgram(shaderProgram);

	//Delte the Shader and Vertex objects as both now exist in a linked state in shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	//Create reference for Vertex Array Object and Vertex Buffer Object,EBO is Index buffer Object
	GLuint VAO, VBO,EBO;

	//Generated VAO, VBO and EBO with only 1 object each
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//Make VAO current object by binding it
	glBindVertexArray(VAO);
	
	//Bind VBO to state that it's a GL_BUFFER_ARRAY
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	//Introduce vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	//Make EBO current, bind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	//Link indices array with EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Configure the Vertex Attriute so OpenL knows how to read the buffer....
	//Here we draw a triangle
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	//Enable Vertex Attribbute so that openGL knows to use it
	glEnableVertexAttribArray(0);

	//Bing both VAO and VBO to 0 so they don't get overwritten
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//EBO is unbbound after VAO is unbound sice EBO is store in VAO
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	
	
	
	glClearColor(0.07f,0.13f,0.17f,1.0f);

	glClear(GL_COLOR_BUFFER_BIT);//back buffer set
	glfwSwapBuffers(window);//switch front buffer with the freshly created back buffer

	while (!glfwWindowShouldClose(window))//exit window on pressing close
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Tell OpenGL which shader program we want to use
		glUseProgram(shaderProgram);
		//Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);
		//Draw the triangles
		glDrawElements(GL_TRIANGLES,9,GL_UNSIGNED_INT,0);
		glfwSwapBuffers(window);
		glfwPollEvents();//check if close pressed
		
	}

	//delete objects we created
	glDeleteVertexArrays(1,&VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1,&EBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);//clean execution destroy viewport
	glfwTerminate();//destroy window
	return 0;
}
