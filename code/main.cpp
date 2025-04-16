#include<bits/stdc++.h>

#include <utility>
#include "matrix.h"
using namespace std;

vector<string> opt;
map<string, variant<Matrix<int>, Matrix<double> > > mp;
int y, p;

// 冲突处理：check()在覆盖矩阵时提供交互确认
bool check(const string &name) {
    if (!y && mp.contains(name)) {
        cout << "There has been a matrix named " << name << ".\n";
        cout << "Would you like to recover it? (y / n)\n";
        string s;
        cin >> s;
        if (s != "y") {
            return false;
        }
    }
    return true;
}

// 类型验证：is_int()验证字符串是否为纯数字
bool is_int(const string &s) {
    for (int i = 0; i < s.length(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return true;
};

// 输入解析：split()将用户输入拆分为可处理的数据，将输入字符串按空格分割成多个参数
void split(vector<string> &res, const string &s, char ch) {
    res.clear();
    string tmp;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ' ') {
            if (!tmp.empty()) res.push_back(tmp);
            tmp.clear();
        } else {
            tmp += s[i];
        }
    }
    if (!tmp.empty()) res.push_back(tmp);
}

// 名称参数读取：验证参数索引是否存在，名称不以'-'开头(避免与选项混淆)
bool read_name(int id, const string &cmd_name = "", const string &arg_name = "") {
    bool flag = !cmd_name.empty();
    if (opt.size() <= id) {
        if (flag) cout << "Failed to execute command \"" << cmd_name << "\": lack of arguments <" << arg_name << ">.\n";
        return false;
    }
    const string &name = opt[id];
    if (name[0] == '-') {
        if (flag) cout << "Failed to execute command \"" << cmd_name << "\": <" << arg_name <<
                  "> shouldn't be begin with '-'.\n";
        return false;
    }
    return true;
}

// 整型参数读取：不允许存在负数，额外调用is_int验证格式显示更具体的类型错误信息
bool read_int(int id, const string &cmd_name = "", const string &arg_name = "") {
    bool flag = !cmd_name.empty();

    if (opt.size() <= id) {
        if (flag) cout << "Failed to execute command \"" << cmd_name << "\": lack of arguments <" << arg_name << ">.\n";
        return false;
    }
    if (!is_int(opt[id])) {
        if (flag) cout << "Failed to execute command \"" << cmd_name << "\": except an integer, but not.\n";
        return false;
    }

    return true;
}

// 名称存在性检查：如果名称已存在且未设置-y选项，显示确认提示并根据用户输入决定是否继续，防止意外覆盖重要矩阵
bool in_name(const string &name, const string &cmd_name) {
    bool flag = !cmd_name.empty();
    if (!mp.contains(name)) {
        if (flag) cout << "Failed to execute command \"" << cmd_name << "\": there is not a matrix named \"" << name <<
                  "\".\n";
        return false;
    }
    return true;
}

// 输出函数：如果不存在这个矩阵，将会输出不存在。
void print(const string &name) {
    if (!mp.contains(name)) {
        cout << "There isn't a matrix named \"" << name << "\".\n";
        return;
    }
    cout << "Name = \"" << name << "\"\n";
    visit([](const auto &p) { p.print(); }, mp[name]);
}

