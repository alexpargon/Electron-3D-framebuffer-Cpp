#pragma once
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <string>

class FramebufferMMap
{
public:
  FramebufferMMap(const std::string &name, size_t size)
      : shm_name(name), shm_size(size), shm_fd(-1), shm_ptr(nullptr) {}

  bool open()
  {
    shm_fd = ::shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
      return false;
    if (ftruncate(shm_fd, shm_size) == -1)
      return false;
    shm_ptr = ::mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    return shm_ptr != MAP_FAILED;
  }

  void *data() { return shm_ptr; }

  void close()
  {
    if (shm_ptr && shm_ptr != MAP_FAILED)
    {
      ::munmap(shm_ptr, shm_size);
      shm_ptr = nullptr;
    }
    if (shm_fd != -1)
    {
      ::close(shm_fd);
      shm_fd = -1;
    }
    ::shm_unlink(shm_name.c_str());
  }

  ~FramebufferMMap() { close(); }

private:
  std::string shm_name;
  size_t shm_size;
  int shm_fd;
  void *shm_ptr;
};
