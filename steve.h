#ifndef STEVE_H
#define STEVE_H

#include "vec3.h"
#include <GL/gl.h>

class Steve {
public:
    vec3 position;
    float size;

    Steve(float size = 0.3f) : position(0.0f, 0.0f, 0.0f), size(size) {}

    void draw() {
        GLfloat vertices[] = {
            -size, -size, -size,  size, -size, -size,  size,  size, -size, -size,  size, -size, // Back face
            -size, -size,  size,  size, -size,  size,  size,  size,  size, -size,  size,  size, // Front face
            -size, -size, -size, -size,  size, -size, -size,  size,  size, -size, -size,  size, // Left face
             size, -size, -size,  size,  size, -size,  size,  size,  size,  size, -size,  size, // Right face
            -size, -size, -size, -size, -size,  size,  size, -size,  size,  size, -size, -size, // Bottom face
            -size,  size, -size, -size,  size,  size,  size,  size,  size,  size,  size, -size  // Top face
        };

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_QUADS, 0, 24);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
};

#endif // STEVE_H
