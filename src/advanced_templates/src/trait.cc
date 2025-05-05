#include <type_traits>
#include <iostream>

int
main()
{
  int x = 42;
  const int &y = x;
  int &&z = 10;
  // decltype 是 c++ 11 中的一个关键字, 是 declared type 的缩写, 意为声明的类型,
  // decltype 的主要作用是在编译时推导表达式的类型，并将该类型用于模板元编程、类型别名、函数返回类型推导等场景。
  // decltype(expression)
  // 具体的内容, 需要再仔细查看, 这里不是重点

  using T1 = decltype(x);     // int
  using T2 = decltype(y);     // const int&
  using T3 = decltype(z);     // int&&
  using T4 = decltype(x + y); // int (纯右值)

  std::cout << std::is_same_v<T1, int> << '\n';         // true
  std::cout << std::is_same_v<T2, const int &> << '\n'; // true
  std::cout << std::is_same_v<T3, int &&> << '\n';      // true
  std::cout << std::is_same_v<T4, int> << '\n';         // true

  // std::decay_t 将类型退化为"值类型", 移除引用, const, volatile 等修饰符
  using T5 = std::decay_t<int &>;       // int
  using T6 = std::decay_t<const int &>; // int
  using T7 = std::decay_t<int[10]>;     // int*
  using T8 = std::decay_t<void()>;      // void(*)()

  // std::is_same_v 用于判断两个类型是否相同
  std::cout << std::is_same_v<T5, int> << '\n';        // true
  std::cout << std::is_same_v<T6, int> << '\n';        // true
  std::cout << std::is_same_v<T7, int *> << '\n';      // true
  std::cout << std::is_same_v<T8, void (*)()> << '\n'; // true
}