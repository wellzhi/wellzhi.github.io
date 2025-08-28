import os
import re
import oss2
from PIL import Image
import hashlib
import json
from datetime import datetime
from urllib.parse import urlparse
from config import *

class ImageUploader:
    def __init__(self):
        # 初始化OSS客户端
        auth = oss2.Auth(OSS_ACCESS_KEY_ID, OSS_ACCESS_KEY_SECRET)
        self.bucket = oss2.Bucket(auth, OSS_ENDPOINT, OSS_BUCKET_NAME)
        
        # 文件映射缓存文件
        self.mapping_file = 'image_mapping.json'
        self.load_mapping()
    
    def load_mapping(self):
        """加载本地文件与OSS文件的映射关系"""
        if os.path.exists(self.mapping_file):
            with open(self.mapping_file, 'r', encoding='utf-8') as f:
                self.file_mapping = json.load(f)
        else:
            self.file_mapping = {}
    
    def save_mapping(self):
        """保存文件映射关系"""
        with open(self.mapping_file, 'w', encoding='utf-8') as f:
            json.dump(self.file_mapping, f, ensure_ascii=False, indent=2)
    
    def get_file_hash(self, file_path):
        """计算文件的MD5哈希值"""
        hash_md5 = hashlib.md5()
        with open(file_path, "rb") as f:
            for chunk in iter(lambda: f.read(4096), b""):
                hash_md5.update(chunk)
        return hash_md5.hexdigest()
    
    def check_oss_file_exists(self, oss_key):
        """检查OSS中文件是否存在"""
        try:
            return self.bucket.object_exists(oss_key)
        except Exception as e:
            print(f"检查OSS文件存在性失败: {e}")
            return False
    
    def get_pics_directory(self, markdown_file):
        """获取markdown文件对应的pics目录"""
        markdown_dir = os.path.dirname(markdown_file)
        pics_dir = os.path.join(markdown_dir, DEFAULT_PICS_DIR)
        return pics_dir if os.path.exists(pics_dir) else None
    
    def find_image_in_pics_dir(self, markdown_file, image_filename):
        """在pics目录中查找图片文件"""
        pics_dir = self.get_pics_directory(markdown_file)
        if pics_dir:
            image_path = os.path.join(pics_dir, image_filename)
            if os.path.exists(image_path):
                return image_path
        return None
    
    def should_process_file(self, markdown_file):
        """判断是否应该处理该markdown文件"""
        if not PROCESS_DRAFT_FILES:
            try:
                with open(markdown_file, 'r', encoding='utf-8') as f:
                    content = f.read()
                    # 检查是否为草稿
                    if 'draft = true' in content or 'draft: true' in content:
                        print(f"跳过草稿文件: {markdown_file}")
                        return False
            except Exception as e:
                print(f"读取文件失败: {e}")
                return False
        return True
    
    def find_existing_oss_file(self, local_file_path, file_hash):
        """查找OSS中是否已存在相同内容的文件"""
        # 1. 首先检查映射缓存
        if local_file_path in self.file_mapping:
            cached_info = self.file_mapping[local_file_path]
            if cached_info['hash'] == file_hash and self.check_oss_file_exists(cached_info['oss_key']):
                return cached_info['oss_key'], cached_info['url']
        
        # 2. 检查是否有相同哈希值的文件
        for mapped_path, info in self.file_mapping.items():
            if info['hash'] == file_hash and self.check_oss_file_exists(info['oss_key']):
                # 更新当前文件的映射
                self.file_mapping[local_file_path] = info.copy()
                self.save_mapping()
                return info['oss_key'], info['url']
        
        # 3. 尝试使用原始文件名在OSS中查找
        original_filename = os.path.basename(local_file_path)
        potential_oss_key = f"{OSS_IMAGE_PREFIX}{original_filename}"
        if self.check_oss_file_exists(potential_oss_key):
            oss_url = f"{OSS_BASE_URL}/{potential_oss_key}"
            # 更新映射
            self.file_mapping[local_file_path] = {
                'hash': file_hash,
                'oss_key': potential_oss_key,
                'url': oss_url,
                'upload_time': datetime.now().isoformat()
            }
            self.save_mapping()
            return potential_oss_key, oss_url
        
        return None, None
    
    def compress_image(self, image_path, quality=IMAGE_QUALITY, max_width=MAX_IMAGE_WIDTH):
        """压缩图片以减少文件大小"""
        try:
            with Image.open(image_path) as img:
                # 如果图片宽度超过最大宽度，则按比例缩放
                if img.width > max_width:
                    ratio = max_width / img.width
                    new_height = int(img.height * ratio)
                    img = img.resize((max_width, new_height), Image.Resampling.LANCZOS)
                
                # 保存压缩后的图片
                compressed_path = image_path.replace('.png', '_compressed.png').replace('.jpg', '_compressed.jpg').replace('.jpeg', '_compressed.jpeg')
                
                # 根据原始格式保存
                if image_path.lower().endswith('.png'):
                    img.save(compressed_path, 'PNG', optimize=True)
                else:
                    img.save(compressed_path, 'JPEG', quality=quality, optimize=True)
                
                return compressed_path
        except Exception as e:
            print(f"图片压缩失败 {image_path}: {e}")
            return image_path
    
    def generate_consistent_filename(self, original_path, file_hash):
        """生成一致的文件名，优先保持原始文件名"""
        original_filename = os.path.basename(original_path)
        
        # 如果原始文件名合理（不包含特殊字符），直接使用
        if re.match(r'^[a-zA-Z0-9._@-]+$', original_filename):
            return original_filename
        
        # 否则生成基于哈希的文件名
        ext = os.path.splitext(original_filename)[1]
        timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
        return f"{timestamp}_{file_hash[:8]}{ext}"
    
    def upload_to_oss(self, local_path, oss_key):
        """上传文件到OSS"""
        try:
            result = self.bucket.put_object_from_file(oss_key, local_path)
            if result.status == 200:
                print(f"上传成功: {oss_key}")
                return f"{OSS_BASE_URL}/{oss_key}"
            else:
                print(f"上传失败: {result.status}")
                return None
        except Exception as e:
            print(f"上传异常: {e}")
            return None
    
    def process_markdown_images(self, markdown_file):
        """处理Markdown文件中的图片"""
        print(f"开始处理文件: {markdown_file}")
        
        # 检查是否应该处理该文件
        if not self.should_process_file(markdown_file):
            return
        
        # 检查文件是否存在
        if not os.path.exists(markdown_file):
            print(f"文件不存在: {markdown_file}")
            return
        
        try:
            with open(markdown_file, 'r', encoding='utf-8') as f:
                content = f.read()
        except Exception as e:
            print(f"读取文件失败: {e}")
            return
        
        # 匹配Markdown图片语法和HTML img标签
        markdown_pattern = r'!\[([^\]]*)\]\(([^\)]+)\)'
        html_pattern = r'<img[^>]+src=["\']([^"\'>]+)["\'][^>]*>'
        
        updated_content = content
        processed_images = set()  # 避免重复处理
        images_found = 0
        images_processed = 0
        
        # 处理Markdown格式的图片
        matches = re.findall(markdown_pattern, content)
        print(f"找到 {len(matches)} 个Markdown格式的图片引用")
        
        for alt_text, image_path in matches:
            images_found += 1
            print(f"处理图片 {images_found}: {image_path}")
            
            if image_path in processed_images or image_path.startswith('http'):
                print(f"跳过图片（已处理或为网络图片）: {image_path}")
                continue
            
            processed_images.add(image_path)
            new_url = self.process_single_image(markdown_file, image_path)
            
            if new_url and new_url != image_path:
                old_image_md = f"![{alt_text}]({image_path})"
                new_image_md = f"![{alt_text}]({new_url})"
                updated_content = updated_content.replace(old_image_md, new_image_md)
                print(f"已替换: {image_path} -> {new_url}")
                images_processed += 1
            else:
                print(f"图片处理失败或无需替换: {image_path}")
        
        # 处理HTML格式的图片
        html_matches = re.findall(html_pattern, content)
        print(f"找到 {len(html_matches)} 个HTML格式的图片引用")
        
        for image_path in html_matches:
            if image_path in processed_images or image_path.startswith('http'):
                continue
            
            processed_images.add(image_path)
            new_url = self.process_single_image(markdown_file, image_path)
            
            if new_url and new_url != image_path:
                updated_content = updated_content.replace(image_path, new_url)
                print(f"已替换HTML图片: {image_path} -> {new_url}")
                images_processed += 1
        
        # 写回文件
        if updated_content != content:
            try:
                with open(markdown_file, 'w', encoding='utf-8') as f:
                    f.write(updated_content)
                print(f"文件已更新: {markdown_file}，处理了 {images_processed}/{images_found} 个图片")
            except Exception as e:
                print(f"写入文件失败: {e}")
        else:
            print(f"无需更新: {markdown_file}，共找到 {images_found} 个图片引用")
    
    def process_single_image(self, markdown_file, image_path):
        """处理单个图片"""
        print(f"  处理单个图片: {image_path}")
        
        # 处理相对路径 - 改进的路径解析逻辑
        full_path = None
        
        # 1. 如果是相对路径，先尝试在pics目录中查找
        if not image_path.startswith('http') and not os.path.isabs(image_path):
            # 移除可能的 ./ 前缀
            clean_path = image_path.lstrip('./')
            
            # 如果路径包含pics/，直接在markdown文件目录下查找
            if 'pics/' in clean_path:
                full_path = os.path.join(os.path.dirname(markdown_file), clean_path)
            else:
                # 否则在pics目录中查找
                pics_path = self.find_image_in_pics_dir(markdown_file, clean_path)
                if pics_path:
                    full_path = pics_path
                else:
                    # 如果pics目录中没有，尝试在markdown文件目录下查找
                    full_path = os.path.join(os.path.dirname(markdown_file), clean_path)
        else:
            full_path = image_path
        
        print(f"  解析后的完整路径: {full_path}")
        
        # 检查文件是否存在
        if not full_path or not os.path.exists(full_path):
            print(f"  文件不存在: {full_path}")
            return None
        
        # 检查文件格式是否支持
        file_ext = os.path.splitext(full_path)[1].lower()
        if file_ext not in SUPPORTED_FORMATS:
            print(f"  不支持的文件格式: {file_ext}")
            return None
        
        # 计算文件哈希
        try:
            file_hash = self.get_file_hash(full_path)
            print(f"  文件哈希: {file_hash[:8]}...")
        except Exception as e:
            print(f"  计算文件哈希失败: {e}")
            return None
        
        # 检查是否已存在相同文件
        existing_oss_key, existing_url = self.find_existing_oss_file(full_path, file_hash)
        if existing_oss_key:
            print(f"  文件已存在于OSS: {existing_url}")
            return existing_url
        
        # 压缩图片
        print(f"  开始压缩图片...")
        compressed_path = self.compress_image(full_path)
        
        # 生成一致的文件名
        consistent_filename = self.generate_consistent_filename(full_path, file_hash)
        oss_key = f"{OSS_IMAGE_PREFIX}{consistent_filename}"
        print(f"  OSS存储路径: {oss_key}")
        
        # 再次检查OSS中是否存在（使用一致的文件名）
        if self.check_oss_file_exists(oss_key):
            oss_url = f"{OSS_BASE_URL}/{oss_key}"
            print(f"  OSS中已存在同名文件: {oss_url}")
            
            # 更新映射
            self.file_mapping[full_path] = {
                'hash': file_hash,
                'oss_key': oss_key,
                'url': oss_url,
                'upload_time': datetime.now().isoformat()
            }
            self.save_mapping()
            
            # 清理临时文件
            if compressed_path != full_path and os.path.exists(compressed_path):
                os.remove(compressed_path)
            
            return oss_url
        
        # 上传到OSS
        print(f"  开始上传到OSS...")
        oss_url = self.upload_to_oss(compressed_path, oss_key)
        
        if oss_url:
            print(f"  上传成功: {oss_url}")
            # 更新映射
            self.file_mapping[full_path] = {
                'hash': file_hash,
                'oss_key': oss_key,
                'url': oss_url,
                'upload_time': datetime.now().isoformat()
            }
            self.save_mapping()
        else:
            print(f"  上传失败")
        
        # 清理临时文件
        if compressed_path != full_path and os.path.exists(compressed_path):
            os.remove(compressed_path)
        
        return oss_url
    
    def batch_process_directory(self, directory):
        """批量处理目录下的所有Markdown文件"""
        print(f"开始批量处理目录: {directory}")
        
        if not os.path.exists(directory):
            print(f"目录不存在: {directory}")
            return
        
        total_files = 0
        processed_files = 0
        
        for root, dirs, files in os.walk(directory):
            for file in files:
                if file.endswith('.md'):
                    total_files += 1
                    markdown_file = os.path.join(root, file)
                    print(f"\n=== 处理文件 {total_files}: {markdown_file} ===")
                    
                    try:
                        self.process_markdown_images(markdown_file)
                        processed_files += 1
                    except Exception as e:
                        print(f"处理文件时出错: {e}")
        
        print(f"\n=== 批量处理完成 ===")
        print(f"总文件数: {total_files}")
        print(f"成功处理: {processed_files}")
        print(f"映射文件已保存到: {self.mapping_file}")
    
    def clean_orphaned_mappings(self):
        """清理无效的映射记录"""
        orphaned_keys = []
        for local_path in self.file_mapping:
            if not os.path.exists(local_path):
                orphaned_keys.append(local_path)
        
        for key in orphaned_keys:
            del self.file_mapping[key]
            print(f"清理无效映射: {key}")
        
        if orphaned_keys:
            self.save_mapping()
            print(f"已清理 {len(orphaned_keys)} 个无效映射")

if __name__ == "__main__":
    uploader = ImageUploader()
    
    # 清理无效映射
    uploader.clean_orphaned_mappings()
    
    # 处理单个文件进行测试
    print("=== 测试单个文件 ===")
    test_file = './content/posts/GSK/4-AAT-judge/2025-01-01_graphic-reasoning/index.md'
    if os.path.exists(test_file):
        uploader.process_markdown_images(test_file)
    else:
        print(f"测试文件不存在: {test_file}")
    
    print("\n=== 批量处理所有文件 ===")
    # 批量处理目录
    uploader.batch_process_directory('/Users/wellzhi/Documents/code/my-github/wellzhi.github.io/content/posts')