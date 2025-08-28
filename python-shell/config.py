# OSS配置
OSS_ACCESS_KEY_ID = 'dd3bizhz9PVfj'
OSS_ACCESS_KEY_SECRET = 'KnrHRjo37TgPUpQlF'
OSS_ENDPOINT = 'https://oss-cn-guangzhou.aliyuncs.com'  # 根据实际区域修改
OSS_BUCKET_NAME = 'notespace'
OSS_BASE_URL = f'https://{OSS_BUCKET_NAME}.oss-cn-guangzhou.aliyuncs.com'  # 根据实际区域修改

# OSS存储路径前缀
OSS_IMAGE_PREFIX = 'images/'

# 图片处理配置
IMAGE_QUALITY = 85
MAX_IMAGE_WIDTH = 1200
SUPPORTED_FORMATS = ['.jpg', '.jpeg', '.png', '.webp']

# 默认图片目录名称（相对于markdown文件所在目录）
DEFAULT_PICS_DIR = 'pics'

# 是否处理草稿文件
PROCESS_DRAFT_FILES = True