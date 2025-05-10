#include <format>
#include <iostream>
#include <vector>
#include <span>


bool
match(int a, int b)
{
  return a == b;
}


bool
both_odd(int a, int b)
{
  return a % 2 == 1 && b % 2 == 1;
}


void
print_match(size_t position, int a, int b)
{
  std::cout << std::format("match found at position {}({}, {}).\n",
                           position,
                           a,
                           b);
}


template <typename MatchFn, typename HandlerFn>
void
process(std::span<int> s1, std::span<int> s2, MatchFn match, HandlerFn handler)
{
  if(s1.size() != s2.size())
  {
    std::cerr << std::format("err! span1 span2 not equal\n");
    return;
  }
  for(size_t i = 0; i < s1.size(); ++i)
  {
    if(match(s1[i], s2[i]))
    {
      handler(i, s1[i], s2[i]);
    }
  }
}


// 函数指针声明语法: 返回类型 (*指针名)(参数类型列表);
// * 表示这是一个指针
// 结合 (), (*) 表示这是一个指向函数的指针
// 这是一个类型别名, 可以用来声明变量或参数
// 不加名字, 表示一个匿名函数指针类型，仅描述函数的签名（返回类型和参数列表）。
using match_fp = bool (*)(int, int);
using handler_fp = void (*)(size_t, int, int);


// 这是定义了一个函数指针变量! mp 是变量
// 定义一个变量， 并且给这个指针变量进行赋值了
bool (*mp)(int, int) = match;


void
process(std::span<int> s1,
        std::span<int> s2,
        // match_fp match, // 相当于下面的代码
        bool (*match)(int, int),
        // handler_fp handler)
        void (*handler)(size_t, int, int))
{
  if(s1.size() != s2.size())
  {
    std::cerr << std::format("err! span1 span2 not equal\n");
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


class Beacon
{
public:
  void
  out() const
  {
    std::cout << std::format("this is Beacon::out()\n");
  }
};

// using beacon_out = void (*Beacon::out)();

int
main()
{
  // 术语：回调
  // 表示可以调用的东西，可以是函数指针，或者任何行为类似于函数指针的东西：重载了 operator() 的对象，lambda 表达式
  // 一级函数：C++中的函数称之为一级函数，因为函数可以像普通变量那样来使用
  // 高阶函数：C++中接收其它函数作为参数的函数，或者返回函数的函数称为高阶函数

  std::vector<int> vec1{1, 2, 3, 4, 5};
  std::vector<int> vec2{5, 4, 3, 2, 1};
  process(vec1, vec2, match, print_match);
  std::cout << "---------------\n";
  process(vec1, vec2, both_odd, print_match);
  std::cout << "---------------\n";
  process(vec1, vec2, mp, print_match);


  // 直接定义函数指针变量
  bool (*ptr)(int, int) = nullptr; // ptr 是函数指针变量
  ptr = match;                     // 将 match 的地址赋给 ptr
  process(vec1, vec2, ptr, print_match);

  std::cout << "member function:---------------\n";
  // 这里自己第一次看时将后面的{}当成是函数调用了，
  // 实际上这里更应该是列表初始化
  // 前面的是定义一个指向 Beacon 成员函数的指针
  // 后面的 {} 表示进行了初始化的操作
  // void (Beacon::*beacon_test)() const;
  void (Beacon::*beacon_out)() const {&Beacon::out};
  Beacon bea;
  // std::cout<<bea.*beacon_out<<"\n";
  // 之所以在 bea.*beacon_out 的外面加括号，是因为()的优先级高于*
  // 不加括号就会变成 bea.*(beacon_out()) 的形式。
  (bea.*beacon_out)();


  // 也可以使用类型别名
  std::cout << "member function, type name:---------------\n";
  using BeaconMemPtr = void (Beacon::*)() const;
  BeaconMemPtr bout = &Beacon::out;
  (bea.*bout)();

  std::cout << "member function, auto:---------------\n";
  auto bout2{&Beacon::out}; // void (Beacon::*) () const
  (bea.*bout2)();
  return 0;
}
