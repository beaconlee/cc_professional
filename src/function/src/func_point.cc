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


int
main()
{
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

  return 0;
}
