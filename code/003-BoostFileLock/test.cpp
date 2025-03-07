#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>

using namespace boost::interprocess;

std::string fileName("test.txt");

int main() {
  FILE *file = nullptr;
  char aa[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";

  if ((file = fopen(fileName.c_str(), "ab+")) == nullptr) {
    std::cout << "open file falure" << std::endl;
  }
  char bb[500];
  memset(bb, 0, 500);
  file_lock f_lock(fileName.c_str());
  {
    sharable_lock<file_lock> sh_lock(f_lock);
    fseek(file, 0, SEEK_SET);
    for (int i = 0; i < 1000; i++) {
      int intb = fread(bb, 500, 1, file);
      if (intb == 0) {
        std::cout << "Read: " << bb << std::endl;
      }
    }
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
  }

  {
    scoped_lock<file_lock> sh_lock(f_lock);
    fseek(file, 0, SEEK_END);
    for (int i = 0; i < 1000; i++) {
      int inta = fwrite(aa, strlen(aa), 1, file);
      if (inta == 1) {
        std::cout << "Write: " << i << std::endl;
      }
    }
    boost::thread::sleep(boost::get_system_time() + boost::posix_time::seconds(5));
  }
  fflush(file);

  fclose(file);
  return (0);
}
