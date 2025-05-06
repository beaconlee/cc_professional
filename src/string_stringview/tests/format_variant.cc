#include "log.hh"

#include <format>
#include <iostream>
#include <utility>
#include <variant>

class Beacon
{
public:
  Beacon(std::string name, int age)
    : name_(std::move(name))
    , age_(age)
  {}

  [[nodiscard]] std::string_view
  get_name() const
  {
    return name_;
  }

  [[nodiscard]] unsigned int
  get_age() const
  {
    return age_;
  }

private:
  std::string name_;
  unsigned int age_;
};


template <>
class std::formatter<Beacon>
{
public:
  constexpr auto
  parse(format_parse_context &cx)
  {
    const auto *beg = cx.begin();
    // std::cout << "*beg:" << *beg << "\n";
    return cx.begin();
  }

  template <typename FormatContext>
  auto
  format(const Beacon &bea, FormatContext &ct) const
  {
    return format_to(
        ct.out(),
        // std::format("name: {},age: {}", bea.get_name(), bea.get_age()));
        "name: {},age: {}",
        bea.get_name(),
        bea.get_age());
  }
};

using Literal = std::variant<std::monostate, std::string, double, Beacon>;

template <>
class std::formatter<Literal>
{
public:
  constexpr auto
  parse(format_parse_context &ct)
  {
    return ct.begin();
  }

  //todo 关于这里为什么要加 constexpr 以及使用visit,需要后面在做深入的分析
  template <typename FormatContext>
  auto
  format(const Literal &literal, FormatContext &ct) const
  {
    // 这样写会直接产生 coredump
    // return format_to(ct.out(), "{}", literal);
    // ! 自己的问题 1. 没有返回 visit 的函数值, 只是在 lambda 中进行了返回
    //            2. 直接使用了 visit 没有判断类型. 也没有加 constexpr

    return std::visit(
        [&ct](const auto &value)
        {
          if constexpr(std::is_same_v<std::decay_t<decltype(value)>,
                                      std::monostate>)
          {
            return format_to(ct.out(), "null");
          }
          else
          {
            return format_to(ct.out(), "{}", value);
          }
        },
        literal);
  }
};

int
main(int /*argc*/, char **argv)
{
  logger::Logger::Init(argv[0]);
  Literal l1;
  std::cout << std::format("bea: {}\n", l1);

  Beacon bea("beacon", 17);

  l1 = bea;

  std::cout << std::format("bea: {}\n", bea);
  std::cout << std::format("bea: {}\n", l1);

  return 0;
}
