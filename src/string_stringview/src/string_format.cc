#include "log.hh"

#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>


void
basic()
{
  WARNING << "format, 中的第一个参数是待格式化的字符串, 里面包含有占位符, "
             "后续参数是用于填充待格式化字符串中占位符的值";
  WARNING << "占位符一般都是 {}, 括号内的格式是[index][:spcifier]";
  WARNING << "只能对所有的 {} 添加 idx 或者不添加, 不能混用";
  INFO << std::format("{}, {}!", "hello", "world");
}


void
style()
{
  WARNING << "填充样式是有顺序的,[[fill]align][sign][#][0][width][.precision]["
             "type]";
  int num = 701;
  int width = 7;
  INFO << "width:5, " << std::format("|{:5}|", num);
  // 动态宽度
  INFO << "width:7, " << std::format("|{:{}}|", num, width);


  INFO << "align <:" << std::format("|{:<5}|", num);
  INFO << "align ^:" << std::format("|{:^5}|", num);
  INFO << "align >:" << std::format("|{:>5}|", num);


  INFO << "align <:" << std::format("|{:#<7}|", num);
  INFO << "align ^:" << std::format("|{:@^7}|", num);
  INFO << "align >:" << std::format("|{:!>7}|", num);

  int ng_num = -701;
  // + 对于正数用+号  对于负数用-号
  // - 对于负数用-号  正数不做处理
  //  (空格) 对于正数用 (空格), 负数用负号
  INFO << "sign +:" << std::format("|{:#<+7}|", num);
  INFO << "sign +:" << std::format("|{:#<+7}|", ng_num);
  INFO << "sign -:" << std::format("|{:#<-7}|", num);
  INFO << "sign -:" << std::format("|{:#<-7}|", ng_num);
  INFO << "sign  :" << std::format("|{:#< 7}|", num);
  INFO << "sign  :" << std::format("|{:#< 7}|", ng_num);

  int i = 42;
  INFO << "#: " << std::format("|{:10b}|", i);
  INFO << "#: " << std::format("|{:10B}|", i);
  INFO << "#: " << std::format("|{:#10b}|", i);
}

void
precision()
{
  WARNING << "this is precision";
  std::string str{"beacon"};
  INFO << std::format("{:.2}", str);
}

void
alternate()
{
  WARNING << "this is alternate formatting";
  WARNING << "# 会为输出的字符串添加备用格式, 如果是整形, 会添加所表示的进制, "
             "如果是浮点数, 会添加小数点";
  int i = 29;
  double d = 17;
  // |  035| 会增加对应的进制说明符
  INFO << "i{:#5o}: " << std::format("|{:#5o}|", i);
  // |   35|
  INFO << "i{:#5o}: " << std::format("|{:5o}|", i);
  // |       17.|, 就算没有小数, 也会添加一个小数点的
  INFO << "d{:#10}: " << std::format("|{:#10}|", d);
  // |        17|
  INFO << " d{:10}: " << std::format("|{:10}|", d);
}

void
type()
{
  WARNING << "this is type";
  WARNING << "type 指定了给定值要被格式化成的类型";
  // 整型:输出的进制, b(B), d(十进制), o, x(X), 如果type未指定,整型默认是d
  // 浮点型:e(E):科学计数法表示, f(F)固定表示法, g(G)以e(E)表示指数的通用方法, a(A)十六进制表示,如果没有指定, 则默认用g
  // 布尔型: s 以文本输出 bool, b B c d o x X 以整型输出0或1, 如果未指定,则用s
  // 字符串: s 输出字符串的副本, 未指定就是 s
  // 字符型: c 输出字符副本,b B d o x X 输出整型表示, 未指定就是c
  // 指针:   p (0x)为前缀的十六进制表示法, 未指定就是 p
  int a = 22;
  INFO << "a{:8b}" << std::format("|{:8b}|", a);
  INFO << "a{:8o}" << std::format("|{:8o}|", a);
  INFO << "a{:8x}" << std::format("|{:8x}|", a);

  double d = 17;
  INFO << "d:{:8e}" << std::format("|{:8e}|", d);
  INFO << "d:{:8f}" << std::format("|{:8f}|", d);
  INFO << "d:{:8g}" << std::format("|{:8g}|", d);

  bool b = true;
  INFO << "b:{:5s}" << std::format("|{:5s}|", b);
  INFO << "b:{:5b}" << std::format("|{:5b}|", b);

  int *p = new int(2);
  // INFO << "p:{10p}" << std::format("|{}|", p);
  INFO << "p:{:10p}" << std::format("|{:10p}|", static_cast<void *>(p));
  delete p;
}

