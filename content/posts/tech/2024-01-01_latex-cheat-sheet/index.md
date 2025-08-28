---
date: 2024-01-01T04:14:54-08:00
draft: false
title: 数学符号 latex 速查表
weight: 100
hideMeta: true
tocOpen: true
---



## 1. 基础语法

| 功能           | 语法                  | 效果             |
| ------------ | ------------------- | -------------- |
| **行内公式**     | `$a^2+b^2=c^2$`     | $a^2+b^2=c^2$  |
| **公式块**      | `$$ a^2+b^2=c^2 $$` | $a^2+b^2=c^2$  |
| **强制大号公式**   | `\dfrac{a}{b}`      | $\dfrac{a}{b}$ |
| **普通分数（小号）** | `\tfrac{a}{b}`      | $\tfrac{a}{b}$ |

---

## 2. 上下标

| 语法           | 效果           |
| :------------- | -------------- |
| `x^2`          | $x^2$          |
| `x_1`          | $x_1$          |
| `x_{ij}`       | $x_{ij}$       |
| `e^{i\pi}+1=0` | $e^{i\pi}+1=0$ |

---

## 3. 分数与根号

| 语法             | 效果                        |
| -------------- | ------------------------- |
| `\frac{a}{b}`  | $\frac{a}{b}$             |
| `\dfrac{1}{2}` | $\dfrac{1}{2}$ （大号，推荐公式块） |
| `\tfrac{1}{2}` | $\tfrac{1}{2}$ （小号，推荐行内）  |
| `\sqrt{x}`     | $\sqrt{x}$                |
| `\sqrt[3]{x}`  | $\sqrt[3]{x}$             |

---

## 4. 求和/积分/极限

| 语法                                | 效果                                |
| --------------------------------- | --------------------------------- |
| `\sum_{i=1}^n i`                  | $\sum_{i=1}^n i$                  |
| `\prod_{i=1}^n i`                 | $\prod_{i=1}^n i$                 |
| `\int_0^1 x^2 dx`                 | $\int_0^1 x^2 dx$                 |
| `\iint_D f(x,y)dA`                | $\iint_D f(x,y)\,dA$              |
| `\lim_{x \to 0} \frac{\sin x}{x}` | $\lim_{x \to 0} \frac{\sin x}{x}$ |

---

## 5. 矩阵

```latex
\begin{bmatrix}
a & b \\
c & d
\end{bmatrix}
```

$$
\begin{bmatrix}
a & b \\
c & d
\end{bmatrix}
$$

常见：

* `pmatrix` → ()
* `bmatrix` → \[]
* `Bmatrix` → {}
* `vmatrix` → ∣ ∣
* `Vmatrix` → ∥ ∥

---

## 6. 分段函数

```latex
f(x)=
\begin{cases}
x^2, & x\ge0 \\
-x,  & x<0
\end{cases}
```

$$
f(x)=
\begin{cases}
x^2, & x\ge0 \\
-x,  & x<0
\end{cases}
$$

---

## 7. 常用符号

| 类型        | 语法                                            | 效果                                            |
| --------- | --------------------------------------------- | --------------------------------------------- |
| **希腊字母**  | `\alpha,\beta,\gamma,\pi,\theta`              | $\alpha,\beta,\gamma,\pi,\theta$              |
| **无穷/导数** | `\infty, \partial, \nabla`                    | $\infty, \partial, \nabla$                    |
| **关系**    | `\leq,\geq,\neq,\approx`                      | $\leq,\geq,\neq,\approx$                      |
| **运算**    | `\cdot,\times,\div,\pm`                       | $\cdot,\times,\div,\pm$                       |
| **集合**    | `\in,\notin,\subset,\supset,\cup,\cap`        | $\in,\notin,\subset,\supset,\cup,\cap$        |
| **逻辑**    | `\forall,\exists,\neg,\land,\lor`             | $\forall,\exists,\neg,\land,\lor$             |
| **箭头**    | `\to,\rightarrow,\Leftarrow,\Rightarrow,\iff` | $\to,\rightarrow,\Leftarrow,\Rightarrow,\iff$ |

---

## 8. 向量 & 装饰

| 语法                    | 效果                    |
| --------------------- | --------------------- |
| `\vec{a}`             | $\vec{a}$             |
| `\overrightarrow{AB}` | $\overrightarrow{AB}$ |
| `\hat{x}`             | $\hat{x}$             |
| `\bar{x}`             | $\bar{x}$             |
| `\tilde{x}`           | $\tilde{x}$           |

---

## 9. 多行对齐

```latex
\begin{aligned}
a &= b+c \\
  &= d+e
\end{aligned}
```

$$
\begin{aligned}
a &= b+c \\
  &= d+e
\end{aligned}
$$

---

## 10. 特殊环境

* **对齐数组**：

```latex
\begin{array}{c|c}
a & b \\ \hline
c & d
\end{array}
```

$$
\begin{array}{c|c}
a & b \\ \hline
c & d
\end{array}
$$

* **省略号**：
  * `\cdots` → $\cdots$
  * `\ldots` → $\ldots$
  * `\vdots` → $\vdots$
  * `\ddots` → $\ddots$

---

