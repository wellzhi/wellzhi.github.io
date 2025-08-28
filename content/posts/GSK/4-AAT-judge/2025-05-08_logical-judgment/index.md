+++
date = '2025-05-08T14:52:34+08:00'
draft = true
title = '判断推理-逻辑判断'
+++
## 翻译推理

![思维导图](https://notespace.oss-cn-guangzhou.aliyuncs.com/images/mind.png)

### 1. 题型识别

（1）题干出现明显的逻辑关联词
（2）提问方式为"可以推出"、"不可以推出"

### 2. 解题思维

 <span style="color: red; text-decoration: underline;">先翻译，再推理，注意：尽量不要通过理解语义来解题，会有偏差</span>

### 3. 翻译规则

#### 3.1 前推后

  **(1)** 如果······那么······

  **(2)** 若······则······

  **(3)** 只要······就······

  **(4)** 所有······都······

  **(5)** 为了······一定······


#### 3.2 后推前

  **(1)** 只有······才······

  **(2)** 不······不······

  **(3)** 除非······否则不······

  **(4)** ······是······的基础/假设/前提/关键

  **(5)** ······是······的必要条件/必不可少的条件


#### 3.3 易错点

- **谁是必要条件，谁放在箭头后**
- 
- **除非······否则······**

### 4. 推理规则

```mermaid
graph LR
    %% 定义节点样式
    classDef default fill:#f9f9f9,stroke:#333,stroke-width:1px
    classDef main fill:#e1f5fe,stroke:#01579b,stroke-width:2px,font-weight:bold
    classDef rule fill:#fff8e1,stroke:#ff8f00,stroke-width:1px
    classDef formula fill:#f3e5f5,stroke:#7b1fa2,stroke-width:1px

    %% 主节点
    A[推理规则] --> B[逆否等价]
    A --> C[传递规则]

    %% 逆否等价分支
    B --> D["A→B=-B→-A"]
    D --> E["肯前必肯后，否后必否前，否前肯后不必然"]

    %% 传递规则分支
    C --> |"穿串"|F["①→②, ②→③ ⇒ ①→③"]

    %% 应用样式
    class A main
    class B,C rule
    class D,E,F formula

    %% 调整布局
    linkStyle default stroke:#666,stroke-width:1px
```

### 5. 且或关系


#### 5.1. "且"类系
  
- 和
- 并且
- 既......又......
- 不仅......而且......
- 虽然......但是......

#### 5.2. "或"类系

- ......或者......
- 或者......或者......
- ......和......至少有一个
- 当"或"类系为真时，<span style="color: red; text-decoration: underline;">否定一项可以得到另一项</span>，即是：“否1推1”

5.3. 德·摩根定律

- -(A且B)=-A或-B
- -(A或B)=-A且-B