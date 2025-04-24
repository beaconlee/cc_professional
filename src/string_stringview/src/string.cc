#include "log.hh"

#include <string>
#include <cxxabi.h>

// 可以使用 s 表示是一个字符串
using namespace std::literals::string_literals;

int
main(int /*argc*/, char **argv)
{
  logger::Logger::Init(argv[0]);

  // string 是 模板类 std::basic_string 的一个实例类
  // 提供了自动内存管理

  std::string str1{"beacon"};
  std::string str2{"yydylg"};
  std::string str3{"yydylg"};

  // string 重载了一些运算符: == = !=  <=  >= += +
  if(str2 == str3)
  {
    INFO << "str2(yydylg) == str3(yydylg)";
  }
  else
  {
    INFO << "str2(yydylg) != str3(yydylg)";
  }

  // std::string 提供的方法
  // 1. data(), 在 c++17 以前,data始终返回 const char *, 在 c++17 以后, 如果 string 非 const, 则返回的是 char*

  // 2. substr(pos, len)

  // 3. replace(pos, len, str)

  // 4. start_with/end_with(str)

  // 5. find(str) 查找子串

  // 6. compare(other_str)


  // !注意下面的区别, 虽然都是字面量, 但是存储的地方不同.
  // - "beacon" 字面量存储在只读内存区域。
  // - literial 是一个指针，存储在栈上，指向只读内存区域的 "beacon"。
  const char *literial{"beacon"}; // 类型是 const char[N]
  // - "beacon" 字面量存储在只读内存区域。
  // - str 是 std::string 对象，通常存储在栈上。
  // - str 内部在堆上分配内存，复制 "beacon" 的内容到堆上。
  // 创建 str 时, std::string 的构造函数会被调用, 传入一个指向 "beacon" 的指针
  // str 会在堆上开辟一块内存,
  std::string str{"beacon"};
  // 不是所有自定义字符串都先存储在静态存储区,只有涉及字符串字面量初始化时才涉及静态存储区的字面量存储和运行时拷贝
  // 对于其它的情况(用户输入(stdin),动态拼接,拷贝构造等),字符串内容直接在堆上(或者SSO缓冲区)管理
  // c++ 标准库中, std::string 通常会使用 Small String Optimization, SSO,
  // 对于短字符(通常是 <= 15-20, 取决于具体实现), 不会在堆上开辟内存,会直接存放在 std::string 对象的内部缓冲区(栈上)


  //////////////////////////////////////////////////////////////////////////////////////////////////
  // 源代码中,字符串字面量通常解释为 cosnt char *
  // 使用 标准用户定义的字面量 s, 可以把字符串字面量解释为 std::string
  auto str4{"beacon"};  // 解释为 const char *
  auto str5{"beacon"s}; // 这样创建的 str5 就是一个 string
  int status = -1;
  std::unique_ptr<char, void (*)(void *)> res{
      abi::__cxa_demangle(typeid(str4).name(), nullptr, nullptr, &status),
      std::free};
  const auto *str4_type = (status == 0) ? res.get() : typeid(str4).name();
  INFO << "str4 type:" << str4_type;

  status = -1;
  std::unique_ptr<char, void (*)(void *)> res2{
      abi::__cxa_demangle(typeid(str5).name(), nullptr, nullptr, &status),
      std::free};
  const auto *str5_type = (status == 0) ? res2.get() : typeid(str5).name();
  INFO << "str5 type:" << str5_type;


  // s 的使用方式
  // 这里的 vector 被推断成的是 const char *
  std::vector vec{"beacon", "yydylg", "uan"};
  status = -1;
  std::unique_ptr<char, void (*)(void *)> res3{
      abi::__cxa_demangle(typeid(vec).name(), nullptr, nullptr, &status),
      std::free};
  const auto *vec_type = (status == 0) ? res3.get() : typeid(vec).name();
  INFO << "vec_type:" << vec_type;
  // 这里的 vector 被推断成 std::string
  std::vector vec2{"beacon"s, "yydygl"s, "uan"s};

  status = -1;
  std::unique_ptr<char, void (*)(void *)> res4{
      abi::__cxa_demangle(typeid(vec2).name(), nullptr, nullptr, &status),
      std::free};
  const auto *vec2_type = (status == 0) ? res4.get() : typeid(vec2).name();
  INFO << "vec2_type:" << vec2_type;


  return 0;
}
