#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <cstring>
#include <fstream>

const int WIDTH = 512;
const int HEIGHT = 512;

void saveFramebufferToFile(const char *filename)
{
  unsigned char *pixels = new unsigned char[WIDTH * HEIGHT * 4];
  glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  std::ofstream file(filename, std::ios::binary);
  file.write((char *)pixels, WIDTH * HEIGHT * 4);
  file.close();
  delete[] pixels;
}

void drawCube(float angle)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -5.0f);
  glRotatef(angle, 1.0f, 1.0f, 0.0f);
  glColor3f(1.0f, 0.0f, 0.0f); // Red
  // Draw cube manually
  glBegin(GL_QUADS);
  // Front face
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glVertex3f(1.0f, -1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  // Back face
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glVertex3f(1.0f, -1.0f, -1.0f);
  // Top face
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  // Bottom face
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(1.0f, -1.0f, -1.0f);
  glVertex3f(1.0f, -1.0f, 1.0f);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  // Right face
  glVertex3f(1.0f, -1.0f, -1.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(1.0f, -1.0f, 1.0f);
  // Left face
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glEnd();
}

int main()
{
  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Spinning Red Cube", nullptr, nullptr);
  if (!window)
  {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glewInit();
  glViewport(0, 0, WIDTH, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Simple perspective projection
  float fovY = 45.0f;
  float aspect = (float)WIDTH / (float)HEIGHT;
  float zNear = 0.1f;
  float zFar = 100.0f;
  float fH = tan(fovY / 360.0f * M_PI) * zNear;
  float fW = fH * aspect;
  glFrustum(-fW, fW, -fH, fH, zNear, zFar);
  glMatrixMode(GL_MODELVIEW);
  float angle = 0.0f;
  while (!glfwWindowShouldClose(window))
  {
    drawCube(angle);
    saveFramebufferToFile("/tmp/cube_framebuffer.bin"); // Example: share via file
    glfwSwapBuffers(window);
    glfwPollEvents();
    angle += 1.0f;
    if (angle > 360.0f)
      angle -= 360.0f;
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
