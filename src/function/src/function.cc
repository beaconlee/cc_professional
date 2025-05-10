#include <iostream>
#include <format>
#include <functional>


// std::functional<R(ArgTypes)> 其中 R 是返回类型， ArgTypes 是函数参数类型的逗号分隔列表


void
fun(int num, std::string_view str)
{
  std::cout << std::format("hello, I'm {} years old, enjoy {}\n", num, str);
}


using match_func = std::function<bool(int, int)>;
using match_handler = std::function<void(uint, int, int)>;

// 传入函数对象， 也可以使用函数模板
void
find_match(std::span<int> s1,
           std::span<int> s2,
           const match_func &match,
           const match_handler &handler)
{
  if(s1.size() != s2.size())
  {
    std::cerr << "err! s1 not equal s2\n";
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

template <typename MatchFunc, typename HandlerFunc>
void
find_match_temp(std::span<int> s1,
                std::span<int> s2,
                const MatchFunc &match,
                const HandlerFunc &handler)
{
  if(s1.size() != s2.size())
  {
    std::cerr << "err! s1 not equal s2\n";
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

int
main()
{
  // std::function<void(int, std::string_view)> fc(fun);
  // 注意这里使用列表初始化，自己看时有时还是会将其当作是{}作用域符号
  std::function<void(int, std::string_view)> fc{fun};
  fc(17, "maoqing");

  // 这里算是解决了自己多年的困惑，自己之前一直不知道为什么创建std::function时，需要在前面加一个void，有时又是其它的
  // 现在已经清楚了


  // 这里可以使用模板类型推导, 推导的结果是一个 std::function<void(int, std::string_view)>
  std::function fc2{fun};

  // 直接使用 auto 的话，推导的结果是函数指针， void (*)(int, std::string_view)
  // 对于上面的理解， * 表示是一个指针， 加上()之后，表示这是一个指向函数的指针
  auto fc3{fun};


  std::cout << std::format("-----------------------------\n");
  std::vector<int> vec1{1, 2, 3, 4, 5};
  std::vector<int> vec2{5, 4, 3, 2, 1};
  find_match(vec1, vec2, match, print);


  // 由于函数对象本身也是一个对象， 因此可以使用模板来表示待传入的函数对象
  std::cout << std::format("function template-----------------------------\n");
  find_match_temp<match_func, match_handler>(vec1, vec2, match, print);

  return 0;
}