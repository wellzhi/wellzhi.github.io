import os
import re
import requests
import json
from pathlib import Path
import frontmatter

class WechatMPSender:
    def __init__(self, app_id, app_secret):
        self.app_id = app_id
        self.app_secret = app_secret
        self.access_token = None
        
    def get_access_token(self):
        """获取微信公众号access_token"""
        url = f"https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid={self.app_id}&secret={self.app_secret}"
        response = requests.get(url)
        data = response.json()
        
        if 'access_token' in data:
            self.access_token = data['access_token']
            return True
        else:
            print(f"获取access_token失败: {data}")
            return False
    
    def create_draft(self, title, content, author="", digest=""):
        """创建草稿"""
        if not self.access_token:
            if not self.get_access_token():
                return False
        
        url = f"https://api.weixin.qq.com/cgi-bin/draft/add?access_token={self.access_token}"
        
        # 构建文章数据
        articles = {
            "articles": [{
                "title": title,
                "author": author,
                "digest": digest,
                "content": content,
                "content_source_url": "",
                "thumb_media_id": "",  # 如果有封面图片，需要先上传获取media_id
                "show_cover_pic": 0,
                "need_open_comment": 0,
                "only_fans_can_comment": 0
            }]
        }
        
        response = requests.post(url, json=articles)
        result = response.json()
        
        if result.get('errcode') == 0:
            print(f"文章 '{title}' 已成功添加到草稿箱")
            return True
        else:
            print(f"添加文章 '{title}' 到草稿箱失败: {result}")
            return False

def parse_markdown_file(file_path):
    """解析markdown文件，提取front matter和内容"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            post = frontmatter.load(f)
            
        # 获取front matter
        metadata = post.metadata
        
        # 获取文章内容
        content = post.content
        
        # 将front matter转换为字符串格式
        front_matter_str = "---\n"
        for key, value in metadata.items():
            front_matter_str += f"{key}: {value}\n"
        front_matter_str += "---\n\n"
        
        # 组合完整内容
        full_content = front_matter_str + content
        
        return {
            'title': metadata.get('title', os.path.basename(file_path)),
            'content': full_content,
            'author': metadata.get('author', ''),
            'digest': metadata.get('description', '')[:120] if metadata.get('description') else ''  # 摘要限制120字符
        }
    except Exception as e:
        print(f"解析文件 {file_path} 失败: {e}")
        return None

def get_all_posts(posts_dir):
    """获取posts目录下的所有文章文件"""
    posts = []
    posts_path = Path(posts_dir)
    
    if not posts_path.exists():
        print(f"目录 {posts_dir} 不存在")
        return posts
    
    # 遍历所有.md文件和文件夹中的index.md文件
    for item in posts_path.iterdir():
        if item.is_file() and item.suffix == '.md':
            # 直接的.md文件
            posts.append(item)
        elif item.is_dir():
            # 文件夹中的index.md文件
            index_file = item / 'index.md'
            if index_file.exists():
                posts.append(index_file)
    
    return posts

def main():
    # 配置微信公众号信息
    APP_ID = "wx8b425f039214ed2a"  # 请替换为您的AppID
    APP_SECRET = "acae75c8580eecbbb6ff5834497126a0"  # 请替换为您的AppSecret
    
    # 检查配置
    if APP_ID == "your_app_id_here" or APP_SECRET == "your_app_secret_here":
        print("请先配置您的微信公众号AppID和AppSecret")
        return
    
    # 初始化微信公众号发送器
    sender = WechatMPSender(APP_ID, APP_SECRET)
    
    # 获取所有文章
    posts_dir = "content/posts"
    posts = get_all_posts(posts_dir)
    
    if not posts:
        print("未找到任何文章文件")
        return
    
    print(f"找到 {len(posts)} 篇文章")
    
    # 发送每篇文章到草稿箱
    success_count = 0
    for post_file in posts:
        print(f"\n处理文章: {post_file}")
        
        # 解析文章
        article_data = parse_markdown_file(post_file)
        if not article_data:
            continue
        
        # 发送到草稿箱
        if sender.create_draft(
            title=article_data['title'],
            content=article_data['content'],
            author=article_data['author'],
            digest=article_data['digest']
        ):
            success_count += 1
    
    print(f"\n完成！成功发送 {success_count}/{len(posts)} 篇文章到草稿箱")

if __name__ == "__main__":
    main()