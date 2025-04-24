#include "log.hh"

char *
copy_string(const char *str)
{
  // idea strlen 之所以返回的长度不包括最后的分隔字符的原因
  // idea 可能的场景就是下面的拼接操作
  char *result = new char[strlen(str) + 1];
  // 这里自己犯了一个错误, new char(strlen(str) + 1), 这里是将一个unsigned int转换成char了
  strcpy(result, str);
  return result;
}

char *
append_str(const char *str1, const char *str2, const char *str3)
{
  char *result = new char[strlen(str1) + strlen(str2) + strlen(str3) + 1];
  strcpy(result, str1);
  strcat(result, str2);
  strcat(result, str3);
  return result;
}

int
main(int /*argc*/, char **argv)
{
  logger::Logger::Init(argv[0]);
  const char *str1{"beacon"};
  const char *str2{"uan"};
  const char *str3{"bquan"};
  const char str_a[] = {"beacon"};

  // strlen 操作符
  // strlen 返回的是字符的长度,不包含结尾的 NUl
  INFO << "strlen(str1):" << strlen(str1);
  // 这里会将一个数组衰减为指针
  // Do not implicitly decay an array into a pointer;
  INFO << "strlen(str_a):" << strlen(str_a);

  // sizeof 操作符
  // sizeof 应用于指针时, 返回的是指针的大小,
  INFO << "sizeof(str1):" << sizeof(str1);
  // sizeof 应用于数组时, 返回的是数组中分配空间的大小
  // c 风格字符串时,包含有结尾的 NUL
  // 虽然数组可以衰减为指针,但是数组和指针还是有区别的
  INFO << "sizeof(str_a):" << sizeof(str_a);

  auto *copy1 = copy_string(str1);
  WARNING << copy1;

  auto *copy2 = append_str(str1, str2, str3);
  WARNING << copy2;

  return 0;
}