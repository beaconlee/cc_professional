#include <vector>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <iostream>


class User
{
public:
  User(std::string _name, int _age, double _score)
    : name_(std::move(_name))
    , age_(_age)
    , score_(_score)
  {}

  [[nodiscard]] std::string_view
  name() const
  {
    return name_;
  }

  [[nodiscard]] int
  age() const
  {
    return age_;
  }

  [[nodiscard]] double
  score() const
  {
    return score_;
  }

private:
  std::string name_;
  int age_;
  double score_;
};

std::string
format_table(const std::vector<User> &users)
{
  std::ostringstream oss;
  // 表头
  oss << std::left << std::setw(20) << "Name" << std::setw(10) << "Age"
      << std::setw(10) << "Score" << "\n";
  oss << std::string(40, '-') << "\n";

  // 表格内容
  for(const auto &user : users)
  {
    oss << std::left << std::setw(20) << user.name() << std::setw(10)
        << user.age() << std::fixed << std::setprecision(2) << std::setw(10)
        << user.score() << "\n";
  }

  return oss.str();
}

void
isstream()
{
  std::string str{"Alice,25,95.5,Bob,30,88.75"};
  // 将上面的 str 转换成 字符串流, 以此来简化操作
  std::istringstream is(str);
  std::string name{};
  std::string temp{};
  int age{0};
  double score{0.};

  std::vector<User> usrs;
  while(std::getline(is, name, ','))
  {
    if(!std::getline(is, temp, ','))
    {
      break;
    }
    try
    {
      age = std::stoi(temp);
    }
    catch(const std::exception &e)
    {
      std::cerr << "Error converting age:" << temp << "\n";
      break;
    }

    if(!std::getline(is, temp, ','))
    {
      break;
    }
    try
    {
      score = std::stoi(temp);
    }
    catch(const std::exception &e)
    {
      std::cerr << "Error converting score:" << score << "\n";
      break;
    }
    usrs.emplace_back(name, age, score);
  }

  // 输出解析结果
  for(const auto &usr : usrs)
  {
    std::cout << "Name: " << usr.name() << ", Age: " << usr.age()
              << ", Score: " << usr.score() << "\n";
  }
}

void
osstream()
{
  std::vector<User> users = {{"Alice", 25, 95.5}, {"Bob", 30, 88.75}};
  std::string table = format_table(users);
  std::cout << table;
}

void
osstream2()
{
  std::cout << "Enter tokens.\nControl+D followed by Enter to end.\n";
  std::ostringstream os;

  while(std::cin)
  {
    std::string next_token;
    std::cin >> next_token;
    os << next_token;
  }

  std::cout << "result: " << os.str() << "\n";
}

void
cur_position_istream()
{
  // 输入字符串
  std::string str = "Alice 25 95.5";
  std::istringstream iss(str);

  // 读取数据并跟踪当前位置
  std::string name;
  int age{0};
  double score{0.};
  // 初始位置
  std::cout << "Initial get position: " << iss.tellg() << "\n";
  // 读取 name
  iss >> name;
  std::cout << "After reading name (" << name
            << "): get position = " << iss.tellg() << "\n";
  // 读取 age
  iss >> age;
  std::cout << "After reading age (" << age
            << "): get position = " << iss.tellg() << "\n";
  // 读取 score
  iss >> score;
  std::cout << "After reading score (" << score
            << "): get position = " << iss.tellg() << "\n";
  // 检查是否到达末尾
  if(iss.eof())
  {
    std::cout << "Reached end of stream\n";
  }
  // 回退到开头
  iss.seekg(0);
  std::cout << "After seekg(0): get position = " << iss.tellg() << "\n";

  // 再次读取 name
  iss >> name;
  std::cout << "Read name again (" << name
            << "): get position = " << iss.tellg() << "\n";

  // 使用 std::string 完成对应的操作,需要如下复杂的实现:
  // std::string str = "Alice 25 95.5";
  // size_t pos = 0;
  // std::string name = str.substr(pos, str.find(' ', pos) - pos);
  // pos = str.find(' ', pos) + 1;
  // std::string age_str = str.substr(pos, str.find(' ', pos) - pos);
  // int age = std::stoi(age_str);
  // pos = str.find(' ', pos) + 1;
  // double score = std::stod(str.substr(pos));
}

void
cur_position_ostream()
{
  // 创建 ostringstream
  std::ostringstream oss;

  // 初始位置
  std::cout << "Initial put position: " << oss.tellp() << "\n";

  // 写入 name
  oss << "Alice";
  std::cout << "After writing 'Alice': put position = " << oss.tellp() << "\n";

  // 写入 age
  oss << " " << 25;
  std::cout << "After writing ' 25': put position = " << oss.tellp() << "\n";

  // 写入 score with formatting
  oss << " " << std::fixed << std::setprecision(2) << 95.5;
  std::cout << "After writing ' 95.50': put position = " << oss.tellp() << "\n";

  // 获取最终字符串
  std::string result = oss.str();
  std::cout << "Result: " << result << "\n";

  // 移动到开头并覆盖
  oss.seekp(0);
  oss << "Bob";
  std::cout << "After overwriting with 'Bob': put position = " << oss.tellp()
            << "\n";
  std::cout << "New result: " << oss.str() << "\n";

  // 同样的操作,如果用 std::string, 实现会更加复杂:
  // std::string str;
  // str += "Alice";
  // str += " ";
  // str += std::to_string(25);
  // str += " ";
  // str += std::to_string(95.5); // 需额外格式化
  // 要覆盖开头部分，需要用 replace 或重新赋值：
  // str.replace(0, 3, "Bob");
}

void
cur_position()
{
  // 字符串流的当前位置,由底层的std::streambuf实现:
  /*
  字符串流的当前位置由底层的 std::streambuf 实现：

  输入操作（std::istringstream）：
    std::streambuf 维护三个指针：eback()（缓冲区开头）、gptr()（当前读取位置）、egptr()（缓冲区末尾）。
    每次读取（如 >> 或 std::getline）会移动 gptr()，tellg() 返回 gptr() 相对于缓冲区开头的偏移量。
  输出操作（std::ostringstream）：
    维护 pbase()（输出缓冲区开头）、pptr()（当前写入位置）、epptr()（缓冲区末尾）。
    每次写入（如 <<）会移动 pptr()，tellp() 返回 pptr() 的偏移量。
  动态缓冲：字符串流的缓冲区会自动扩展（比如 std::ostringstream 写入数据时），当前位置随之更新。
  这些机制对用户是透明的，开发者只需通过 <<、>> 或 seekg/seekp 操作即可利用当前位置。
  */
  // 当前位置分为输入和输出两种
  //  输入当前位置: get pointer,对应输入流,表示下一次读取的字符位置
  //  输出当前位置: put pointer,对应输出流,表示下一次写入的字符位置
  //  相比于std::string,虽然可以通过索引(str[5])来访问特定的位置,但它不维护一个"当前位置"状态,你需要显示指定(通过substr,find,或者手动索引),而字符串流会自动记录当前的位置,简化连续的读写操作

  // 顺序读写：流操作，'<<','>>',std::getline等会自动更新当前位置
  // 查询或修改位置： tellg()查询写入当前位置,tellp()查询输出当前位置,seekg()设置当前写入位置,seekp()设置当前输出位置.
  // 流状态,会根据当前位置是判断是否到达数据末尾(eof())或发生错误.
  cur_position_istream();
  cur_position_ostream();
}



int
main()
{
  osstream();
  std::cout << "-------------------------------\n";
  isstream();
  return 0;
}