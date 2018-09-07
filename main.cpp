/**
 * This project (GLSL_Project1) is created by Fahim Hasan Khan (fahimhasan.khan [at] ucalgary.ca)
 * based on the works of Andrew Robert Owens (arowens [at] ucalgary.ca)
 * who is a PhD student at Graphics Jungle, University of Calgary
 *
 * File:	main.cpp
 *
 * Summary:
 *
 * This is a (very) basic program to
 * 1) load shaders from external files, and make a shader program
 * 2) make Vertex Array Object and Vertex Buffer Object for the quad
 *
 **/

#include <iostream>
#include <cmath>
#include <chrono>
#include <limits>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "ShaderTools.h"
#include "Vec3f.h"
#include "Mat4f.h"
#include "OpenGLMatrixTools.h"
#include "Camera.h"



//==================== GLOBAL VARIABLES ====================//
/*	Put here for simplicity. Feel free to restructure into
*	appropriate classes or abstractions.
*/

// Drawing Program
GLuint basicProgramID;

// Data needed for Quad
GLuint vaoID;
GLuint vertBufferID;
Mat4f M;

GLuint vaofbrecID;   /// front back rectangle
GLuint vertBufferfbrecID;
Mat4f Mfbrec;

GLuint vaorlrecID; /// object of rec - right n left
GLuint vertBufferrlrecID;
Mat4f Mrlrec;


GLuint vaotringleID; /// object of tringle
GLuint vertBuffertringleID;
Mat4f Mtringle;


GLuint vaoroofID; /// object of roof
GLuint vertBufferroofID;
Mat4f Mroof;


GLuint vaobackroofID; /// object of back roof
GLuint vertBufferbackroofID;
Mat4f Mbackroof;


GLuint vaodoorID; /// object of door
GLuint vertBufferdoorID;
Mat4f Mdoor;


GLuint vaowinID; /// object of windows(2ta)
GLuint vertBufferwinID;
Mat4f Mwin;

// Data needed for Line


// Only one camera so only one veiw and perspective matrix are needed.
Mat4f V;
Mat4f P;

// Only one thing is rendered at a time, so only need one MVP
// When drawing different objects, update M and MVP = M * V * P
Mat4f MVP;

// Camera and veiwing Stuff
Camera camera;
int g_moveUpDown = 0;
int g_moveLeftRight = 0;
int g_moveBackForward = 0;
int g_rotateLeftRight = 0;
int g_rotateUpDown = 0;
int g_rotateRoll = 0;
float g_rotationSpeed = 0.015625;
float g_panningSpeed = 0.25;
bool g_cursorLocked;
float g_cursorX, g_cursorY;

bool g_play = false;

int WIN_WIDTH = 800, WIN_HEIGHT = 600;
int FB_WIDTH = 800, FB_HEIGHT = 600;
float WIN_FOV = 10;
float WIN_NEAR = 0.01;
float WIN_FAR = 1000;

//================= FUNCTION DECLARATIONS ====================//
void displayFunc();
void resizeFunc();
void init();
void generateIDs();
void deleteIDs();
void setupVAO();
void loadQuadGeometryToGPU();
void loadfbrecGeometryToGPU(); /// fb rec
void loadrlrecGeometryToGPU(); /// rec - rl
void loadtringleGeometryToGPU(); /// tringles
void loadroofGeometryToGPU(); /// roof
void loadbackroofGeometryToGPU(); /// b roof
void loaddoorGeometryToGPU(); /// door
void loadWinGeometryToGPU(); /// for window
void reloadProjectionMatrix();
void loadModelViewMatrix();
void setupModelViewProjectionTransform();

