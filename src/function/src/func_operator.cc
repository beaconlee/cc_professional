#include <cmath>
#include <iostream>
#include <format>
#include <functional>
#include <queue>


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

void comparison_func_objec()
{

}

int
main()
{
  std::vector<int> vec1{1, 2, 3, 4, 5};
  std::vector<int> vec2{5, 4, 3, 2, 1};
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
  arithmetic_func_object();
  return 0;
}