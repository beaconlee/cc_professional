#include "log.hh"
#include <string_view>

#include <cxxabi.h>

using namespace std::string_view_literals;

// 如果函数需要一个只读字符串作为参数时,那么就可以用 std::string_view 代替 std::string 和 const char *
// 这里传入的只有两个:指针和大小, 所以这里不需要用引用, 直接拷贝就可以了
std::string_view
extract_extension(std::string_view file_name)
{
  return file_name.substr(file_name.find_last_of('.') + 1);
}

void
not_implement_string_view_2_string()
{
  // 无法隐式将一个 std::string_view 转换为 std::string
  // 要构建一个 std::string 要么显示的使用 std::string 的构造函数, 要么使用 std::string_view 的 data 成员
  // 同样的原因,也无法连接 string 和 string_view
  // 但是可以使用 data 成员
  std::string_view sv{"beacon"};
  std::string str{"beacon"};
  // Invalid operands to binary expression ('std::string' (aka 'basic_string<char>') and 'std::string_view' (aka 'basic_string_view<char>'))
  // auto result = str + sv;
  auto result = str + sv.data();
  auto resutl2 = str.append(sv.data(), sv.length());
}

void
safety()
{
  // 函数应该返回 std::string 或 const std::string &
  // 返回 std::string_view 会面临一系列的不安全行为
  // 必须确保 std::string_view 包含的指针在其生命周期内有效

  // ! 基于此, string_view 也不应该用来存储一个临时字符串
  std::string str1{"hello"};
  // 这是一个未定义的行为, sv1 中的指针指向的是一个临时字符串的地址, 在这个赋值语句过后,这个字符串就会被销毁
  // ! 这里真正的问题在于自己没有意识到 str1+"world"是一个临时字符串, 这和 c++ 中的右值有关么?
  std::string_view sv1{str1 + " world"};
  ERROR << "this is error: " << sv1;
}

void
string_view_literals()
{
  // 使用标准的用户定义的字面量 sv 将一个字符串字面量表示为 string_view_literals
  auto sv{"beacon"sv};
  int status = -1;
  std::unique_ptr<char, void (*)(void *)> res{
      abi::__cxa_demangle(typeid(sv).name(), nullptr, nullptr, &status),
      std::free};
  INFO << "sv type: " << ((status == 0) ? res.get() : typeid(sv).name());
}

int
main(int /*argc*/, char **argv)
{
  logger::Logger::Init(argv[0]);

  // std::string_view 是 std::basic_string_view<> 模板类的一个实例
  // using string_view = basic_string_view<char>;
  // 内部只有两个成员:
  //                 size_t	    _M_len;
  //                 const _CharT* _M_str;


  // std::string_view 的构造函数有:
  //    1. string literial
  INFO << "construct: 字符串字面量: "
       << extract_extension("beacon/workspace/cc_professional/build/src/"
                            "string_stringview/c_str.INFO");
  //    2. string
  INFO << "construct: std::string: "
       << extract_extension(
              std::string("beacon/workspace/cc_professional/build/src/"
                          "string_stringview/c_str.INFO"));
  //    3. raw string literial
  // err! forget raw string syntax
  INFO << "construct: raw string literial: "
       << extract_extension(R"(beacon/workspace/cc_professional/build/src/"
                          "string_stringview/c_str.INFO)");

  const char *str = "beacon.wudi";
  size_t len = strlen(str) + 1;
  INFO << "construct: pointer and length: " << extract_extension({str, len});
  INFO << "construct: pointer and length: "
       << extract_extension(std::string_view{str, len});

  safety();

  string_view_literals();

  return 0;
}