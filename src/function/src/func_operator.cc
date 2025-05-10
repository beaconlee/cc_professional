#include <cmath>
#include <iostream>
#include <format>
#include <functional>
#include <queue>
#include <set>
#include <sstream>

using namespace std::string_view_literals;

// !函数调用运算符
// operator() 这个是函数调用运算符，虽然自己之前已经多次重载过了，并且使用了，但直到今天，自己才真的理解它的是什么。。。

// 重载了函数调用运算符的类称为函数对象，或者仿函数


template <typename MatchFunc, typename HandlerFunc>
void
process_matches(std::span<const int> s1,
                std::span<const int> s2,
                const MatchFunc &match,
                const HandlerFunc &handler)
{
  if(s1.size() != s2.size())
  {
    throw std::invalid_argument("s1 and s2 must have equal size");
    return;
  }

  for(uint i = 0; i < s1.size(); ++i)
  {
    if(match(s1[i], s2[i]))
    {
      handler(i, s1[i], s2[i]);
    }
  }
}

bool
match(int a, int b)
{
  return a == b;
}

void
print(uint i, int a, int b)
{
  std::cout << std::format("find matched at position:{}, ({},{})\n", i, a, b);
}

class IsLargeThan
{
public:
  explicit IsLargeThan(int val)
    : val_(val)
  {}
  bool
  operator()(int a, int b) const
  {
    return a >= val_ && b >= val_;
  }

private:
  int val_{0};
};

class Beacon
{
public:
  // !自己还没有理解到c++中重载运算符的意义是什么,所以这里重载+时,自己才不知道要返回什么
  // 在重载 + 运算符时，返回类型通常需要符合加法运算的语义，即“将两个对象相加后得到一个新的结果”。
  // void
  // operator+(const Beacon &oth)
  // {
  //   score_ += oth.score_;
  // }
  Beacon &
  operator+=(const Beacon &oth)
  {
    score_ += oth.score_;
    return *this;
  }

  Beacon
  operator+(const Beacon &oth) const
  {
    Beacon result = *this;
    return result += oth;
  }
  // 如果要实现其它类型和 Beacon 的相加,应该使用友元函数
  friend Beacon
  operator+(const int &lhs, const Beacon &rhs)
  {
    Beacon bea;
    bea.score_ = lhs + rhs.score_;
    return bea;
  }

private:
  int score_{0};
};

template <typename Iter, typename StartValue, typename Operator>
auto
accumulate_data(Iter begin, Iter end, StartValue start_value, Operator op)
{
  auto accumulated{start_value};
  // for(auto item = begin; item != end; ++item)
  for(auto item{begin}; item != end; ++item)
  {
    accumulated = op(accumulated, *item);
  }

  return accumulated;
}

double
geometric_mean(std::span<const int> s)
{
  /*
  带等号的初始化（auto x = { ... }）：auto 被推导为 std::initializer_list<T>，因为编译器认为你在构造一个初始化列表。
  不带等号的初始化（auto x{ ... }）：auto 被推导为 { ... } 内的表达式的实际类型（例如 accumulate_data 的返回值类型），因为这被视为直接初始化。
  */
  // ! 错误写法
  // auto accumulate = {
  //     accumulate_data(std::begin(s), std::end(s), 1, std::plus<>())};
  // plus 是加法
  // multiplies 是乘法操作
  // idea, 使用运算符仿函数时,建议不要指定模板参数,使用透明运算符,避免数据丢失(如果指定了int的话,在计算double时会丢失数据)
  auto accumulate{
      accumulate_data(std::begin(s), std::end(s), 1.0, std::multiplies<>())};
  std::cout << std::format("accumulate: {}\n", accumulate);
  return std::pow(accumulate, 1.0 / static_cast<double>(s.size()));
}

void
arithmetic_func_object()
{
  // c++ 中实现了一些算术函数对象:+ - * / %
  // std::plus<int> mp;
  // 透明函数对象,可以用于任何类型,只要他们自身实现了加法运算符
  std::plus<> mp;
  std::cout << std::format("plus 3,5 result:{}\n", mp(3, 5));


  std::vector<int> vec{1, 2, 3, 4, 5};
  std::cout << std::format("vec geometric mean: {}\n", geometric_mean(vec));
}