class KeyValue
{
public:
  KeyValue(std::string_view key, int value)
    : key_(key)
    , value_(value)
  {}

  [[nodiscard]] std::string_view
  get_key() const
  {
    return key_;
  }

  [[nodiscard]] int
  get_value() const
  {
    return value_;
  }

private:
  std::string key_;
  int value_;
};

// 主要是重载两个函数 parse 和 formatter
//    其中 parse 用于解析格式说明符{:}，:后面的内容
//        formatter 用于根据格式说明符来进行格式化输出
template <>
class std::formatter<KeyValue>
{
public:
  // 解析格式说明符   format specifier
  // 在 std::format 的格式字符串中, ({:}, {:2}), {} 内部的 : 及其后面的内容是格式说明符
  //  parse 读取 格式说明符,提取相关信息, 并将其存储, 以供后续的 format函数使用
  ////////////////////////////////////////////////////////////////
  // parse 接收一个 format_parse_context, 通常简写成ctx
  // ctx.begin(), 指向当前格式说明符中的起始迭代器,通常是 : 后面的第一个字符.
  // ctx.end(), 指向当前格式说明符中的结束迭代器,通常是 } 字符
  // 格式字符串的范围是 [ctx.begin(), ctx.end()]
  // 返回一个迭代器，指向解析结束后的位置（通常是格式化字符串中未处理的部分）。
  constexpr auto
  parse(format_parse_context &context)
  {
    const auto *iter{context.begin()};
    const auto *const end{context.end()};

    if(iter == end || *iter == '}')
    {
      type_ = OutputType::KeyAndValue;
      return iter;
    }
    switch(*iter)
    {
      case 'a': // {:a}
        type_ = OutputType::KeyOnly;
        break;
      case 'v': // {:v}
        type_ = OutputType::ValueOnly;
        break;
      case 'c': // {:c}
        type_ = OutputType::KeyAndValue;
        break;
      default:
        throw format_error{"Invalid keyvalue format specifer."};
    }

    ++iter;
    if(iter != end && *iter != '}')
    {
      throw format_error{"Invalid keyvalue format specifer."};
    }
    return iter;
  }

  // 格式化 KeyValue 对象
  template <typename FormatContext>
  auto
  format(const KeyValue &kv, FormatContext &context) const
  {
    switch(type_)
    {
      using enum OutputType;
      case KeyOnly:
        return format_to(context.out(), "{}", kv.get_key());
      case ValueOnly:
        return format_to(context.out(), "{}", kv.get_value());
      default:
        return format_to(context.out(), "{}-{}", kv.get_key(), kv.get_value());
    }
  }

private:
  enum class OutputType
  {
    KeyOnly,
    ValueOnly,
    KeyAndValue,
  };

  OutputType type_{OutputType::KeyAndValue};
};

class Beacon
{
public:
  Beacon(std::string name, uint age, std::string dream, uint score)
    : name_(std::move(name))
    , age_(age)
    , dream_(std::move(dream))
    , score_(score)
  {}

  [[nodiscard]] std::string
  get_name() const
  {
    return name_;
  }

  [[nodiscard]] uint
  get_age() const
  {
    return age_;
  }

