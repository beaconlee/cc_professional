#include "log.hh"

#include <cxxabi.h>
#include <charconv>


namespace beacon
{

void
to_string()
{
  int a = 701;
  double b = 701.0701;
  float c = 701.0701;
  unsigned int d = 7010701;
  unsigned long int e = 70107010701;
  long double f = 7010701.07010701;

  INFO << "int a to string:" << std::to_string(a);
  INFO << "double b to string:" << std::to_string(b);
  INFO << "float c to string:" << std::to_string(c);
  INFO << "unsigned d int to string:" << std::to_string(d);
  INFO << "unsigned e long int to string:" << std::to_string(e);
  INFO << "long double f to string:" << std::to_string(f);
}

void
string_to_inter_type()
{
  std::string inter{"7010701"};

  auto toi = std::stoi(inter);
  int status = -1;
  std::unique_ptr<char, void (*)(void *)> res{
      abi::__cxa_demangle(typeid(toi).name(), nullptr, nullptr, &status),
      std::free};
  INFO << "toi type:" << ((status == 0) ? res.get() : typeid(toi).name());
  auto toul = std::stoul(inter);

  status = -1;
  std::unique_ptr<char, void (*)(void *)> res2{
      abi::__cxa_demangle(typeid(toul).name(), nullptr, nullptr, &status),
      std::free};
  INFO << "toul type:" << ((status == 0) ? res2.get() : typeid(toul).name());

  auto tol = std::stol(inter);
  status = -1;
  std::unique_ptr<char, void (*)(void *)> res3{
      abi::__cxa_demangle(typeid(tol).name(), nullptr, nullptr, &status),
      std::free};
  INFO << "tol type:" << ((status == 0) ? res3.get() : typeid(tol).name());

  auto toll = std::stoll(inter);
  status = -1;
  std::unique_ptr<char, void (*)(void *)> res4{
      abi::__cxa_demangle(typeid(toll).name(), nullptr, nullptr, &status),
      std::free};
  INFO << "toll type:" << ((status == 0) ? res4.get() : typeid(toll).name());
}

void
string_to_floater_type()
{
  int status = -1;
  std::string floater{"701.0701"};
  auto tof = std::stof(floater);
  status = -1;
  std::unique_ptr<char, void (*)(void *)> res5{
      abi::__cxa_demangle(typeid(tof).name(), nullptr, nullptr, &status),
      std::free};
  INFO << "tof type:" << ((status == 0) ? res5.get() : typeid(tof).name());

  auto tod = std::stod(floater);
  status = -1;
  std::unique_ptr<char, void (*)(void *)> res6{
      abi::__cxa_demangle(typeid(tod).name(), nullptr, nullptr, &status),
      std::free};
  INFO << "tod type:" << ((status == 0) ? res6.get() : typeid(tod).name());

  auto told = std::stold(floater);
  status = -1;
  std::unique_ptr<char, void (*)(void *)> res7{
      abi::__cxa_demangle(typeid(told).name(), nullptr, nullptr, &status),
      std::free};
  INFO << "toll type:" << ((status == 0) ? res7.get() : typeid(told).name());
}

void
string_to_int()
{
  std::string value1{"123456USD"};
  INFO << "123456USD to int:" << std::stoi(value1);

  std::string value2{"USD123456"};
  // 这样会触发一个异常 std::invalid_argument
  // INFO << "USD123456 to int:" << std::stoi(value2);

  std::string value3{"123456USD123456"};
  // 只会转换前面的数字, USD 以及后面的 123456 会被忽略掉
  INFO << "123456USD123456 to int:" << std::stoi(value3);
}

void
string_to_float()
{
  std::string value1{"123456.12345678"};
  auto f1 = std::stof(value1);
  auto d1 = std::stod(value1);
  auto ld = std::stold(value1);
  // 输出精确到小数点后三位
  INFO << "123456.12345678 to f:" << std::to_string(f1);
  // 输出精确到小数点后7位
  INFO << "123456.12345678 to d:" << std::to_string(d1);
  // 输出精确到小数点后7位
  INFO << "123456.12345678 to ld:" << std::to_string(ld);

  std::string value2{"11323f.2342"};
  // 11323.000000
  INFO << "11323f.2342 to d:" << std::to_string(std::stod(value2));

  std::string value3{"11323.2f342"};
  // 11323.200000
  INFO << "11323.2f342 to d:" << std::to_string(std::stod(value3));

  std::string value4{"11323.2342f"};
  // 11323.234200
  INFO << "11323.2342f to d:" << std::to_string(std::stod(value4));
}


void
char_conv()
{
  const size_t buffer_size = 100;
  char buffer[buffer_size];
  int value = 7010701;

  /*
  result 成员: 其中 ptr 指向 buffer 中写入字符最后一个指针位置
  struct to_chars_result
  {
    char* ptr;
    errc ec;

#if __cplusplus > 201703L && __cpp_impl_three_way_comparison >= 201907L
    friend bool
    operator==(const to_chars_result&, const to_chars_result&) = default;
#endif
  };
  */
  // to_chars(char* __first, char* __last, T __value, int __base = 10
  // 默认是是十进制,可选2, 8, 16
  // 如果传入0,则根据value来进行判断: 0x或0X 则是16, 0 则是8, 其它是 10
  auto result = std::to_chars(static_cast<char *>(buffer),
                              static_cast<char *>(buffer) + buffer_size,
                              value);
  if(result.ec == std::errc())
  {
    INFO << "int value to string using std::to_chars: "
         << std::string(static_cast<char *>(buffer), result.ptr);
  }
  else
  {
    ERROR << "Failed to convert int value to string using std::to_chars";
  }

  /*
  /// floating-point format for primitive numerical conversion
  enum class chars_format
  {
    scientific (科学计数法，值为 1)
    fixed (定点表示法，值为 2)
    hex (十六进制浮点表示，值为 4)
    general (通用格式，值为 fixed | scientific，即 3)
    scientific = 1, fixed = 2, hex = 4, general = fixed | scientific
  };
  */
  double double_value = 701.0701;
  result = std::to_chars(static_cast<char *>(buffer),
                         static_cast<char *>(buffer) + buffer_size,
                         double_value,
                         std::chars_format::fixed);
  if(result.ec == std::errc())
  {
    INFO << "double value to string using std::to_chars: "
         << std::string(static_cast<char *>(buffer), result.ptr);
  }
  else
  {
    ERROR << "Failed to convert double value to string using std::to_chars";
  }
  double result_value = 0.0;
  /*
  /// Result type of std::from_chars
  struct from_chars_result
  {
    const char* ptr;
    errc ec;

#if __cplusplus > 201703L && __cpp_impl_three_way_comparison >= 201907L
    friend bool
    operator==(const from_chars_result&, const from_chars_result&) = default;
#endif
  };
  */
  // 结果中的 ptr 指向第一个未被转化的字符
  auto result2 =
      std::from_chars(static_cast<char *>(buffer), result.ptr, result_value);
  // 701.07, 有精度损失
  INFO << "from chars value: " << result_value;
}

} // namespace beacon

int
main(int /*argc*/, char **argv)
{
  logger::Logger::Init(argv[0]);

  beacon::to_string();
  WARNING << "--------------------type--------------------";
  beacon::string_to_inter_type();
  beacon::string_to_floater_type();
  WARNING << "--------------------value--------------------";
  beacon::string_to_int();
  beacon::string_to_float();
  WARNING << "--------------------char--------------------";
  beacon::char_conv();

  return 0;
}
