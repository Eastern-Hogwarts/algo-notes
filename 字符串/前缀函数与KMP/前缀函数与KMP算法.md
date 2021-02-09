# 前缀函数与KMP算法

[TOC]

## 前缀函数

对于一个长度为 $n$ 的字符串 $s$, 其前缀函数为一个同长度的数组 $\pi$。其中 $\pi[i]$ 为既是子串 $s[0\dots i]$ 的前缀同时又是该子串的后缀的最长的**真**前缀长度，即
$$
\pi[i] = \max_{k=0,\dots i}\{ k | s[0\dots k-1] = s[i-(k-1)\dots i] \}
$$
根据定义可知 $\pi[0] = 0$

例子：

```
s  = "a  b  c  a  b  c  d"
pi = [0, 0, 0, 1, 2, 3, 0]

s  = "a  a  b  a  a  a  b"
pi = [0, 1, 0, 1, 2, 2, 3]
```



## 计算前缀函数

### 朴素算法

```
输入：字符串 s
遍历所有前缀子串 s'
	遍历 s' 的所有前缀，寻找与之对应的 s 的后缀
```



遍历所有前缀子串需要 $O(n)$

遍历前缀子串的前缀需要 $O(n)$

比较字符串需要 $O(n)$

朴素算法的时间复杂度是 $O(n^3)$



### 高效算法

一种巧妙地利用了之前的计算的 DP 方法

首先约定符号：$s[0,i] = s[0\dots i-1]$

首先我们能直接得到 $\pi[0] = 0$

假设已知 $p[i]$ 需要计算 $p[i+1]$, 分为两种情况

第一种情况，$s[i+1] = s[\pi[i]]$, 即刚好为之前的前缀加上一个相同的字符，以下图为例，若 $\pi[i]=3$ 且 $s_3 = s_{i+1}$, 自然此时 $\pi[i+1] = \pi[i] + 1$
$$
\underbrace{\overbrace{s_0 ~ s_1 ~ s_2}^{\pi[i]} ~ \overbrace{s_3}^{s_3 = s_{i+1}}}_{\pi[i+1] = \pi[i] + 1} ~ \dots ~ \underbrace{\overbrace{s{i-2} ~ s_{i-1} ~ s_{i}}^{\pi[i]} ~ \overbrace{s_{i+1}}^{s_3 = s_{i + 1}}}_{\pi[i+1] = \pi[i] + 1}
$$
第二种情况，$s[i+1] \neq s[\pi[i]]$, 需要缩短候选字符串。假设我们下一步需要寻找的应该是坐标 $j < \pi[i]$, 满足 $s[0, \dots j-1] = s[i-j+1\dots i]$, 然后再次检查 $s[i+1] = s[j]$ 是否成立. 如果成立则转变为第一种情况，此时 $\pi[i+1] = j+1$. 

那么如何寻找 $j$ ? 因为此时 $s[0\dots\pi[i]-1] = s[i-\pi[i]+1\dots i]$, 子串 $s[0,\dots j-1]$ 必然会在 $s[0\dots\pi[i]-1]$ 和 $s[i-\pi[i]+1\dots i]$ 的前后缀中都出现，也就使得子串 $s[0,\dots j-1]$ 在 $s[0\dots i]$ 中也会作为相同的前后缀出现，那么此时我们需要找的下一个坐标应该是 $j = \pi[\pi[i]-1]$（用迭代的观点来看就是 $j = \pi[j-1]$, 初始 $j=\pi[i]$）. 这样可以一直迭代到 $j=0$, 那么此时如果 $s[i+1] = s[0]$ 则 $\pi[i+1] = 0$, 反之 $\pi[i+1] = 0$.
$$
\overbrace{\underbrace{s_0 ~ s_1}_j ~ s_2 ~ s_3}^{\pi[i]} ~ \dots ~ \overbrace{s_{i-3} ~ s_{i-2} ~ \underbrace{s_{i-1} ~ s_{i}}_j}^{\pi[i]} ~ s_{i+1}
$$
那么得到算法：

- 在一个循环中以 $i = 1$ 到 $i = n - 1$ 的顺序计算前缀函数 $\pi[i]$ 的值（ $\pi[0]$ 被赋值为 $0$ ）。
- 为了计算当前的前缀函数值 $\pi[i]$ ，我们令变量 $j$ 表示右端点位于 $i - 1$ 的最好的后缀的长度。初始时 $j = \pi[i - 1]$ 。
- 通过比较 $s[j]$ 和 $s[i]$ 来检查长度为 $j + 1$ 的后缀是否同时也是一个前缀。如果二者相等，那么我们置 $\pi[i] = j + 1$ ，否则我们减少 $j$ 至 $\pi[j - 1]$ 并且重复该过程。
- 如果 $j = 0$ 并且仍没有任何一次匹配，则置 $\pi[i] = 0$ 并移至下一个下标 $i + 1$ 。

