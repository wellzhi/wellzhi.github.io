# hugo server -D # -D会将草稿文章也渲染出来
# 重新build项目,重新生成，过时内容删除
hugo --gc --minify
# hugo
cd public
git add .
git commit -m "Deploy to GitHub Pages"
git push -f origin gh-pages
cd ..
