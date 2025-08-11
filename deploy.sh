# hugo server -D # -D会将草稿文章也渲染出来
hugo
cd public
git add .
git commit -m "Deploy to GitHub Pages"
git push -f origin gh-pages
cd ..