void windowSetSizeFunc();
void windowKeyFunc(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
void windowMouseMotionFunc(GLFWwindow *window, double x, double y);
void windowSetSizeFunc(GLFWwindow *window, int width, int height);
void windowSetFramebufferSizeFunc(GLFWwindow *window, int width, int height);
void windowMouseButtonFunc(GLFWwindow *window, int button, int action,
                           int mods);
void windowMouseMotionFunc(GLFWwindow *window, double x, double y);
void windowKeyFunc(GLFWwindow *window, int key, int scancode, int action,
                   int mods);
void animateQuad(float t);
void moveCamera();
void reloadMVPUniform();
void reloadColorUniform(float r, float g, float b);
std::string GL_ERROR();
int main(int, char **);

//==================== FUNCTION DEFINITIONS ====================//

void displayFunc() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Use our shader
  glUseProgram(basicProgramID);


  /// ===== DRAW front back rectangle ====== //
  MVP = P * V * Mfbrec;
  reloadMVPUniform();
  reloadColorUniform(.5, .75, .25);

  // Use VAO that holds buffer bindings
  // and attribute config of buffers
  glBindVertexArray(vaofbrecID);
  // Draw Quads, start at vertex 0, draw 4 of them (for a quad)
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDrawArrays(GL_TRIANGLES, 4, 7);
   glDrawArrays(GL_TRIANGLES, 7, 10);




   /// ===== DRAW left right rec ====== //
  MVP = P * V * Mrlrec;
  reloadMVPUniform();
  reloadColorUniform(.75, 0.75, .25);

  // Use VAO that holds buffer bindings
  // and attribute config of buffers
  glBindVertexArray(vaorlrecID);
  // Draw Quads, start at vertex 0, draw 4 of them (for a quad)
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glDrawArrays(GL_TRIANGLES, 4, 7);
  glDrawArrays(GL_TRIANGLES, 7, 10);





   /// ===== DRAW  tringle ====== //
  MVP = P * V * Mtringle;
 // reloadMVPUniform();
  reloadColorUniform(.5, 0.5, .25);

  // Use VAO that holds buffer bindings
  // and attribute config of buffers
  glBindVertexArray(vaotringleID);
  // Draw Quads, start at vertex 0, draw 4 of them (for a quad)
  glDrawArrays(GL_TRIANGLES, 0, 3);
glDrawArrays(GL_TRIANGLES, 3, 6);




 /// ===== DRAW  roof ====== //
  MVP = P * V * Mroof;
  reloadMVPUniform();
  reloadColorUniform(.55, 0.35, .2);

  // Use VAO that holds buffer bindings
  // and attribute config of buffers
  glBindVertexArray(vaoroofID);
  // Draw Quads, start at vertex 0, draw 4 of them (for a quad)
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



   /// ===== DRAW back roof ====== //
  MVP = P * V * Mbackroof;
  reloadMVPUniform();
  reloadColorUniform(.55, 0.35, .2);

  // Use VAO that holds buffer bindings
  // and attribute config of buffers
  glBindVertexArray(vaobackroofID);
  // Draw Quads, start at vertex 0, draw 4 of them (for a quad)
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


/// ===== DRAW door ====== //
  MVP = P * V * Mdoor;
  reloadMVPUniform();
  reloadColorUniform(.15, 0.2, .3);

  // Use VAO that holds buffer bindings
  // and attribute config of buffers
  glBindVertexArray(vaodoorID);
  // Draw Quads, start at vertex 0, draw 4 of them (for a quad)

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



 /// ===== DRAW window ====== //
  MVP = P * V * Mwin;
  reloadMVPUniform();
  reloadColorUniform(.3, 0.2, .2);

  // Use VAO that holds buffer bindings
  // and attribute config of buffers
  glBindVertexArray(vaowinID);
  // Draw Quads, start at vertex 0, draw 4 of them (for a quad)

 glDrawArrays(GL_TRIANGLES, 0, 3);
 glDrawArrays(GL_TRIANGLES, 3, 6);
 glDrawArrays(GL_TRIANGLES, 6, 9);
 glDrawArrays(GL_TRIANGLES, 9, 12);



}

