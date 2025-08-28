---
title: "Leaf Bundle实现内容复用"
date: 2010-01-01
description: "使用Leaf Bundle在Hugo+PaperMod中复用子Markdown文件，实现自动嵌入或点击展开内容。"
tags: ["Hugo", "PaperMod", "Markdown", "Leaf Bundle", "Shortcode"]
draft: false
---

在 Hugo + PaperMod 中，我们经常会遇到这样的需求：

- 一篇文章内容很长，希望拆分成多个 Markdown 文件；
- 希望复用某些固定内容片段，比如免责声明、版权信息等；
- 希望在文章中点击标题才展开某些内容，而不是直接展开。

本文介绍两种实现方式：**resourceinclude**（自动嵌入内容）和 **collapsible**（点击展开内容），并结合 Leaf Bundle 使用。

---

## 先看看最终效果

---

## 开篇

这里是前言。

## 第一部分

{{< resourceinclude "part1.md" >}}

{{< collapsible "part1.md" "📖 第一部分（点击展开）" >}}

## 第二部分

{{< resourceinclude "part2.md" >}}

{{< collapsible "part2.md" "📖 第二部分（点击展开）" >}}

## 插图

[图片](image1.png)

## 附件

[下载 PDF](notes.pdf)

## 结尾

谢谢阅读。

---

## 什么是 Leaf Bundle

Leaf Bundle（叶子包）是 Hugo 的 Page Bundle 类型之一，其特点是：

- 一个目录包含一个 `index.md`，生成唯一的页面；
- 同目录下的其他文件（Markdown、图片、PDF 等）属于该页面资源；
- 子文件不会单独生成页面，可以在主文档中引用。

示例目录结构：

```

content/posts/my-post/
├── index.md      # 主文档
├── part1.md      # 子文档
├── part2.md
├── image1.png
└── notes.pdf

````

---

## 方法一：resourceinclude 自动嵌入内容

### 1. 创建 Shortcode

在项目目录 `layouts/shortcodes/resourceinclude.html` 中写入：

```go
{{ $file := .Get 0 }}
{{ with .Page.Resources.GetMatch $file }}
  {{ .Content }}
{{ else }}
  <p style="color:red;">❌ 未找到资源：{{ $file }}</p>
{{ end }}
````

* `.Page.Resources.GetMatch` 会在当前 Leaf Bundle 中查找资源；
* `.Content` 会渲染 Markdown 成 HTML。

### 2. 使用示例

`index.md` 内容：

```markdown
---
title: "我的大文章"
date: 2025-08-28
---

# 主文档

前言……

## 第一部分
{{< resourceinclude "part1.md" >}}

## 第二部分
{{< resourceinclude "part2.md" >}}
```

子文件 `part1.md`：

```markdown
---
title: "第一部分"
---

这里是第一部分的内容。
```

子文件 `part2.md`：

```markdown
---
title: "第二部分"
---

这里是第二部分的内容。
```

**效果**：Hugo 构建时，`part1.md` 和 `part2.md` 会自动嵌入主文档中。

---

## 方法二：collapsible 点击展开内容

如果希望子内容 **默认折叠，需要点击才展开**，可以使用 `<details>` + shortcode 实现。

### 1. 创建 Shortcode

`layouts/shortcodes/collapsible.html`：

```go
{{ $file := .Get 0 }}
{{ $title := .Get 1 }}
{{ $path := path.Join .Page.File.Dir $file }}

{{ if fileExists $path }}
<details class="collapsible-block">
  <summary>{{ $title }}</summary>
  <div class="collapsible-content">
    {{ readFile $path | markdownify }}
  </div>
</details>
{{ else }}
<p style="color:red;">❌ 未找到资源：{{ $file }}</p>
{{ end }}
```

* 使用 `readFile` + `markdownify`，无需子 Markdown 文件带 front matter；
* `<details>` 标签支持折叠展开，PaperMod 默认样式兼容。

### 2. 使用示例

`index.md` 内容：

```markdown
---
title: "我的大文章"
date: 2025-08-28
---

# 主文档

前言……

{{< collapsible "part1.md" "📖 第一部分（点击展开）" >}}

{{< collapsible "part2.md" "📖 第二部分（点击展开）" >}}
```

**效果**：页面显示两个折叠标题，点击才展开对应内容。

### 3. 可选 CSS 美化

在 `assets/css/custom.css`：

```css
.collapsible-block summary {
  cursor: pointer;
  font-weight: bold;
  margin: 0.5em 0;
}

.collapsible-content {
  padding: 0.5em 1em;
  border-left: 3px solid #ccc;
}
```

---

## 总结

* **Leaf Bundle** 是 Hugo 内容组织的核心工具，一篇文章和所有附属资源放在同一目录；
* **resourceinclude**：自动嵌入子 Markdown，适合内容复用；
* **collapsible**：点击展开子内容，适合长文折叠或隐藏辅助信息；
* 使用 Leaf Bundle + shortcodes，可以让文章结构清晰、内容复用方便、页面美观。



