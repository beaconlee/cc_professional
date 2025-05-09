#include <functional>
#include <any>
#include <format>
#include <iostream>
#include <typeindex>

class Fruit
{
public:
  [[nodiscard]] virtual std::string_view
  name() const = 0;
};


class Apple : public Fruit
{
public:
  virtual ~Apple() = default;
  [[nodiscard]] std::string_view
  name() const override
  {
    return name_;
  }

private:
  std::string name_{"apple"};
};

class Banana : public Fruit
{
public:
  virtual ~Banana() = default;

  [[nodiscard]] std::string_view
  name() const override
  {
    return name_;
  }

private:
  std::string name_{"banana"};
};


template <>
class std::formatter<Fruit>
{
public:
  auto constexpr parse(std::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto
  format(const Fruit &fruit, FormatContext &ctx) const
  {
    return format_to(ctx.out(), "{}", fruit.name());
  }
};

template <>
class std::formatter<Apple>
{
public:
  auto constexpr parse(std::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto
  format(const Apple &fruit, FormatContext &ctx) const
  {
    return format_to(ctx.out(), "{}", fruit.name());
  }
};

template <>
class std::formatter<Banana>
{
public:
  auto constexpr parse(std::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto
  format(const Banana &fruit, FormatContext &ctx) const
  {
    return format_to(ctx.out(), "{}", fruit.name());
  }
};

using any_handler = std::function<void(const std::any &any)>;

std::unordered_map<std::type_index, any_handler> handlers{{
    {std::type_index(typeid(int)),
     [](const std::any &any)
     { std::cout << std::format("{}\n", std::any_cast<int>(any)); }},
    {std::type_index(typeid(Apple)),
     [](const std::any &any)
     { std::cout << std::format("{}\n", std::any_cast<Apple>(any)); }},
    {std::type_index(typeid(Banana)),
     [](const std::any &any)
     { std::cout << std::format("{}\n", std::any_cast<Banana>(any)); }},
}};


// type_index 是 type_info 的一个包装器
// 那么 type_info 是什么？ 由什么获得呢？
void
process_any(const std::any &any)
{
  auto it = handlers.find(std::type_index(any.type()));
  if(it == handlers.end())
  {
    return;
  }
  it->second(any);
}


int
main()
{
  Apple ap;
  std::cout << std::format("{}\n", ap);

  std::any any_apple(ap);
  std::cout << std::format("any apple has value: {}, value type: {}\n",
                           any_apple.has_value(),
                           any_apple.has_value() ? any_apple.type().name()
                                                 : "nullptr");
  process_any(any_apple);


  any_apple = Banana();
  process_any(any_apple);

  return 0;
}