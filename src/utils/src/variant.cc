#include "log.hh"

#include <variant>
#include <format>

// c++ 中的 variant 的作用是用来存储可能是多个类型的值的对象, 类似于 protobuf 中的 oneof
// 在创建 variant 时, 不能传入重复的类型, 并且第一个类型应该有默认的构造函数, 或者使用monostate来替代
// 一次只能存储一个类型的值, 当存入新的值时, 会自动擦除前面的值
// 可以通过 index 或者 type 两种方式来获取 variant 中存储的值
// 可以使用 visit 函数来进行输出

// 获取当前 variant 对象存储的类型(或者 index):
// index() 获取当前的 index, holds_alternative<type>(xx) 查看当前是否有包含有某个特定类型的值

// 直接获取某个类型的值时, 如果不存在会抛出异常, 应该判断当前是否存储了某个类型:
// 传入一个 var 的指针, 返回的是一个查询类型的指针, 如果没有当前类型的就返回一个 nullpt,
// auto result = std::get_if<type>(&var);

// 结合 visit 函数, 与可调用对象, 会自动根据 variant 的类型来调用对应的重载函数.
// 需要在可调用对象中, 重载所有的类型

// using Literal = std::variant<std::string, double, bool, std::nullptr_t>;
using Literal =
    std::variant<std::monostate, std::string, double, bool, std::nullptr_t>;

class LiteralVisit
{
public:
  void
  operator()(double /*d*/)
  {
    ERROR << "visit is double()";
  }

  void
  operator()(const std::string & /**/)
  {
    ERROR << "visit is std::string";
  }

  void
  operator()(bool & /**/)
  {
    ERROR << "visit is bool";
  }

  void
  operator()(std::nullptr_t)
  {
    ERROR << "visit is std::nullptr_t";
  }

  void
  operator()(std::monostate /*unused*/)
  {
    ERROR << "visit is std::monostate";
  }
};

// 不能在 variant 中存储数组(就像 optional 一样), 引用, 可以存储指针, reference_wrapper<const T> 或 reference_wrapper<T> 的实例



int
main(int /*argc*/, char **argv)
{
  logger::Logger::Init(argv[0]);
  Literal var1;
  std::visit(LiteralVisit{}, var1);

  WARNING << "---------string---------";
  var1 = "beacon";
  INFO << "using get<1>: " << std::get<1>(var1);
  INFO << "using get<std::string>: " << std::get<std::string>(var1);
  INFO << "cur variant index:" << var1.index();
  INFO << "cur type is std::string:"
       << std::holds_alternative<std::string>(var1);
  INFO << "cur type is double:" << std::holds_alternative<double>(var1);
  std::visit(LiteralVisit{}, var1);

  auto *has_string = std::get_if<std::string>(&var1);
  if(has_string != nullptr)
  {
    INFO << "cur type is string";
  }
  auto *has_double = std::get_if<double>(&var1);
  if(has_double != nullptr)
  {
    INFO << "cur type is double";
  }

  WARNING << "---------double---------";
  var1 = 17.0701;
  INFO << "using get<2>: " << std::get<2>(var1);
  INFO << "using get<double>: " << std::get<double>(var1);
  INFO << "cur variant index:" << var1.index();
  INFO << "cur type is std::string:"
       << std::holds_alternative<std::string>(var1);
  INFO << "cur type is double:" << std::holds_alternative<double>(var1);

  has_string = std::get_if<std::string>(&var1);
  if(has_string != nullptr)
  {
    INFO << "cur type is string";
  }
  has_double = std::get_if<double>(&var1);
  if(has_double != nullptr)
  {
    INFO << "cur type is double";
  }
  std::visit(LiteralVisit{}, var1);

  return 0;
}