void animateQuad(float t) {

    Mwin = RotateAboutYMatrix(100 * t);
    Mdoor = RotateAboutYMatrix(100 * t);
    Mbackroof = RotateAboutYMatrix(100 * t);
    Mroof = RotateAboutYMatrix(100 * t);
    Mtringle = RotateAboutYMatrix(100 * t);
    Mrlrec = RotateAboutYMatrix(100 * t);
    Mfbrec = RotateAboutYMatrix(100 * t);
  float s = (std::sin(t) + 1.f) / 2.f;
  float x = (1 - s) * (10) + s * (-10);

  M = TranslateMatrix(x, 0, 0) *( Mwin+Mdoor+Mbackroof+Mroof+Mtringle+Mrlrec+Mfbrec);

  setupModelViewProjectionTransform();
  reloadMVPUniform();
}





void loadfbrecGeometryToGPU() {
  // Just basic layout of floats, for a quad
  // 3 floats per vertex, 4 vertices
  std::vector<Vec3f> verts;



   ///* front rec */
     verts.push_back(Vec3f (0.2, 0.1, 0.0));
     verts.push_back(Vec3f (0.2, 0.575, 0.0));
     verts.push_back(Vec3f (0.9, 0.1, 0.0));
     verts.push_back(Vec3f (0.9, 0.575, 0.0));



    ///*back rec*/
     verts.push_back(Vec3f (0.2, 0.1, 0.5));
     verts.push_back(Vec3f (0.2, 0.575, 0.5));
     verts.push_back(Vec3f (0.9, 0.575, 0.5));


     verts.push_back(Vec3f (0.2, 0.1, 0.5));
     verts.push_back(Vec3f (0.9, 0.575, 0.5));
     verts.push_back(Vec3f (0.9, 0.1, 0.5));







  glBindBuffer(GL_ARRAY_BUFFER, vertBufferfbrecID);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 10, // byte size of Vec3f, 4 of them
               verts.data(),      // pointer (Vec3f*) to contents of verts
               GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}







void loadrlrecGeometryToGPU() {
  // Just basic layout of floats, for a quad
  // 3 floats per vertex, 4 vertices
  std::vector<Vec3f> verts;

    ///* left rec */

     verts.push_back(Vec3f (0.2, 0.1, 0.5));
      verts.push_back(Vec3f (0.2, 0.575, 0.5));
     verts.push_back(Vec3f (0.2, 0.1, 0.0));
     verts.push_back(Vec3f (0.2, 0.575, 0.0));



    ///* right rec */

         verts.push_back(Vec3f (0.9, 0.575, 0.0));
         verts.push_back(Vec3f (0.9, 0.575, 0.5));
          verts.push_back(Vec3f (0.9, 0.1, 0.5));



          verts.push_back(Vec3f (0.9, 0.1, 0.5));
           verts.push_back(Vec3f (0.9, 0.1, 0.0));
            verts.push_back(Vec3f (0.9, 0.575, 0.0));



  glBindBuffer(GL_ARRAY_BUFFER, vertBufferrlrecID);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 10, // byte size of Vec3f, 4 of them
               verts.data(),      // pointer (Vec3f*) to contents of verts
               GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}





void loadtringleGeometryToGPU() {
  // Just basic layout of floats, for a quad
  // 3 floats per vertex, 4 vertices
  std::vector<Vec3f> verts;

    ///* left tri */

     verts.push_back(Vec3f (0.9, 0.575, 0.0));
     verts.push_back(Vec3f (0.9, 0.575, 0.5));
     verts.push_back(Vec3f (0.9, 0.8, 0.25));


    ///* right tri */

     verts.push_back(Vec3f (0.2, 0.575, 0.0));
     verts.push_back(Vec3f (0.2, 0.575, 0.5));
     verts.push_back(Vec3f (0.2, 0.8, 0.25));









  glBindBuffer(GL_ARRAY_BUFFER, vertBuffertringleID);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 6, // byte size of Vec3f, 4 of them
               verts.data(),      // pointer (Vec3f*) to contents of verts
               GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}




