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

#if defined(_WIN32)
#include <windows.h>
#else
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

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

  // Cross-platform shared memory for pixel buffer
  size_t pixel_buf_size = WIDTH * HEIGHT * 4;
  unsigned char *shm_pixels = nullptr;

#if defined(_WIN32)
  HANDLE hMapFile = CreateFileMappingA(
      INVALID_HANDLE_VALUE,    // Use paging file
      NULL,                    // Default security
      PAGE_READWRITE,          // Read/write access
      0,                       // Maximum object size (high-order DWORD)
      (DWORD)pixel_buf_size,   // Maximum object size (low-order DWORD)
      "Global\\cube_pixel_shm" // Name of mapping object
  );
  if (hMapFile == NULL)
  {
    std::cerr << "Could not create file mapping object (" << GetLastError() << ")\n";
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
  }
  shm_pixels = (unsigned char *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, pixel_buf_size);
  if (shm_pixels == NULL)
  {
    std::cerr << "Could not map view of file (" << GetLastError() << ")\n";
    CloseHandle(hMapFile);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 1;
  }
#else
  const char *shm_name = "/cube_pixel_shm";
  int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
  if (shm_fd < 0)
  {
    std::cerr << "Failed to open POSIX shared memory object" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }
  if (ftruncate(shm_fd, pixel_buf_size) < 0)
  {
    std::cerr << "Failed to set size of shared memory object" << std::endl;
    close(shm_fd);
    shm_unlink(shm_name);
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }
  shm_pixels = (unsigned char *)mmap(NULL, pixel_buf_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (shm_pixels == MAP_FAILED)
  {
    std::cerr << "Failed to mmap shared memory" << std::endl;
    close(shm_fd);
    shm_unlink(shm_name);
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }
#endif

  float angle = 0.0f;
  while (!glfwWindowShouldClose(window))
  {
    drawCube(angle);
    // Write framebuffer to shared memory for Electron
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, shm_pixels);
    // File-based export for comparison
    saveFramebufferToFile("/tmp/cube_framebuffer.bin");
    glfwSwapBuffers(window);
    glfwPollEvents();
    angle += 1.0f;
    if (angle > 360.0f)
      angle -= 360.0f;
  }

  // Cleanup shared memory
#if defined(_WIN32)
  UnmapViewOfFile(shm_pixels);
  CloseHandle(hMapFile);
#else
  munmap(shm_pixels, pixel_buf_size);
  close(shm_fd);
  shm_unlink(shm_name);
#endif
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
