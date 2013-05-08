// CS315 GLUT Template
// Koch Snowflake
// Using template provided by Alex Clarke in CS 315 lab
// Additions made by Matthew Sauder

#include "Angel.h"
#include "vec2.h"

const int NumTimesToSubdivide = 5;
const int NumVertices = 3072;      // 3 * 4 ^ NumTimesToSubdivide
const int NumPoints = 2 * NumVertices; // Two points for every vertex
const float tan60 = 1.732050807568877;

vec2 points[NumPoints]; // points to draw, filled during initialization
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

// Subdivide a line segment so that three equal line segments becomes 4 equal line segments
//         b
//         /\
//        /  \
//       /    \
// p0---/      \----p1
//     a        c
void divide_segment (const vec2& p0, const vec2& p1, int count) {
    vec2 a = (p1 - p0) / 3 + p0;        // Find the point along the initial segment that is 1/3 along
    vec2 c = 2 * (p1 - p0) / 3 + p0;    // Find the point along the initial segment that is 2/3 along
    vec2 v1 = (c - a) / 2;              // Find the vector which points from A to midpoint M
    vec2 perpv1(-v1.y, v1.x);           // Find vector perpendicular to v1
    vec2 v2 = v1 + perpv1 * tan60;      // Use 30-60-90 right angle triangle to find v2
    vec2 b = a + v2;                    // Find C using A and our new vector v2
    if (count > 0) {
        // Recursively subdivide each of the generated segments
        divide_segment(p0, a, count - 1);
        divide_segment(a, b, count - 1);
        divide_segment(b, c, count - 1);
        divide_segment(c, p1, count - 1);
    }
    else {
        // Add each pair of points to our array for drawing later
        points[i] = p0; i++;
        points[i] = a; i++;
        points[i] = a; i++;
        points[i] = b; i++;
        points[i] = b; i++;
        points[i] = c; i++;
        points[i] = c; i++;
        points[i] = p1; i++;
    }
}

// Divide line segments which make up an initial triangle ABC
//         b
//         /\
//        /  \
//       /    \
//      /      \
//     a--------c
//
void divide_triangle (vec2& a, vec2& b, vec2& c, int count) {
    divide_segment(a, b, count);
    divide_segment(b, c, count);
    divide_segment(c, a, count);
}

// OpenGL initialization
void init()
{
    vec2 vertices[3] = {
        vec2(-0.816497, -0.471405),
        vec2( 0.0,       0.942809),
        vec2( 0.816497, -0.471405)
    };
    
    // Call for the "Koch Snowflake"-afying of the 3 initial vertices
    if (NumTimesToSubdivide != 0) {
        divide_triangle(vertices[0], vertices[1], vertices[2], NumTimesToSubdivide - 1);
    }
    else { // Or just add the three points to later draw a triangle
        points[0] = vertices[0];
        points[1] = vertices[1];
        points[2] = vertices[2];
    }
    
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
    glDrawArrays( GL_LINES, 0, NumPoints );
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