void loadroofGeometryToGPU() {
  // Just basic layout of floats, for a quad
  // 3 floats per vertex, 4 vertices
  std::vector<Vec3f> verts;


    ///* roof */
    //polygon
     verts.push_back(Vec3f (0.2, 0.575, 0.0));
     verts.push_back(Vec3f (0.2, 0.8, 0.25));
     verts.push_back(Vec3f (0.9, 0.575, 0.0));
     verts.push_back(Vec3f (0.9, 0.8, 0.25));


  glBindBuffer(GL_ARRAY_BUFFER, vertBufferroofID);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 4, // byte size of Vec3f, 4 of them
               verts.data(),      // pointer (Vec3f*) to contents of verts
               GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}


void loadbackroofGeometryToGPU() {
  // Just basic layout of floats, for a quad
  // 3 floats per vertex, 4 vertices
  std::vector<Vec3f> verts;
    ///*back roof */
    //polygon
     verts.push_back(Vec3f (0.2, 0.575, 0.5));
     verts.push_back(Vec3f (0.2, 0.8, 0.25));
     verts.push_back(Vec3f (0.9, 0.575, 0.5));
     verts.push_back(Vec3f (0.9, 0.8, 0.25));


  glBindBuffer(GL_ARRAY_BUFFER, vertBufferbackroofID);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 4, // byte size of Vec3f, 4 of them
               verts.data(),      // pointer (Vec3f*) to contents of verts
               GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}


void loaddoorGeometryToGPU() {
  // Just basic layout of floats, for a quad
  // 3 floats per vertex, 4 vertices
  std::vector<Vec3f> verts;



    ///* door */
   //polygon
     verts.push_back(Vec3f (0.47, 0.1, 0.51));
      verts.push_back(Vec3f (0.47, 0.48, 0.51));
     verts.push_back(Vec3f (0.65, 0.1, 0.51));
     verts.push_back(Vec3f (0.65, 0.48, 0.51));




  glBindBuffer(GL_ARRAY_BUFFER, vertBufferdoorID);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 4, // byte size of Vec3f, 4 of them
               verts.data(),      // pointer (Vec3f*) to contents of verts
               GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}

void loadWinGeometryToGPU() {
  // Just basic layout of floats, for a quad
  // 3 floats per vertex, 4 vertices
  std::vector<Vec3f> verts;
 ///* window 1 */
    //polygon
     verts.push_back(Vec3f (0.72, 0.25, 0.51));
     verts.push_back(Vec3f (0.72, 0.4,  0.51));
     verts.push_back(Vec3f (0.83, 0.25, 0.51));


     verts.push_back(Vec3f (0.72, 0.4,  0.51));
     verts.push_back(Vec3f (0.83, 0.25, 0.51));
     verts.push_back(Vec3f (0.83, 0.4,  0.51));




    ///* window 2 */
    //polygon
     verts.push_back(Vec3f (0.27, 0.25,  0.51));
     verts.push_back(Vec3f (0.27, 0.4,  0.51));
     verts.push_back(Vec3f (0.38, 0.25,  0.51));



      verts.push_back(Vec3f (0.27, 0.4,  0.51));
     verts.push_back(Vec3f (0.38, 0.25,  0.51));
     verts.push_back(Vec3f (0.38, 0.4,  0.51));


  glBindBuffer(GL_ARRAY_BUFFER, vertBufferwinID);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(Vec3f) * 12, // byte size of Vec3f, 4 of them
               verts.data(),      // pointer (Vec3f*) to contents of verts
               GL_STATIC_DRAW);   // Usage pattern of GPU buffer
}

