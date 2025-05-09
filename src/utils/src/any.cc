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

struct ValueVisitor
{
  virtual void
  visit(int value) = 0;
  virtual void
  visit(const std::string &value) = 0;
  virtual void
  visit(const Beacon &value) = 0;
};

struct PrintVisitor : ValueVisitor
{
  void
  visit(int value) override
  {
    std::cout << std::format("Int: {}\n", value);
  }
  void
  visit(const std::string &value) override
  {
    std::cout << std::format("String: {}\n", value);
  }
  void
  visit(const Beacon &value) override
  {
    std::cout << "Beacon object\n";
  }
};

void
process_any(const std::any &a, ValueVisitor &visitor)
{
  if(a.type() == typeid(int))
  {
    visitor.visit(std::any_cast<int>(a));
  }
  else if(a.type() == typeid(std::string))
  {
    visitor.visit(std::any_cast<std::string>(a));
  }
  else if(a.type() == typeid(Beacon))
  {
    visitor.visit(std::any_cast<Beacon>(a));
  }
  else
  {
    std::cout << std::format("Unknown type: {}\n", a.type().name());
  }
}

// 如果类型集合事先已知, 那么可以使用 variant 进行替代
using VarType = std::variant<int, std::string, Beacon>;

void
print_variant(const VarType &v)
{
  std::visit(
      [](const auto &value)
      {
        using T = std::decay_t<decltype(value)>;
        if constexpr(std::is_same_v<T, int>)
        {
          std::cout << std::format("Int: {}\n", value);
        }
        else if constexpr(std::is_same_v<T, std::string>)
        {
          std::cout << std::format("String: {}\n", value);
        }
        else if constexpr(std::is_same_v<T, Beacon>)
        {
          std::cout << "Beacon object\n";
        }
      },
      v);
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


  std::cout << "\n\n-------------------\n";
  PrintVisitor visitor;
  std::any a2 = 42;
  process_any(a2, visitor);
  a2 = "beacon"s;
  process_any(a2, visitor);
  a2 = Beacon();
  process_any(a2, visitor);

  std::cout << "\n\n-------------------\n";

  VarType v = 42;
  print_variant(v);
  v = "beacon"s;
  print_variant(v);
  v = Beacon();
  print_variant(v);
  return 0;
}

/*
如果你的需求是真正的动态类型处理（例如支持运行时添加新类型），std::any 可能不是最佳选择。你可以考虑以下方法：

自定义类型系统：实现一个自定义的类型注册机制，将类型与处理函数关联起来，类似于动态语言（如 Python）的反射机制。
第三方库：使用支持运行时反射的库（如 Boost.TypeErasure 或其他反射库）。
脚本语言嵌入：将动态类型处理的任务交给嵌入的脚本语言（如 Lua 或 Python）。
*/