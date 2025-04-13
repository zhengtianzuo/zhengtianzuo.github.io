#include "person_generated.h"
#include <fstream>
#include <cstdio>

int main() {
  // 读取二进制文件
  std::ifstream file("data.bin", std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    printf("Failed to open file.\n");
    return -1;
  }

  size_t size = file.tellg();
  char* buf = new char[size];
  file.seekg(0, std::ios::beg);
  file.read(buf, size);
  file.close();

  // 验证缓冲区
  flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t*>(buf), size);
  if (!verifier.VerifyBuffer<Person>()) {
    delete[] buf;
    printf("Verification failed.\n");
    return -1;
  }

  // 直接访问数据
  const Person* person = GetPerson(buf);
  printf("Name: %s, Age: %d\n", person->name()->c_str(), person->age());

  // 遍历嵌套数据
  if (person->friends() != nullptr) {
    for (int i = 0; i < person->friends()->size(); ++i) {
      const Person* friend_person = person->friends()->Get(i);
      printf("Friend: %s\n", friend_person->name()->c_str());
    }
  }

  delete[] buf;
  return 0;
}