void setupVAO() {


glBindVertexArray(vaofbrecID);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferfbrecID);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );




                        glBindVertexArray(vaorlrecID);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferrlrecID);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );



                        glBindVertexArray(vaotringleID);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBuffertringleID);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                         );








        glBindVertexArray(vaoroofID);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferroofID);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                         );




   glBindVertexArray(vaobackroofID);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferbackroofID);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                         );

 glBindVertexArray(vaodoorID);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferdoorID);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );

  glBindVertexArray(vaowinID);

  glEnableVertexAttribArray(0); // match layout # in shader
  glBindBuffer(GL_ARRAY_BUFFER, vertBufferwinID);
  glVertexAttribPointer(0,        // attribute layout # above
                        3,        // # of components (ie XYZ )
                        GL_FLOAT, // type of components
                        GL_FALSE, // need to be normalized?
                        0,        // stride
                        (void *)0 // array buffer offset
                        );

  glBindVertexArray(0); // reset to default
}

void reloadProjectionMatrix() {
  // Perspective Only

  // field of view angle 60 degrees
  // window aspect ratio
  // near Z plane > 0
  // far Z plane

  P = PerspectiveProjection(WIN_FOV, // FOV
                            static_cast<float>(WIN_WIDTH) /
                                WIN_HEIGHT, // Aspect
                            WIN_NEAR,       // near plane
                            WIN_FAR);       // far plane depth
}

void loadModelViewMatrix() {

  Mrlrec = IdentityMatrix();
  Mtringle = IdentityMatrix();
  Mdoor = IdentityMatrix();
  Mwin = IdentityMatrix();
  Mbackroof = IdentityMatrix();
  Mroof = IdentityMatrix();
  Mfbrec = IdentityMatrix();
  // view doesn't change, but if it did you would use this
  V = camera.lookatMatrix();
}

void reloadViewMatrix() { V = camera.lookatMatrix(); }

void setupModelViewProjectionTransform() {
  MVP = P * V *( Mdoor+Mwin+Mbackroof+Mroof+Mtringle+Mrlrec+Mfbrec); // transforms vertices from right to left (odd huh?)
}

void reloadMVPUniform() {
  GLint id = glGetUniformLocation(basicProgramID, "MVP");

  glUseProgram(basicProgramID);
  glUniformMatrix4fv(id,        // ID
                     1,         // only 1 matrix
                     GL_TRUE,   // transpose matrix, Mat4f is row major
                     MVP.data() // pointer to data in Mat4f
                     );
}

void reloadColorUniform(float r, float g, float b) {
  GLint id = glGetUniformLocation(basicProgramID, "inputColor");

  glUseProgram(basicProgramID);
  glUniform3f(id, // ID in basic_vs.glsl
              r, g, b);
}

void generateIDs() {
  // shader ID from OpenGL
  std::string vsSource = loadShaderStringfromFile("./shaders/basic.vert");
  std::string fsSource = loadShaderStringfromFile("./shaders/basic.frag");
  basicProgramID = CreateShaderProgram(vsSource, fsSource);

  // VAO and buffer IDs given from OpenGL

  glGenVertexArrays(1, &vaofbrecID);
  glGenBuffers(1, &vertBufferfbrecID);

  glGenVertexArrays(1, &vaorlrecID);
  glGenBuffers(1, &vertBufferrlrecID);

  glGenVertexArrays(1, &vaotringleID);
  glGenBuffers(1, &vertBuffertringleID);

  glGenVertexArrays(1, &vaoroofID);
  glGenBuffers(1, &vertBufferroofID);

  glGenVertexArrays(1, &vaobackroofID);
  glGenBuffers(1, &vertBufferbackroofID);

  glGenVertexArrays(1, &vaodoorID);
  glGenBuffers(1, &vertBufferdoorID);

  glGenVertexArrays(1, &vaowinID);
  glGenBuffers(1, &vertBufferwinID);
}

