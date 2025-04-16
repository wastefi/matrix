//
// Created by waste on 25-3-12.
//

#ifndef MATRIX_H
#define MATRIX_H

using namespace std;

inline mt19937_64 mrand(random_device{}());

inline double rnd() {
    return static_cast<double>(mrand()) / static_cast<double>(numeric_limits<unsigned long long>::max()) * 20 - 10;
}

template<class T>
class Matrix {
private:
    vector<vector<T> > a;
    int n, m;

public:
    // 无参数构造
    Matrix() {
        n = 0;
        m = 0;
    }

    // 由列表构造
    Matrix(initializer_list<initializer_list<T> > b) {
        for (auto it: b) {
            a.emplace_back(move(it));
        }
        n = a.size();
        m = a[0].size();
    }

    // 由 std::vector 构造
    template<class T2>
    explicit Matrix(const vector<vector<T2> > &b) {
        n = b.size();
        m = b[0].size();
        a.resize(n, vector<T>(m));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = b[i][j];
            }
        }
    }

    // 由矩阵拷贝构造
    template<class T2>
    // ReSharper disable once CppNonExplicitConvertingConstructor
    Matrix(const Matrix<T2> &b) {
        n = b.Row();
        m = b.Col();
        a.resize(n, vector<T>(m, 0));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                a[i][j] = b.get(i, j);
            }
        }
    }

    // 根据矩阵大小、以及参数（可选）构造
    Matrix(int n, int m, const string &ch = "0") {
        this->n = n;
        this->m = m;
        if (ch == "0") {
            a = move(vector<vector<T> >(n, vector<T>(m, 0)));
        } else if (ch == "I") {
            assert(n == m && "initialization failed: n != m");
            a = move(vector<vector<T> >(n, vector<T>(m, 0)));
            for (int i = 0; i < n; i++) {
                a[i][i] = 1;
            }
        } else if (ch == "1") {
            a = move(vector<vector<T> >(n, vector<T>(m, 1)));
        } else if (ch == "R") {
            a = move(vector<vector<T> >(n, vector<T>(m, 0)));
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    a[i][j] = rnd();
                }
            }
        } else if (ch == "r") {
            a = move(vector<vector<T> >(n, vector<T>(m, 0)));
            if (n * m) cout << "Please enter exactly " << n * m << " elements.\n";
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    cin >> a[i][j];
                }
            }
        } else {
            assert(0 && "initialization failed: unknown type");
        }
    }

    // 根据大小，并给定函数构造
    Matrix(int n, int m, const function<T(int, int)> &fun) {
        this->n = n;
        this->m = m;
        a = move(vector<vector<T> >(n, vector<T>(m, 0)));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                a[i][j] = fun(i, j);
            }
        }
    }

    // 构造方阵
    explicit Matrix(int n, string ch = "0"): Matrix(n, n, ch) {
    };


    // 行数
    [[nodiscard]] int Row() const {
        return n;
    }

    // 列数
    [[nodiscard]] int Col() const {
        return m;
    }

    // 方阵的迹
    [[nodiscard]] T trace() const {
        if (n != m) {
            assert(0 && "failed to get trace: n != m");
        }
        T sum = 0;
        for (int i = 0; i < n; i++) {
            sum += a[i][i];
        }
        return sum;
    }

    // 下标运算符，可直接修改
    vector<T> &operator [](const int &x) {
        return a[x];
    }

    // 仅读取元素
    T get(const int &i, const int &j) const {
        return a[i][j];
    }

    // 加法
    template<class T2>
    auto operator +(const Matrix<T2> &x) const {
        if (x.Row() != n || x.Col() != m) {
            assert(0 && "failed to do operation \"+\": different sizes");
        }
        Matrix<decltype(a[0][0] + x.get(0, 0))> c(a);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                c[i][j] += x.get(i, j);
            }
        }
        return c;
    }

    // 加赋值
    template<class T2>
    Matrix &operator +=(const Matrix<T2> &x) {
        *this = *this + x;
        return *this;
    }

    // 减法
    template<class T2>
    auto operator -(const Matrix<T2> &x) const {
        if (x.Row() != n || x.Col() != m) {
            assert(0 && "failed to do operation \"-\": different sizes");
        }
        Matrix<decltype(a[0][0] - x.get(0, 0))> c(a);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                c[i][j] -= x.get(i, j);
            }
        }
        return c;
    }

    // 减赋值
    template<class T2>
    Matrix &operator -=(const Matrix<T2> &x) {
        *this = *this - x;
        return *this;
    }

    // 乘法
    template<class T2>
    auto operator *(const Matrix<T2> &x) const {
        if (x.Row() != m) {
            assert(0 && "failed to do operation \"*\": different sizes");
        }
        Matrix<decltype(a[0][0] * x.get(0, 0))> c(n, x.Col());
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < x.Col(); j++) {
                for (int k = 0; k < m; k++) {
                    c[i][j] += a[i][k] * x.get(k, j);
                }
            }
        }
        return c;
    }

    // 乘赋值
    template<class T2>
    Matrix &operator *=(const Matrix<T2> &x) {
        *this = *this * x;
        return *this;
    }

    // 等于
    template<class T2>
    bool operator ==(const Matrix<T2> &x) const {
        if (x.Row() != n || x.Col() != m) return false;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (a[i][j] != x[i][j]) return false;
            }
        }
        return true;
    }

    // 不等于
    template<class T2>
    bool operator !=(const Matrix<T2> &x) const {
        return !(*this == x);
    }

    // 按格式输出
    void print() const {
        cout << "Type = Matrix<" << typeid(T).name() << ">\n";
        cout << "Size = ";
        if (n == 0 && m == 0) {
            cout << "[empty]\n";
        } else {
            cout << n << " x " << m << "\n";
        }

        bool f = static_cast<T>(1) / 2 != 0;

        auto dig = [](int x) -> int {
            int cnt = 0;
            if (x == 0) cnt++;
            if (x < 0) cnt++;
            while (x) {
                x /= 10;
                cnt++;
            }
            return cnt;
        };
        int sz = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                sz = max(sz, dig(a[i][j]));
            }
        }
        for (int i = 0; i < n; i++) {
            if (n == 1) {
                cout << " < ";
            } else if (i == 0) {
                cout << " / ";
            } else if (i == n - 1) {
                cout << " \\ ";
            } else {
                cout << " | ";
            }
            for (int j = 0; j < m; j++) {
                if (f) cout << fixed << setprecision(3) << setw(sz + 6) << a[i][j] << " ";
                else cout << setw(sz + 2) << a[i][j] << " ";
            }
            if (n == 1) {
                cout << ">";
            } else if (i == 0) {
                cout << "\\";
            } else if (i == n - 1) {
                cout << "/";
            } else {
                cout << "|";
            }
            cout << "\n";
        }
    }

    // 矩阵转置
    Matrix &trans() {
        vector<vector<T> > b(m, vector<T>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                b[j][i] = a[i][j];
            }
        }
        swap(n, m);
        a = move(b);
        return *this;
    }
};

// 幂运算
template<typename T>
Matrix<T> pow(Matrix<T> a, int x) {
    if (a.Row() != a.Col()) {
        assert(0 && "failed to do operation pow: different sizes");
    }
    Matrix<T> res(a.Row(), "I");
    while (x) {
        if (x & 1) res *= a;
        a *= a;
        x >>= 1;
    }
    return res;
}

#endif //MATRIX_H
