#include <any>
#include <format>
#include <iostream>
#include <typeindex>
#include <unordered_map>
#include <functional>

using namespace std::string_literals;


class Beacon
{};

using Handler = std::function<void(const std::any &)>;

const std::unordered_map<std::type_index, Handler> handlers = {
    {std::type_index(typeid(int)),
     [](const std::any &a)
     { std::cout << std::format("Value: {}\n", std::any_cast<int>(a)); }},
    {std::type_index(typeid(std::string)),
     [](const std::any &a) {
       std::cout << std::format("Value: {}\n", std::any_cast<std::string>(a));
     }},
    {std::type_index(typeid(Beacon)),
     [](const std::any &a) { std::cout << "Beacon object\n"; }}};

void
process_any(const std::any &a)
{
  auto it = handlers.find(std::type_index(a.type()));
  if(it != handlers.end())
  {
    it->second(a);
  }
  else
  {
    std::cout << std::format("Unknown type: {}\n", a.type().name());
  }
}

int
main()
{
  // std::any::type() 返回的是 std::type_info 是一个运行时类型信息(RTTI)对象
  // std::any_cast<T> 是需要在编译器就获取类型信息的, c++ 是一门静态类型语言

  std::any int_any{2};
  std::cout << std::format("int_any has value:{}\n", int_any.has_value());
  std::cout << std::format("int_any value name:{}\n", int_any.type().name());
  std::cout << std::format("int_any value:{}\n", std::any_cast<int>(int_any));

  // 如果这里没有 s 的话, 那么会将其当作 const char * 来进行处理, 表示一个字符串常量
  int_any = "beacon"s;
  std::cout << std::format("int_any has value:{}\n", int_any.has_value());
  std::cout << std::format("int_any value name:{}\n", int_any.type().name());
  std::cout << std::format("int_any value:{}\n",
                           std::any_cast<std::string>(int_any));

  int_any = Beacon();
  std::cout << std::format("int_any has value:{}\n", int_any.has_value());
  std::cout << std::format("int_any value name:{}\n", int_any.type().name());
  // std::cout << std::format("int_any value:{}\n",
  //                          std::any_cast<std::string>(int_any));
  // 与 optional 和 variant 一样, 无法存储 any 的引用, 但是可以存储 any 的指针, reference_wrapper<const T> 或 reference_wrapper<T> 的实例.


  std::cout << "-------------------\n";
  std::any a = 42;
  process_any(a);
  a = "beacon"s;
  process_any(a);
  a = Beacon();
  process_any(a);
  a = 3.14; // 未知类型
  process_any(a);
  return 0;
}