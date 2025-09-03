#!/bin/bash

set -e

echo "开始部署到 gh-pages 分支..."

# 1. 构建 Hugo 网站
echo "构建 Hugo 网站..."
hugo --minify

# 2. 检查 public 目录是否存在
if [ ! -d "public" ]; then
    echo "错误：public 目录不存在，请先运行 hugo 命令"
    exit 1
fi

# 3. 进入 public 目录
cd public

# 4. 初始化 git（如果还没有）
if [ ! -d ".git" ]; then
    git init
    git remote add origin $(git -C .. remote get-url origin)
fi

# 5. 添加所有文件
git add .

# 6. 提交更改
if git diff --staged --quiet; then
    echo "没有更改需要提交"
else
    git commit -m "Deploy site $(date '+%Y-%m-%d %H:%M:%S')"
fi

# 7. 强制推送到 gh-pages 分支
git push -f origin HEAD:gh-pages

echo "部署完成！"
echo "网站将在几分钟后在 https://wellzhi.github.io 上可用"

# 8. 返回主目录
cd ..