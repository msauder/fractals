// CS315 GLUT Template
// Sierpinski gasket with perturbations
// Using template provided by Alex Clarke in CS 315 lab
// Using example4 from textbook as a reference
// Additions made by Matthew Sauder

#include "Angel.h"
#include "vec2.h"

const int NumTimesToSubdivide = 4;
const int NumTetrahedrons = 256;            // 4^5 tetrahedrons
const int NumTriangles = 4*NumTetrahedrons;  // 4 triangles / tetrahedron
const int NumVertices = 3*NumTriangles;      // 3 vertices / triangle

vec2 points[NumVertices]; // points to draw, filled during initialization
int i = 0; // index for points
GLint uColour;

#ifdef __APPLE__
#ifndef GLUT_3_2_CORE_PROFILE
#define GLUT_3_2_CORE_PROFILE 0
#define glBindVertexArray		glBindVertexArrayAPPLE
#define glDeleteVertexArrays	glDeleteVertexArraysAPPLE
#define glIsVertexArray			glIsVertexArrayAPPLE
#define glGenVertexArrays  	glGenVertexArraysAPPLE
#endif
#endif

void perturb (vec2& a, GLfloat limit) {
    // Generate random displacements dx and dy between 0 and limit
    GLfloat dx = (GLfloat)rand()/((GLfloat)RAND_MAX/limit);
    GLfloat dy = (GLfloat)rand()/((GLfloat)RAND_MAX/limit);
    
    // Change sign of dx and dy based on coin flips (50% chance)
    float coin_flip = (float)rand()/RAND_MAX;
    if (coin_flip > 0.5)
        dx = -dx;
    
    coin_flip = (float)rand()/RAND_MAX;
    if (coin_flip > 0.5)
        dy = -dy;
    
    // Return perturbed values using pass-by-reference
    a.x += dx;
    a.y += dy;
}

void triangle (vec2& a, vec2& b, vec2& c) {
    // Add each vertice to the array of points to draw later
    points[i] = a; i++;
    points[i] = b; i++;
    points[i] = c; i++;
}

void divide_triangle (vec2& a, vec2& b, vec2& c, int count) {
    if (count > 0) {
        // Find midpoints between each of the triangle sides
        vec2 v0 = (a + b) / 2.0;
        vec2 v1 = (b + c) / 2.0;
        vec2 v2 = (c + a) / 2.0;
        
        // Determine limit for perturbation (L / 16.0)
        GLfloat d0 = length(v0) / 16.0;
        GLfloat d1 = length(v1) / 16.0;
        GLfloat d2 = length(v2) / 16.0;
        
        // Perturb each of the vectors
        perturb(v0, d0);
        perturb(v1, d1);
        perturb(v2, d2);
    
        // Recursively subdivide each of the smaller triangles
        divide_triangle(a, v0, v2, count - 1);
        divide_triangle(v0, b, v1, count - 1);
        divide_triangle(v2, v1, c, count - 1);
    }
    else {
        // Recursively exit by passing our points to be handled by triangle method
        triangle(a, b, c);
    }
}

// OpenGL initialization
void init()
{
    vec2 vertices[3] = {
        vec2(-0.80f,-0.80f),
        vec2( 0.80f,-0.80f),
        vec2( 0.0f , 0.80f)
    };
    
    divide_triangle(vertices[0], vertices[1], vertices[2], NumTimesToSubdivide);
    
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // *** Position buffer **********************
    // Create a buffer for vertex positions, make it active, and copy data to it
    GLuint positionBuffer;
    glGenBuffers( 1, &positionBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, positionBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f ); //clear colour is black
    glClearDepth(1.0f); //Clear to maximum distance
    
    //Enable the shader's vertex position input and attach the active buffer
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    
    uColour = glGetUniformLocation(program, "uColour");
    
}

//----------------------------------------------------------------------------

void display( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glUniform4f(uColour, 1.0f, 1.0f, 1.0f, 1.0f); // White
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void reshape (int w, int h)
{
	//Put Call to Size Changing function Here
    
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
	switch( key ) {
		case 033:  // Escape key
		case 'q': case 'Q':
			exit( EXIT_SUCCESS );
			break;
	}
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
	glutInit( &argc, argv );
	glutInitWindowSize( 512, 512 );
    
#ifdef __APPLE__
	glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
#else
	// If you are using freeglut, the next two lines will check if
	// the code is truly 3.2. Otherwise, comment them out
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    
#endif
    
    glutCreateWindow( "OpenGL Template" );
    glewExperimental = GL_TRUE;
	glewInit();
    
	init();
    
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutReshapeFunc(reshape);
    
	glutMainLoop();
	return 0;
}