  [[nodiscard]] std::string
  get_dream() const
  {
    return dream_;
  }

  [[nodiscard]] uint
  get_score() const
  {
    return score_;
  }

private:
  std::string name_{"beacon"};
  uint age_{17};
  std::string dream_{"program"};
  uint score_{71};
};

template <>
class std::formatter<Beacon>
{
public:
  constexpr auto
  parse(format_parse_context &ctx)
  {
    // ctx 是 {:} 中冒号后面的内容, 不包括冒号
    const auto *itr{ctx.begin()};
    const auto *const end{ctx.end()};
    if(itr == end || *itr == '}')
    {
      INFO << "{:} is empty";
      format_ |= 0b1111;
      return itr;
    }

    // 这里的问题是,不能使用 itr != end 来进行判断
    // 使用 c++23 还是不能编译过
    // while(itr != end)
    while(*itr != '}')
    {
      switch(*itr)
      {
        case 'n':
          format_ |= 0b1;
          break;
        case 'a':
          format_ |= 0b10;
          break;
        case 'd':
          format_ |= 0b100;
          break;
        case 's':
          format_ |= 0b1000;
          break;
        default:
          // throw invalid_argument("invalid specile format!");
          throw format_error{"Invalid beacon format specifer."};
          break;
      }
      ++itr;
    }


    // for(; itr != end && *itr != '}'; ++itr)
    // {
    //   switch(*itr)
    //   {
    //     case 'n':
    //       format_ |= 0b1;
    //       break;
    //     case 'a':
    //       format_ |= 0b10;
    //       break;
    //     case 'd':
    //       format_ |= 0b100;
    //       break;
    //     case 's':
    //       format_ |= 0b1000;
    //       break;
    //     default:
    //       // throw invalid_argument("invalid specile format!");
    //       throw format_error{"Invalid beacon format specifer."};
    //       break;
    //   }
    // }

    if(itr != end && *itr != '}')
    {
      throw format_error{"Invalid beacon format specifer."};
    }
    return itr;
  }


  template <typename FormatContext>
  auto
  format(const Beacon &bon, FormatContext &ctx) const
  {
    std::stringstream ss;
    if(format_ & 0b1)
    {
      ss << "name:" << bon.get_name();
    }
    if(format_ & 0b10)
    {
      ss << ", age:" << bon.get_age();
    }
    if(format_ & 0b100)
    {
      ss << ", dream:" << bon.get_dream();
    }
    if(format_ & 0b1000)
    {
      ss << ", score:" << bon.get_score();
    }

    return format_to(ctx.out(), "{}", ss.str());
  }

private:
  uint8_t format_{0b0};
};

int
main(int /*argc*/, char **argv)
{
  logger::Logger::Init(argv[0]);

  basic();
  style();
  precision();
  alternate();
  type();

  KeyValue kv{"beacon", 17};
  // INFO << "kv:{:a}" << std::format("{:a}", kv);

  // std::format 返回的是一个 string
  // format_to(), 直接将格式化的结果写入到提供的迭代器中，效率更高，
  // template <typename OutputIt, typename... Args>
  // OutputIt format_to(OutputIt out, std::string_view fmt, const Args&... args);
  // out: 输出迭代器，表示要将格式化后的字符串写入的位置
  // fmt: 待格式化的字符串
  // args: 要格式化的参数,用于替换格式化字符串中的占位符
  // OutputIt: 将字符串写入后迭代器的位置
  // OutputIt: 返回更新后的迭代器, 指向写入完成后的位置
  //! 其实这里自己的问题是不清楚写入迭代器的是什么,以及其使用...
  // todo 但是这里不做展开了, 后面再继续展开
  std::cout << std::format("{}", kv) << "\n"; // 应输出 "test-42"


  Beacon bon{"beacon", 17, "qing", 99};
  std::cout << std::format("{:nads}\n", bon);
  return 0;
}