```c++
vector<int> prefix_function(const string & s) {
    size_t n = s.length();
    vector<int> pi(n, 0);
    for (size_t i = 1; i < n; ++i) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j]) j = pi[j-1];
        pi[i] = (s[i] == s[j]) ? j + 1 : 0;
    }
    return pi;
}
```

时间复杂度$O(n)$.



## KMP

可以使用两种视角来看KMP

### 拼接的前缀函数

给定文本 $t$ 和模式 $s$, 找到 $s$ 在 $t$ 中所有的出现坐标

为了简便起见，我们用 $n$ 表示字符串 $s$ 的长度，用 $m$ 表示文本 $t$ 的长度。

我们构造一个字符串 $s + \# + t$ ，其中 $\#$ 为一个既不出现在 $s$ 中也不出现在 $t$ 中的分隔符。接下来计算该字符串的前缀函数。现在考虑该前缀函数除去最开始 $n + 1$ 个值（即属于字符串 $s$ 和分隔符的函数值）后其余函数值的意义。根据定义， $\pi[i]$ 为右端点在 $i$ 且同时为一个前缀的最长真子串的长度，具体到我们的这种情况下，其值为与 $s$ 的前缀相同且右端点位于 $i$ 的最长子串的长度。由于分隔符的存在，该长度不可能超过 $n$ 。而如果等式 $\pi[i] = n$ 成立，则意味着 $s$ 完整出现在该位置（即其右端点位于位置 $i$ ）。注意该位置的下标是对字符串 $s + \# + t$ 而言的。

因此如果在某一位置 $i$ 有 $\pi[i] = n$ 成立，则字符串 $s$ 在字符串 $t$ 的 $i - (n - 1) - (n + 1) = i - 2n$ 处出现。

正如在前缀函数的计算中已经提到的那样，如果我们知道前缀函数的值永远不超过一特定值，那么我们不需要存储整个字符串以及整个前缀函数，而只需要二者开头的一部分。在我们这种情况下这意味着只需要存储字符串 $s + \#$ 以及相应的前缀函数值即可。我们可以一次读入字符串 $t$ 的一个字符并计算当前位置的前缀函数值。

因此 Knuth-Morris-Pratt 算法（简称 KMP 算法）用 $O(n + m)$ 的时间以及 $O(n)$ 的内存解决了该问题。



### 有限状态机



```c++
class KMP {
private:
    vector<size_t> m_pi;
    string m_pattern;
    void buildPrefixFunction() {
        size_t n = m_pattern.size();
        m_pi = vector<size_t>(n, 0);
        for (size_t i=1; i<n; ++i) {
            size_t j = m_pi[i-1];
            while (j > 0 && m_pattern[i] != m_pattern[j]) j = m_pi[j-1];
            m_pi[i] = (m_pattern[i] == m_pattern[j]) ? j + 1 : 0;
        }
    }
public:
    KMP(const string & s) : m_pattern(s) {
        buildPrefixFunction();
    }
    size_t PatternSize() const { return m_pattern.size(); }
    string Pattern() const { return m_pattern; }
    vector<size_t> PrefixFunc() const { return m_pi; }
    size_t Search(const string & t) const {
        size_t m = t.size(), n = PatternSize();
        size_t i = 0;
        for (size_t j = 0; j < m; ++j) {
            while (t[j] != m_pattern[i] && i > 0) i = m_pi[i-1];
            if (t[j] == m_pattern[i]) i += 1;
            if (i == n) return j - i + 1;
        }
        return m;
    }
    vector<size_t> SearchAll(const string &t) const {
        size_t m = t.size(), n = PatternSize();
        size_t i = 0;
        vector<size_t> ans;
        for (size_t j = 0; j < m; ++j) {
            while (t[j] != m_pattern[i] && i > 0) i = m_pi[i-1];
            if (t[j] == m_pattern[i]) i += 1;
            if (i == n) ans.push_back(j - i + 1);
        }
        return ans;
    }
};
```





## Reference

\[1\] https://oi-wiki.org/string/kmp/

\[2\] https://zhuanlan.zhihu.com/p/83334559

[1]: https://oi-wiki.org/string/kmp/
[2]: https://zhuanlan.zhihu.com/p/83334559