void deleteIDs() {
  glDeleteProgram(basicProgramID);


  glDeleteVertexArrays(1, &vaofbrecID); // fb - rec
  glDeleteBuffers(1, &vertBufferfbrecID);

  glDeleteVertexArrays(1, &vaorlrecID); // rl - rec
  glDeleteBuffers(1, &vertBufferrlrecID);

  glDeleteVertexArrays(1, &vaotringleID); // tringle
  glDeleteBuffers(1, &vertBuffertringleID);

  glDeleteVertexArrays(1, &vaoroofID); // roof
  glDeleteBuffers(1, &vertBufferroofID);
  glDeleteVertexArrays(1, &vaobackroofID); // b roof
  glDeleteBuffers(1, &vertBufferbackroofID);

  glDeleteVertexArrays(1, &vaodoorID);  // door
  glDeleteBuffers(1, &vertBufferdoorID);

  glDeleteVertexArrays(1, &vaowinID);  // window
  glDeleteBuffers(1, &vertBufferwinID);
}

void init() {
  glEnable(GL_DEPTH_TEST);
  glPointSize(50);

  camera = Camera(Vec3f{0, 0, 20}, Vec3f{0, 0, -1}, Vec3f{0, 1, 0});

  // SETUP SHADERS, BUFFERS, VAOs

  generateIDs();
  setupVAO();

  loadrlrecGeometryToGPU();
  loadtringleGeometryToGPU();
  loadroofGeometryToGPU();
  loadbackroofGeometryToGPU();
  loaddoorGeometryToGPU();
  loadWinGeometryToGPU();
  loadfbrecGeometryToGPU();

  loadModelViewMatrix();
  reloadProjectionMatrix();
  setupModelViewProjectionTransform();
  reloadMVPUniform();
}


int main(int argc, char **argv) {
  GLFWwindow *window;

  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window =
      glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "CSE414 Project 1", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwSetWindowSizeCallback(window, windowSetSizeFunc);
  glfwSetFramebufferSizeCallback(window, windowSetFramebufferSizeFunc);
  glfwSetKeyCallback(window, windowKeyFunc);
  glfwSetCursorPosCallback(window, windowMouseMotionFunc);
  glfwSetMouseButtonCallback(window, windowMouseButtonFunc);

  glfwGetFramebufferSize(window, &WIN_WIDTH, &WIN_HEIGHT);

  // Initialize glad
  if (!gladLoadGL()) {
    std::cerr << "Failed to initialise GLAD" << std::endl;
    return -1;
  }

  std::cout << "GL Version: :" << glGetString(GL_VERSION) << std::endl;
  std::cout << GL_ERROR() << std::endl;

  // Initialize all the geometry, and load it once to the GPU
  init(); // our own initialize stuff func

  float t = 0;
  float dt = 0.01;

  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         !glfwWindowShouldClose(window)) {

    if (g_play) {
      t += dt;
      animateQuad(t);
    }

    displayFunc();
    moveCamera();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // clean up after loop
  deleteIDs();

  return 0;
}

//==================== CALLBACK FUNCTIONS ====================//

void windowSetSizeFunc(GLFWwindow *window, int width, int height) {
  WIN_WIDTH = width;
  WIN_HEIGHT = height;

  reloadProjectionMatrix();
  setupModelViewProjectionTransform();
  reloadMVPUniform();
}

void windowSetFramebufferSizeFunc(GLFWwindow *window, int width, int height) {
  FB_WIDTH = width;
  FB_HEIGHT = height;

  glViewport(0, 0, FB_WIDTH, FB_HEIGHT);
}

void windowMouseButtonFunc(GLFWwindow *window, int button, int action,
                           int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      g_cursorLocked = GL_TRUE;
    } else {
      g_cursorLocked = GL_FALSE;
    }
  }
}

void windowMouseMotionFunc(GLFWwindow *window, double x, double y) {
  if (g_cursorLocked) {
    float deltaX = (x - g_cursorX) * 0.01;
    float deltaY = (y - g_cursorY) * 0.01;
    camera.rotateAroundFocus(deltaX, deltaY);

    reloadViewMatrix();
    setupModelViewProjectionTransform();
    reloadMVPUniform();
  }

  g_cursorX = x;
  g_cursorY = y;
}

