hugo -D
cd public
git add .
git commit -m "Deploy to GitHub Pages"
git push -f origin gh-pages
cd ..