void
comparison_func_object()
{
  std::cout << std::format("comparsion func object\n");

  // template<typename _Tp, typename _Sequence = vector<_Tp>,
  //    typename _Compare  = less<typename _Sequence::value_type> >
  std::priority_queue<int> pq;
  pq.push(1);
  pq.push(4);
  pq.push(2);
  pq.push(3);

  while(!pq.empty())
  {
    std::cout << std::format("pq cur top: {}\n", pq.top());
    pq.pop();
  }

  std::priority_queue<int, std::vector<int>, std::greater<>> gpq;
  gpq.push(1);
  gpq.push(4);
  gpq.push(3);
  gpq.push(2);
  while(!gpq.empty())
  {
    std::cout << std::format("gpq cur top: {}\n", gpq.top());
    gpq.pop();
  }

  // 建议使用透明运算符,这样可以避免一次额外的复制
  std::set<std::string> untransparent_set;
  // 这个使用会将字面量: beacon 转换成 std::string 然后再执行比较
  if(untransparent_set.find("beacon") != untransparent_set.end())
  {
    std::cout << std::format("found beacon in untransparent_set!\n");
  }
  else
  {
    std::cout << std::format("not found beacon in untransparent_set!\n");
  }
  // !下面会直接报错:stl_set.h(796, 7): Candidate function not viable: no known conversion from 'basic_string_view<char>' to 'const key_type' (aka 'const std::basic_string<char>') for 1st argument
  // untransparent_set.find("beacon"sv);

  std::set<std::string, std::less<>> transparent_set;
  // 这里比较会直接使用字面量 "beacon" const char * 与 容器中的 std::string 进行比较
  if(transparent_set.find("beacon") != transparent_set.end())
  {
    std::cout << std::format("found beacon in transparent_set!\n");
  }
  else
  {
    std::cout << std::format("not found beacon in transparent_set!\n");
  }
  // ! 不会出现错误
  transparent_set.find("beacon"sv);

  // 在非透明比较的情况下，查找需要构造一个与键类型相同的临时对象.

  // todo 自己实现透明运算符比较
}

bool
all_true(const std::vector<bool> &flags)
{
  return accumulate_data(std::begin(flags),
                         std::end(flags),
                         true,
                         std::logical_and<>());
}


template <>
class std::formatter<std::_Bit_reference, char>
{
public:
  constexpr auto
  parse(std::format_parse_context &ctx)
  {
    return ctx.begin();
  }
  template <typename FormatContext>
  auto
  format(const std::_Bit_reference &bit, FormatContext &ctx) const
  {
    return std::format_to(ctx.out(), "{}", static_cast<bool>(bit));
  }
};

//! 不要预埋答案!!!!
//! 自己在看到关于这部分的代码时,看到了书中没有使用std::span<bool>, 自己当时内心特别欣喜
//! 觉得好像是作者的疏忽,没有使用最佳的实现,但是最终发现是自己的理解存在错误
//! std::vector<bool> 并不是自己理解的那样存储的
//! 后面遇到这种情况,自己更应该好奇作者为什么不这样做,而非是窃喜自己发现了新大陆
//! 这里自己想到了大学时自己面试一个活动时,自己迫不及待的想证明自己,然后去擦了黑板...
void
logic_func_object()
{
  std::cout << std::format("logic func object\n");
  // C++ 标准库中的 std::vector<bool> 是一个特化版本，它不像其他类型的 std::vector<T> 那样存储完整的 T 类型对象。
  // 为了节省内存，std::vector<bool> 使用位压缩（bit-packing），将每个 bool 值存储为单个位，而不是完整的字节。
  // vec 的实际类型是:
  std::vector<bool> vec{true, true, true, true};
  std::stringstream ss;
  // 使用 for 循环遍历 std::vector<bool> 时, 获得的是 std::_Bit_reference 而非, bool
  for(const auto &item : vec)
  {
    // ss << std::format("{:s}, ", static_cast<bool>(item));
    //! 这里自己使用:s,出现了错误
    //  item 是 std::_Bit_reference，不是字符串
    /*

    “对 consteval 函数 'std::basic_format_string<char, const std::_Bit_reference &>::basic_format_string<char[7]>' 的调用不是一个常量表达式。”
    //! 自己理解的英语实在是离谱...完全没有弄懂其中的含义......
    “调用一个常量函数，是非常量表达式，无效的常量函数调用”
    Call to consteval function 'std::basic_format_string<char, const std::_Bit_reference &>::basic_format_string<char[7]>' is not a constant expression clang(invalid_consteval_call)
    format(3491, 5): Non-constexpr function '__unmatched_right_brace_in_format_string' cannot be used in a constant expression
    format(3672, 2): In call to '__scanner._M_scan()'
    func_operator.cc(258, 23): In call to 'basic_format_string<char[7]>("{:s}, ")'
    format(170, 3): Declared here
    string-literal
    Type: const char[7]
    Size: 7 bytes
    Passed by const reference as __fmt (converted to format_string<const _Bit_reference &>)
    */
    // ss << std::format("{:s}, ", item);
    ss << std::format("{}, ", item);
  }

  if(all_true(vec))
  {
    std::cout << std::format("vec[{}] all true\n", ss.str());
  }
  else
  {
    std::cout << std::format("vec[{}] exist false\n", ss.str());
  }
}

int
main()
{
  std::vector<int> vec1{1, 2, 3, 4, 5};
  std::vector<int> vec2{5, 4, 3, 2, 1};


  const int a = 2;
  try
  {
    process_matches(vec1, vec2, IsLargeThan(2), print);
  }
  catch(const std::exception &e)
  {
    std::cerr << std::format("{}", e.what());
  }

  Beacon bea;
  Beacon bea2;
  Beacon bea3 = bea + bea2;

  std::cout << std::format("-------------------\n");
  // arithmetic_func_object();
  // comparison_func_object();
  logic_func_object();

  return 0;
}