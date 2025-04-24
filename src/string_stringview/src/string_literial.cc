#include "log.hh"
#include <iostream>



int
main(int /*argc*/, char **argv)
{
  logger::Logger::Init(argv[0]);

  // 这里的 beacon lee(一个字符串字面量 string literial) 是以值的方式输出的, 而非以变量的方式进行输出
  std::cout << "beacon lee" << "\n";

  // 字符串字面量存储在内存的只读区域, 基于这种方式, 编译器可以重用等效字符串字面量的引用
  // 即使程序使用了 500 次 "beacon" 但是编译器依旧只会创建一个 beacon, 这种技术叫做字面量池(literial pool)
  // 字符串字面量可以赋值给 变量, 但因为字符串字面量位于只读内存区域, 且使用了字面量池技术, 所以可能产生风险.
  const char *str_literial1 = {"beacon"};
  const char *str_literial2 = {"beacon"};
  // 下面两个输出的地址是相同的
  // INFO    2025 424 09:17:08 string_literial.cc:11] str_literial1 addr:0x6150fef1b004
  // INFO    2025 424 09:17:08 string_literial.cc:12] str_literial2 addr:0x6150fef1b004
  INFO << "str_literial1 addr:" << static_cast<const void *>(str_literial1);
  INFO << "str_literial2 addr:" << static_cast<const void *>(str_literial2);


  // best 不要将一个字符串字面量赋值给非 const 变量
  char *str_literial3 = {"beacon"};
  // 这样操作会引发内存错误
  // str_literial3[2] = 'r';

  auto *heap_str = new char[strlen(str_literial1) + 1];
  strcpy(heap_str, str_literial1);
  INFO << "heap_str:" << heap_str; // heap_str:beacon
  // 这里和上面的 str_literial3[2] = 'r' 有着本质的区别
  // heap_str 是位于 堆内存的, 这里修改的是堆内存中的数据
  // str_literial3 是位于 只读 内存区域的, 相当于修改的 "beacon" 这个字面量的值
  heap_str[2] = 'q';
  INFO << "modify head_str:" << heap_str; // modify head_str:beqcon

  // 这里自己犯了个错,没有想到应该是使用 delete[] 的方式
  delete[] heap_str;


  // 将字符串字面量存放在数组中
  // 将字符串字面量用作字符串的初始值
  // 这时编译器会创建一个足够容纳这个字符串的数组,并将字符串复制到这个数组中去.
  // 并不会将其放到只读内存区域,因此可以对其执行操作(修改的是literial_arr)这个数组中的数据
  char literial_arr[]{"beacon"};
  // INFO    2025 424 09:45:41 string_literial.cc:48] literial_arr:beacon
  INFO << "literial_arr:" << literial_arr;
  literial_arr[2] = 'q';
  // INFO    2025 424 09:45:41 string_literial.cc:50] modify literial_arr:beqcon
  INFO << "modify literial_arr:" << literial_arr;


  // raw string literial
  // 无法直接使用 "
  // const char *noraw_str1 =  { "hello " world "!" }
  // 使用转义字符
  const char *noraw_str2 = {"hello \" world \" !"};
  // 自己忘记了原始字符串需要用 () 括起来
  const char *raw_str1 = {R"(hello "world"!)"};

  // 包含多行
  const char *noraw_str3 = {"hello \n world!"};
  const char *raw_str2 = {R"(hello
  world!)"};
  INFO << "raw_str2:" << raw_str2;
  const char *raw_str3 = {R"(\t \n \r)"};
  INFO << "raw_str3(没有转义字符):" << raw_str3;

  // 如果要在原始字符串中嵌入 ) (原始字符串以)为结尾)
  // 语法: R"d-char-sequence(r-char-sequence)d-char-sequence"
  const char *raw_str4 = {R"beacon(现在可以输入 ) 了)beacon"};
  INFO << "raw_str4:" << raw_str4;

  const char *raw_str5 = {R"-(Embedded )" characters)-"};
  INFO << "raw_str5:" << raw_str5;
  return 0;
}