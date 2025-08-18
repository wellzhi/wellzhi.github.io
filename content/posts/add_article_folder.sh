cd ./content/posts

CURRENT_DATE=$(date +%Y-%m-%d)
ARTICLE_TITLE=$(echo $1 | tr '[:upper:]' '[:lower:]' | tr ' ' '-')
ARTICLE_FOLDER_NAME=${CURRENT_DATE}_${ARTICLE_TITLE}

# 创建文章文件夹
mkdir -p ./$ARTICLE_FOLDER_NAME

# 创建文章文件并写入front matter
cat > ./$ARTICLE_FOLDER_NAME/index.md << EOF
---
date: ${CURRENT_DATE}T04:14:54-08:00
draft: false
params:
  author: wellzhi
title: ${ARTICLE_TITLE}
weight: 10
hideMeta: true
---

EOF

mkdir -p ./$ARTICLE_FOLDER_NAME/pics

