#include "person_generated.h"
#include <flatbuffers/flatbuffers.h>
#include <vector>
#include <fstream>

int main() {
  flatbuffers::FlatBufferBuilder builder(1024);

  // 创建嵌套对象
  auto friend_name = builder.CreateString("Alice");
  auto friend_age = 30;
  auto friend_offset = CreatePerson(builder, friend_name, friend_age, 0);

  // 创建主对象
  auto name = builder.CreateString("Bob");
  auto age = 40;

  // 使用 CreateVector 方法创建向量
  std::vector<flatbuffers::Offset<Person>> friends = {friend_offset};
  auto friends_vec = builder.CreateVector(friends);

  auto root = CreatePerson(builder, name, age, friends_vec);

  // 构建最终缓冲区
  builder.Finish(root);
  auto buf = builder.GetBufferPointer();
  size_t size = builder.GetSize();

  // 保存到文件（可选）
  std::ofstream file("data.bin", std::ios::binary);
  file.write(reinterpret_cast<const char*>(buf), size);
  file.close(); 

  return 0;
}