// 按大小创建新矩阵
void size_new() {
    const string &name = opt[1];
    int j = 2;
    int n = 0, m = 0;

    if (read_int(2)) {
        j = 3;
        if (opt[2].size() > 3) {
            cout << "Failed to execute command \"new\": range of the size is too big (0 - 999).\n";
            return;
        }
        n = stoi(opt[2]);
        m = n;
        if (read_int(3)) {
            j = 4;
            if (opt[3].size() > 3) {
                cout << "Failed to execute command \"new\": range of the size is too big (0 - 999).\n";
                return;
            }
            m = stoi(opt[3]);
        }
    }

    string t = "int";
    string o = "0";

    for (int i = j; i < opt.size(); i++) {
        if (opt[i] == "-p" || opt[i] == "-y") continue;
        else if (opt[i] == "-t") {
            if (!read_name(i + 1, "new", "type")) {
                return;
            }
            t = opt[i + 1];
            i++;
        } else if (opt[i] == "-o") {
            if (!read_name(i + 1, "new", "type")) {
                return;
            }
            o = opt[i + 1];
            i++;
        } else {
            if (opt[i][0] == '-') {
                cout << "Failed to execute command \"new\": unknown options " << opt[i] << ".\n";
            } else {
                cout << "Failed to execute command \"new\": single argument " << opt[i] << ".\n";
            }
            return;
        }
    }

    if (!y && !check(name)) {
        return;
    }
    if (t == "int" || t == "i") {
        mp[name] = Matrix<int>(n, m, o);
    } else if (t == "double" || t == "d") {
        mp[name] = Matrix<double>(n, m, o);
    } else {
        cout << "Failed to execute command \"new\": unsupported data type " << t << ".\n";
        return;
    }
}

// 复制矩阵
void copy() {
    if (!read_name(1, "copy", "from_name")) {
        return;
    }
    const string &from_name = opt[1];
    if (!in_name(from_name, "copy")) {
        return;
    }
    if (!read_name(2, "copy", "to_name")) {
        return;
    }
    const string &to_name = opt[2];
    if (!y && !check(to_name)) {
        return;
    }
    mp[to_name] = mp[from_name];
}

// 运算操作：包含矩阵加法、矩阵减法、矩阵乘法
void opr(const string &cmd) {
    if (!read_name(1, cmd, "name1")) {
        return;
    }
    if (!read_name(2, cmd, "name2")) {
        return;
    }

    const string &name1 = opt[1];
    const string &name2 = opt[2];
    string new_name;
    for (int i = 3; i < opt.size(); i++) {
        if (opt[i] == "-p" || opt[i] == "-y") continue;
        else if (opt[i] == "-s") {
            if (!read_name(i + 1, "opr", "new_name")) {
                return;
            }
            new_name = opt[i + 1];
            i++;
        } else {
            if (opt[i][0] == '-') {
                cout << "Failed to execute command \"new\": unknown options " << opt[i] << ".\n";
            } else {
                cout << "Failed to execute command \"new\": single argument " << opt[i] << ".\n";
            }
            return;
        }
    }
    variant<Matrix<int>, Matrix<double> > tmp;
    if (cmd == "add") {
        visit([&](const auto &p1, const auto &p2) { tmp = p1 + move(p2); }, mp[name1], mp[name2]);
    } else if (cmd == "sub") {
        visit([&](const auto &p1, const auto &p2) { tmp = p1 - move(p2); }, mp[name1], mp[name2]);
    } else if (cmd == "mul") {
        visit([&](const auto &p1, const auto &p2) { tmp = p1 * move(p2); }, mp[name1], mp[name2]);
    } else {
        cerr << "how do you get this?\n";
        assert(0);
    }
    if (p) {
        visit([&](const auto &p1) { p1.print(); }, tmp);
    }
    if (!y && !check(new_name)) {
        return;
    }
    mp[new_name] = tmp;
}

// 幂运算操作
void mpow() {
    if (!read_name(1, "pow", "name")) {
        return;
    }
    if (!read_int(2, "pow", "power")) {
        return;
    }

    const string &name1 = opt[1];
    const int power = stoi(opt[2]);
    string new_name;
    for (int i = 3; i < opt.size(); i++) {
        if (opt[i] == "-p" || opt[i] == "-y") continue;
        else if (opt[i] == "-s") {
            if (!read_name(i + 1, "opr", "new_name")) {
                return;
            }
            new_name = opt[i + 1];
            i++;
        } else {
            if (opt[i][0] == '-') {
                cout << "Failed to execute command \"pow\": unknown options " << opt[i] << ".\n";
            } else {
                cout << "Failed to execute command \"pow\": single argument " << opt[i] << ".\n";
            }
            return;
        }
    }
    variant<Matrix<int>, Matrix<double> > tmp;
    visit([&](const auto &p1) { tmp = pow(p1, power); }, mp[name1]);

    if (p) {
        visit([&](const auto &p1) { p1.print(); }, tmp);
    }
    if (!new_name.empty() && !y && !check(new_name)) {
        return;
    }
    mp[new_name] = tmp;
}

