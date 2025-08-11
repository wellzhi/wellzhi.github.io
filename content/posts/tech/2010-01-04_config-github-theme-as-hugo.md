+++
date = '2010-01-04T15:09:06+08:00'
draft = false
title = 'Config Gihub Theme as Hugo'
tocOpen = true
tags = ["Hugo", "PaperMod", "GitHub Pages"]
hideMeta = true
+++

将 GitHub Pages 配置为使用 Hugo 主题 PaperMod，需要以下步骤：

### 1. 安装和配置 Hugo

如果尚未安装 Hugo，先完成以下操作：

安装 Hugo
- 在 macOS 上：brew install hugo
- 在其他平台上，请根据 Hugo 官方文档 安装。

初始化 Hugo 项目
```shell
hugo new site my-website
cd my-website
```

### 2. 添加 PaperMod 主题

将 PaperMod 主题添加到你的 Hugo 项目中：

克隆主题

在项目根目录运行：
```shell
git init
git submodule add https://github.com/adityatelange/hugo-PaperMod.git themes/hugo-PaperMod
```
编辑配置文件

在项目根目录的 config.toml 中设置主题：
```shell
theme = "hugo-PaperMod"
```
配置主题参数

PaperMod 提供丰富的配置选项，可根据需求编辑 config.toml 文件。以下是一个示例：
```shell
baseURL = "https://yourusername.github.io"
languageCode = "en-us"
title = "My Hugo Website"
theme = "hugo-PaperMod"

[params]
  defaultTheme = "auto"  # 主题模式 (auto, light, dark)
  showReadingTime = true
  showWordCount = true
  disableSearch = false

[menu]
  [[menu.main]]
    name = "Home"
    url = "/"
    weight = 1
  [[menu.main]]
    name = "About"
    url = "/about/"
    weight = 2
```

### 3. 添加内容

使用 Hugo 创建内容，例如：
```shell
hugo new posts/my-first-post.md
```
编辑 content/posts/my-first-post.md，为文章添加内容。

### 4. 本地预览

运行以下命令启动本地开发服务器：
```shell
hugo server -D
```
打开浏览器访问 http://localhost:1313 预览网站。

### 5. 部署到 GitHub Pages

将网站部署到 GitHub Pages：

初始化 GitHub 仓库
1. 在 GitHub 上创建一个新的仓库（例如：my-website）。
2. 在本地将仓库与 Hugo 项目关联：
```shell
git remote add origin https://github.com/yourusername/my-website.git
```


### 配置 GitHub Pages 部署分支
#### 1.	生成静态文件：
```shell
hugo -D
```
生成的文件会出现在 public/ 目录下。

#### 2.	将 public/ 目录内容部署到 gh-pages 分支：
```shell
cd public
git init
git add .
git commit -m "Deploy to GitHub Pages"
git branch -M gh-pages
git remote add origin https://github.com/wellzhi/wellzhi.github.io.git
git push -f origin gh-pages
cd ..
```

#### 3.	在 GitHub 仓库设置中启用 GitHub Pages：

- 进入仓库的 Settings → Pages。
- 设置 gh-pages 分支作为 GitHub Pages 的源。

### 6. 测试和发布

等待几分钟后，访问 https://github.com/wellzhi/wellzhi.github.io.git 查看部署结果。

### 7. 日常更新

更新内容后，重复以下步骤：
1. 运行 `hugo -D` 生成静态文件。
2. 将 `public/` 中的内容推送到 gh-pages 分支。

在根目录下，运行一键部署脚本
```shell
hugo -D
cd public
git add .
git commit -m "Deploy to GitHub Pages"
git push -f origin gh-pages
cd ..
```
通过这种方式，你可以轻松使用 PaperMod 主题构建和部署 Hugo 网站到 GitHub Pages！


### 附录
[hugo主题配置](https://github.com/adityatelange/hugo-PaperMod/wiki/Features#theme-switch-toggle-enabled-by-default)
[yml2toml](https://transform.tools/yaml-to-toml)