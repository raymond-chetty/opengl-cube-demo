#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// ============================================================================
// SHADER SOURCES
// ============================================================================

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 vertexColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   vertexColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "in vec3 vertexColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(vertexColor, 1.0);\n"
    "}\n\0";

// ============================================================================
// VERTEX DATA
// ============================================================================

void getCubeVertices(float *vertices, int *vertexCount) {
    // Cube with 8 vertices, 36 indices for 12 triangles (2 per face)
    // Each vertex: 3 positions + 3 color values
    
    float w = 0.5f;  // Half-width of cube
    
    float cubeData[] = {
        // Front face (Red)
        -w, -w,  w,    1.0f, 0.0f, 0.0f,
         w, -w,  w,    1.0f, 0.0f, 0.0f,
         w,  w,  w,    1.0f, 0.0f, 0.0f,
        -w,  w,  w,    1.0f, 0.0f, 0.0f,
        
        // Back face (Green)
        -w, -w, -w,    0.0f, 1.0f, 0.0f,
         w, -w, -w,    0.0f, 1.0f, 0.0f,
         w,  w, -w,    0.0f, 1.0f, 0.0f,
        -w,  w, -w,    0.0f, 1.0f, 0.0f,
        
        // Top face (Blue)
        -w,  w, -w,    0.0f, 0.0f, 1.0f,
         w,  w, -w,    0.0f, 0.0f, 1.0f,
         w,  w,  w,    0.0f, 0.0f, 1.0f,
        -w,  w,  w,    0.0f, 0.0f, 1.0f,
        
        // Bottom face (Yellow)
        -w, -w, -w,    1.0f, 1.0f, 0.0f,
         w, -w, -w,    1.0f, 1.0f, 0.0f,
         w, -w,  w,    1.0f, 1.0f, 0.0f,
        -w, -w,  w,    1.0f, 1.0f, 0.0f,
        
        // Right face (Cyan)
         w, -w, -w,    0.0f, 1.0f, 1.0f,
         w,  w, -w,    0.0f, 1.0f, 1.0f,
         w,  w,  w,    0.0f, 1.0f, 1.0f,
         w, -w,  w,    0.0f, 1.0f, 1.0f,
        
        // Left face (Magenta)
        -w, -w, -w,    1.0f, 0.0f, 1.0f,
        -w,  w, -w,    1.0f, 0.0f, 1.0f,
        -w,  w,  w,    1.0f, 0.0f, 1.0f,
        -w, -w,  w,    1.0f, 0.0f, 1.0f,
    };
    
    int dataSize = sizeof(cubeData);
    for (int i = 0; i < dataSize / sizeof(float); i++) {
        vertices[i] = cubeData[i];
    }
    *vertexCount = 24;  // 6 faces * 4 vertices each
}

void getCubeIndices(unsigned int *indices, int *indexCount) {
    unsigned int cubeIndices[] = {
        // Front
        0, 1, 2,
        0, 2, 3,
        // Back
        4, 6, 5,
        4, 7, 6,
        // Top
        8, 9, 10,
        8, 10, 11,
        // Bottom
        12, 14, 13,
        12, 15, 14,
        // Right
        16, 17, 18,
        16, 18, 19,
        // Left
        20, 22, 21,
        20, 23, 22,
    };
    
    for (int i = 0; i < 36; i++) {
        indices[i] = cubeIndices[i];
    }
    *indexCount = 36;
}

// ============================================================================
// SHADER COMPILATION
// ============================================================================

unsigned int compileShader(const char *source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    // Check for errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Shader compilation failed:\n%s\n", infoLog);
        exit(1);
    }
    
    return shader;
}

unsigned int createShaderProgram(void) {
    unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Shader program linking failed:\n%s\n", infoLog);
        exit(1);
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main(void) {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Cube Demo", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *w, int width, int height) {
        glViewport(0, 0, width, height);
    });
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(glewStatus));
        return -1;
    }
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    // Create shader program
    unsigned int shaderProgram = createShaderProgram();
    
    // Prepare vertex data
    float vertices[24 * 6];  // 24 vertices * 6 floats (pos + color)
    int vertexCount;
    getCubeVertices(vertices, &vertexCount);
    
    unsigned int indices[36];
    int indexCount;
    getCubeIndices(indices, &indexCount);
    
    // Create VAO, VBO, and EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    // Fill VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Fill EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Animation loop
    double lastTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(window)) {
        // Check for ESC key
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        
        // Calculate delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use shader program
        glUseProgram(shaderProgram);
        
        // Create transformations
        glm::mat4 model = glm::rotate(
            glm::mat4(1.0f),
            (float)(currentTime * 1.5f),  // Rotation speed
            glm::vec3(1.0f, 1.0f, 0.5f)   // Rotation axis
        );
        
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 2.0f),  // Camera position
            glm::vec3(0.0f, 0.0f, 0.0f),  // Look at point
            glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
        );
        
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),           // FOV
            800.0f / 600.0f,               // Aspect ratio
            0.1f,                          // Near plane
            100.0f                         // Far plane
        );
        
        // Pass matrices to shader
        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int viewLoc = glGetUniformLocation(shaderProgram, "view");
        int projLoc = glGetUniformLocation(shaderProgram, "projection");
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // Draw cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
