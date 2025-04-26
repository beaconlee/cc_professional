#include "log.hh"

#include <format>
#include <string>
#include <string_view>


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


int
main(int /*argc*/, char **argv)
{
  logger::Logger::Init(argv[0]);

  basic();
  style();
  precision();
  alternate();
  type();
  return 0;
}