int main() {
    cout << "print 'help' to get the manual.\n";
    string str;

    while (getline(cin, str)) {
        split(opt, str, ' ');

        if (opt.empty()) continue;
        string cmd = opt[0];
        p = false, y = false;
        for (const auto &i: opt) {
            if (i == "-y") y = true;
            if (i == "-p") p = true;
        }

        if (cmd == "new") {
            if (!read_name(1, cmd, "name")) {
                continue;
            }
            const string &name = opt[1];
            size_new();
            if (p) {
                print(name);
            }
        } else if (cmd == "copy") {
            copy();
        } else if (cmd == "add" || cmd == "sub" || cmd == "mul") {
            opr(cmd);
        } else if (cmd == "pow") {
            mpow();
        } else if (cmd == "print") {
            if (!read_name(1, cmd, "name")) {
                continue;
            }
            if (opt.size() > 2) {
                cout << "Failed to execute command \"print\": too many arguments.\n";
                continue;
            }
            print(opt[1]);
        } else if (cmd == "delete") {
            if (!read_name(1, cmd, "name")) {
                continue;
            }
            const string &name = opt[1];
            mp.erase(name);
        } else if (cmd == "quit") {
            break;
        } else if (cmd == "help") {
            cout << "Usage:\n"
                    "   new <name> <row_size> <col_size> [-p] [-t] [-o]\n"
                    "   new <name> <size> [-p] [-t] [-o]\n"
                    "   copy <from_name> <to_name>\n"
                    "   add <name1> <name2> [-p] [-s]\n"
                    "   sub <name1> <name2> [-p] [-s]\n"
                    "   mul <name1> <name2> [-p] [-s]\n"
                    "   pow <name> <power> [-p] [-s]\n"
                    "   print <name>\n"
                    "   delete <name>\n"
                    "   quit\n"
                    "Options:\n"
                    "   -p              print the matrix after generating\n"
                    "   -t <type>       set the data type of matrix [default: int]\n"
                    "       int, i          integer\n"
                    "       double, d       double precision floating-point number\n"
                    "       modint          (to be done)\n"
                    "   -o <option>     set the option for generating matrix [default: 0]\n"
                    "       0               zero matrix\n"
                    "       1               1s matrix\n"
                    "       I               identity matrix\n"
                    "       R               random matrix in range [-10, 10]\n"
                    "   -s <new_name>   store the result in \"new_name\"\n";
        } else {
            cout << "unknown command: " << cmd << "\n";
        }
    }
}

/*

Usage:
    new <name> <row_size> <col_size> [-p] [-t] [-o]
    new <name> <size> [-p] [-t] [-o]
    copy <from_name> <to_name>
    add <name1> <name2> [-p] [-s]
    sub <name1> <name2> [-p] [-s]
    mul <name1> <name2> [-p] [-s]
    pow <name> <power> [-p] [-s]
    print <name>
    delete <name>
    quit
Options:
    -p              print the matrix after generating
    -t <type>       set the data type of matrix [default: int]
        int, i          integer
        double, d       double precision floating-point number
        modint          (to be done)
    -o <option>     set the option for generating matrix [default: 0]
        0               zero matrix
        1               1s matrix
        I               identity matrix
        R               random matrix in range [-10, 10]
        r               read from input
    -s <new_name>   store the result in "new_name"

*/