void windowKeyFunc(GLFWwindow *window, int key, int scancode, int action,
                   int mods) {
  bool set = action != GLFW_RELEASE && GLFW_REPEAT;
  switch (key) {
  case GLFW_KEY_ESCAPE:
    glfwSetWindowShouldClose(window, GL_TRUE);
    break;
  case GLFW_KEY_W:
    g_moveBackForward = set ? 1 : 0;
    break;
  case GLFW_KEY_S:
    g_moveBackForward = set ? -1 : 0;
    break;
  case GLFW_KEY_A:
    g_moveLeftRight = set ? 1 : 0;
    break;
  case GLFW_KEY_D:
    g_moveLeftRight = set ? -1 : 0;
    break;
  case GLFW_KEY_Q:
    g_moveUpDown = set ? -1 : 0;
    break;
  case GLFW_KEY_E:
    g_moveUpDown = set ? 1 : 0;
    break;
  case GLFW_KEY_UP:
    g_rotateUpDown = set ? -1 : 0;
    break;
  case GLFW_KEY_DOWN:
    g_rotateUpDown = set ? 1 : 0;
    break;
  case GLFW_KEY_LEFT:
    if (mods == GLFW_MOD_SHIFT)
      g_rotateRoll = set ? -1 : 0;
    else
      g_rotateLeftRight = set ? 1 : 0;
    break;
  case GLFW_KEY_RIGHT:
    if (mods == GLFW_MOD_SHIFT)
      g_rotateRoll = set ? 1 : 0;
    else
      g_rotateLeftRight = set ? -1 : 0;
    break;
  case GLFW_KEY_SPACE:
    g_play = set ? !g_play : g_play;
    break;
  case GLFW_KEY_LEFT_BRACKET:
    if (mods == GLFW_MOD_SHIFT) {
      g_rotationSpeed *= 0.5;
    } else {
      g_panningSpeed *= 0.5;
    }
    break;
  case GLFW_KEY_RIGHT_BRACKET:
    if (mods == GLFW_MOD_SHIFT) {
      g_rotationSpeed *= 1.5;
    } else {
      g_panningSpeed *= 1.5;
    }
    break;
  default:
    break;
  }
}

//==================== OPENGL HELPER FUNCTIONS ====================//

void moveCamera() {
  Vec3f dir;

  if (g_moveBackForward) {
    dir += Vec3f(0, 0, g_moveBackForward * g_panningSpeed);
  }
  if (g_moveLeftRight) {
    dir += Vec3f(g_moveLeftRight * g_panningSpeed, 0, 0);
  }
  if (g_moveUpDown) {
    dir += Vec3f(0, g_moveUpDown * g_panningSpeed, 0);
  }

  if (g_rotateUpDown) {
    camera.rotateUpDown(g_rotateUpDown * g_rotationSpeed);
  }
  if (g_rotateLeftRight) {
    camera.rotateLeftRight(g_rotateLeftRight * g_rotationSpeed);
  }
  if (g_rotateRoll) {
    camera.rotateRoll(g_rotateRoll * g_rotationSpeed);
  }

  if (g_moveUpDown || g_moveLeftRight || g_moveBackForward ||
      g_rotateLeftRight || g_rotateUpDown || g_rotateRoll) {
    camera.move(dir);
    reloadViewMatrix();
    setupModelViewProjectionTransform();
    reloadMVPUniform();
  }
}

std::string GL_ERROR() {
  GLenum code = glGetError();

  switch (code) {
  case GL_NO_ERROR:
    return "GL_NO_ERROR";
  case GL_INVALID_ENUM:
    return "GL_INVALID_ENUM";
  case GL_INVALID_VALUE:
    return "GL_INVALID_VALUE";
  case GL_INVALID_OPERATION:
    return "GL_INVALID_OPERATION";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "GL_INVALID_FRAMEBUFFER_OPERATION";
  case GL_OUT_OF_MEMORY:
    return "GL_OUT_OF_MEMORY";
  default:
    return "Non Valid Error Code";
  }
}
