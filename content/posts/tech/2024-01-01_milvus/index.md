---
date: 2024-01-01T04:14:54-08:00
draft: false
params:
  author: wellzhi
title: "Milvus使用指南"
weight: 10
hideMeta: true
tags:
  - Milvus
  - 向量数据库
  - 向量检索
  - 分布式
  - 搜索引擎

---

## 1. Milvus简介

### 什么是Milvus

Milvus是一个开源的向量数据库，专为处理大规模向量数据而设计。它支持多种向量相似性搜索算法，能够处理十亿级别的向量数据，广泛应用于AI应用场景，如推荐系统、图像检索、自然语言处理等。

### 主要特性

- **高性能**：支持十亿级向量的毫秒级检索
- **多样化索引**：支持多种向量索引算法（IVF、HNSW、ANNOY等）
- **云原生**：基于Kubernetes的分布式架构
- **多语言SDK**：支持Python、Java、Go、Node.js等
- **ACID事务**：保证数据一致性
- **混合搜索**：支持向量和标量数据的混合查询

### 应用场景

- **推荐系统**：基于用户行为向量进行个性化推荐
- **图像检索**：以图搜图、相似图片查找
- **文本搜索**：语义搜索、文档相似性匹配
- **视频分析**：视频内容检索和分析
- **药物发现**：分子结构相似性搜索
- **异常检测**：基于向量距离的异常识别

## 2. 核心概念

### 基本术语

#### Collection（集合）
类似于关系数据库中的表，用于存储向量数据和相关的标量字段。

#### Field（字段）
集合中的列，包括向量字段和标量字段。

#### Entity（实体）
集合中的一行数据，包含多个字段的值。

#### Partition（分区）
集合的子集，用于数据分片和查询优化。

#### Index（索引）
为加速向量检索而构建的数据结构。

#### Segment（段）
Milvus内部的数据存储单元，用于数据管理和查询优化。

### 数据类型

#### 向量类型
- **FloatVector**：浮点数向量
- **BinaryVector**：二进制向量

#### 标量类型
- **Bool**：布尔值
- **Int8/Int16/Int32/Int64**：整数
- **Float/Double**：浮点数
- **String/VarChar**：字符串
- **JSON**：JSON对象

## 3. 安装部署

### 系统要求

#### 硬件要求
- **CPU**：x86_64架构，支持SSE4.2指令集
- **内存**：8GB以上（推荐16GB+）
- **存储**：SSD硬盘（推荐NVMe）
- **网络**：千兆网卡

#### 软件要求
- **操作系统**：Ubuntu 18.04+、CentOS 7+、macOS 10.14+
- **Docker**：20.10+
- **Docker Compose**：1.28+

### Docker安装（推荐）

#### 1. 下载配置文件
```bash
# 下载docker-compose.yml
wget https://github.com/milvus-io/milvus/releases/download/v2.3.0/milvus-standalone-docker-compose.yml -O docker-compose.yml
```

#### 2. 启动Milvus
```bash
# 启动服务
docker-compose up -d

# 检查服务状态
docker-compose ps
```

#### 3. 验证安装
```bash
# 检查Milvus是否正常运行
curl -X GET "http://localhost:9091/health"
```

### Kubernetes部署

#### 1. 安装Helm
```bash
curl https://raw.githubusercontent.com/helm/helm/main/scripts/get-helm-3 | bash
```

#### 2. 添加Milvus Helm仓库
```bash
helm repo add milvus https://milvus-io.github.io/milvus-helm/
helm repo update
```

#### 3. 部署Milvus
```bash
# 创建命名空间
kubectl create namespace milvus

# 部署Milvus集群
helm install milvus milvus/milvus --namespace milvus
```

### 源码编译安装

#### 1. 安装依赖
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install -y build-essential cmake libopenblas-dev

# CentOS/RHEL
sudo yum groupinstall -y "Development Tools"
sudo yum install -y cmake openblas-devel
```

#### 2. 编译安装
```bash
# 克隆源码
git clone https://github.com/milvus-io/milvus.git
cd milvus

# 编译
make build

# 启动
./bin/milvus run standalone
```

## 4. 快速开始

### 安装Python SDK

```bash
pip install pymilvus
```

### 基本操作示例

#### 1. 连接Milvus
```python
from pymilvus import connections, Collection, FieldSchema, CollectionSchema, DataType

# 连接到Milvus
connections.connect(
    alias="default",
    host='localhost',
    port='19530'
)

print("Connected to Milvus")
```

#### 2. 创建集合
```python
# 定义字段
fields = [
    FieldSchema(name="id", dtype=DataType.INT64, is_primary=True, auto_id=False),
    FieldSchema(name="embedding", dtype=DataType.FLOAT_VECTOR, dim=128),
    FieldSchema(name="title", dtype=DataType.VARCHAR, max_length=200),
    FieldSchema(name="category", dtype=DataType.VARCHAR, max_length=50)
]

# 创建集合schema
schema = CollectionSchema(
    fields=fields,
    description="Document embedding collection"
)

# 创建集合
collection = Collection(
    name="documents",
    schema=schema
)

print("Collection created")
```

#### 3. 插入数据
```python
import random

# 准备数据
num_entities = 1000
entities = [
    [i for i in range(num_entities)],  # id字段
    [[random.random() for _ in range(128)] for _ in range(num_entities)],  # embedding字段
    [f"Document {i}" for i in range(num_entities)],  # title字段
    [f"Category {i % 10}" for i in range(num_entities)]  # category字段
]

# 插入数据
insert_result = collection.insert(entities)
print(f"Inserted {len(insert_result.primary_keys)} entities")

# 刷新数据到磁盘
collection.flush()
```

#### 4. 创建索引
```python
# 定义索引参数
index_params = {
    "metric_type": "L2",
    "index_type": "IVF_FLAT",
    "params": {"nlist": 128}
}

# 创建索引
collection.create_index(
    field_name="embedding",
    index_params=index_params
)

print("Index created")
```

#### 5. 加载集合
```python
# 加载集合到内存
collection.load()
print("Collection loaded")
```

#### 6. 向量检索
```python
# 准备查询向量
query_vectors = [[random.random() for _ in range(128)]]

# 执行搜索
search_params = {"metric_type": "L2", "params": {"nprobe": 10}}
results = collection.search(
    data=query_vectors,
    anns_field="embedding",
    param=search_params,
    limit=10,
    output_fields=["title", "category"]
)

# 输出结果
for hits in results:
    for hit in hits:
        print(f"ID: {hit.id}, Distance: {hit.distance}, Title: {hit.entity.get('title')}")
```

## 5. 数据管理

### 集合管理

#### 创建集合
```python
from pymilvus import Collection, FieldSchema, CollectionSchema, DataType

# 定义复杂schema
fields = [
    FieldSchema(name="id", dtype=DataType.INT64, is_primary=True),
    FieldSchema(name="vector", dtype=DataType.FLOAT_VECTOR, dim=256),
    FieldSchema(name="text", dtype=DataType.VARCHAR, max_length=1000),
    FieldSchema(name="score", dtype=DataType.FLOAT),
    FieldSchema(name="timestamp", dtype=DataType.INT64),
    FieldSchema(name="metadata", dtype=DataType.JSON)
]

schema = CollectionSchema(
    fields=fields,
    description="Advanced collection with multiple field types",
    enable_dynamic_field=True  # 启用动态字段
)

collection = Collection(name="advanced_collection", schema=schema)
```

#### 查看集合信息
```python
# 获取集合统计信息
stats = collection.get_stats()
print(f"Collection stats: {stats}")

# 获取集合schema
schema = collection.schema
for field in schema.fields:
    print(f"Field: {field.name}, Type: {field.dtype}, Params: {field.params}")

# 检查集合是否存在
from pymilvus import utility
has_collection = utility.has_collection("advanced_collection")
print(f"Collection exists: {has_collection}")
```

#### 删除集合
```python
# 删除集合
collection.drop()

# 或者使用utility函数
utility.drop_collection("collection_name")
```

### 分区管理

#### 创建分区
```python
# 创建分区
collection.create_partition("partition_2023")
collection.create_partition("partition_2024")

# 查看所有分区
partitions = collection.partitions
for partition in partitions:
    print(f"Partition: {partition.name}")
```

#### 分区数据操作
```python
# 向特定分区插入数据
entities = [
    [1, 2, 3],  # ids
    [[0.1] * 256, [0.2] * 256, [0.3] * 256],  # vectors
    ["text1", "text2", "text3"],  # text
    [0.8, 0.9, 0.7],  # scores
    [1640995200, 1640995300, 1640995400],  # timestamps
    [{"key": "value1"}, {"key": "value2"}, {"key": "value3"}]  # metadata
]

collection.insert(entities, partition_name="partition_2023")

# 在特定分区中搜索
results = collection.search(
    data=[[0.1] * 256],
    anns_field="vector",
    param={"metric_type": "L2", "params": {"nprobe": 10}},
    limit=10,
    partition_names=["partition_2023"]
)
```

### 数据插入和更新

#### 批量插入
```python
import numpy as np

# 大批量数据插入
batch_size = 10000
for i in range(0, 100000, batch_size):
    ids = list(range(i, min(i + batch_size, 100000)))
    vectors = np.random.random((len(ids), 256)).tolist()
    texts = [f"Document {j}" for j in ids]
    scores = np.random.random(len(ids)).tolist()
    timestamps = [1640995200 + j for j in ids]
    metadata = [{"batch": i // batch_size} for _ in ids]
    
    entities = [ids, vectors, texts, scores, timestamps, metadata]
    collection.insert(entities)
    
    if i % 50000 == 0:
        collection.flush()  # 定期刷新
        print(f"Inserted {i + len(ids)} entities")
```

#### 数据更新（Upsert）
```python
# Milvus 2.3+支持upsert操作
update_entities = [
    [1, 2, 3],  # 更新已存在的ID
    [[0.5] * 256, [0.6] * 256, [0.7] * 256],  # 新的向量
    ["Updated text1", "Updated text2", "Updated text3"],  # 新的文本
    [0.95, 0.96, 0.97],  # 新的分数
    [1640995500, 1640995600, 1640995700],  # 新的时间戳
    [{"updated": True}, {"updated": True}, {"updated": True}]  # 新的元数据
]

collection.upsert(update_entities)
```

### 数据删除

#### 按ID删除
```python
# 删除指定ID的实体
delete_ids = [1, 2, 3, 4, 5]
expr = f"id in {delete_ids}"
collection.delete(expr)

# 删除满足条件的实体
expr = "score < 0.5"
collection.delete(expr)
```

#### 按条件删除
```python
# 复杂删除条件
expr = "score < 0.3 and timestamp < 1640995300"
collection.delete(expr)

# 使用JSON字段删除
expr = "JSON_CONTAINS(metadata, '\"updated\": true')"
collection.delete(expr)
```

## 6. 向量检索

### 基本检索

#### 相似性搜索
```python
# 基本向量搜索
query_vectors = [[0.1] * 256, [0.2] * 256]
search_params = {
    "metric_type": "L2",
    "params": {"nprobe": 16}
}

results = collection.search(
    data=query_vectors,
    anns_field="vector",
    param=search_params,
    limit=10,
    output_fields=["text", "score", "timestamp"]
)

for i, hits in enumerate(results):
    print(f"Query {i} results:")
    for hit in hits:
        print(f"  ID: {hit.id}, Distance: {hit.distance:.4f}")
        print(f"  Text: {hit.entity.get('text')}")
        print(f"  Score: {hit.entity.get('score')}")
```

#### 混合搜索
```python
# 向量搜索 + 标量过滤
query_vectors = [[0.1] * 256]
search_params = {"metric_type": "L2", "params": {"nprobe": 16}}

# 添加标量过滤条件
filter_expr = "score > 0.8 and timestamp > 1640995200"

results = collection.search(
    data=query_vectors,
    anns_field="vector",
    param=search_params,
    limit=10,
    expr=filter_expr,
    output_fields=["text", "score", "timestamp", "metadata"]
)
```

### 高级检索

#### 范围搜索
```python
# 搜索距离在指定范围内的向量
from pymilvus import SearchResult

query_vectors = [[0.1] * 256]
search_params = {
    "metric_type": "L2",
    "params": {
        "nprobe": 16,
        "radius": 0.1,  # 最大距离
        "range_filter": 0.05  # 最小距离
    }
}

results = collection.search(
    data=query_vectors,
    anns_field="vector",
    param=search_params,
    limit=100,
    output_fields=["text", "score"]
)
```

#### 多向量搜索
```python
# 同时搜索多个向量字段（如果集合有多个向量字段）
# 假设有text_vector和image_vector两个字段

# 创建包含多个向量字段的集合
multi_vector_fields = [
    FieldSchema(name="id", dtype=DataType.INT64, is_primary=True),
    FieldSchema(name="text_vector", dtype=DataType.FLOAT_VECTOR, dim=128),
    FieldSchema(name="image_vector", dtype=DataType.FLOAT_VECTOR, dim=256),
    FieldSchema(name="title", dtype=DataType.VARCHAR, max_length=200)
]

multi_schema = CollectionSchema(fields=multi_vector_fields)
multi_collection = Collection(name="multi_vector_collection", schema=multi_schema)

# 分别在不同向量字段上搜索
text_results = multi_collection.search(
    data=[[0.1] * 128],
    anns_field="text_vector",
    param={"metric_type": "L2", "params": {"nprobe": 16}},
    limit=10
)

image_results = multi_collection.search(
    data=[[0.1] * 256],
    anns_field="image_vector",
    param={"metric_type": "L2", "params": {"nprobe": 16}},
    limit=10
)
```

### 查询操作

#### 标量查询
```python
# 基于标量字段的查询
query_expr = "score > 0.8"
results = collection.query(
    expr=query_expr,
    output_fields=["id", "text", "score", "timestamp"]
)

for result in results:
    print(f"ID: {result['id']}, Text: {result['text']}, Score: {result['score']}")
```

#### 复杂查询
```python
# 复杂查询表达式
complex_expr = """
    (score > 0.8 and timestamp > 1640995200) or 
    (score > 0.9 and JSON_CONTAINS(metadata, '"important": true'))
"""

results = collection.query(
    expr=complex_expr,
    output_fields=["*"],  # 输出所有字段
    limit=100
)
```

#### 分页查询
```python
# 分页查询大量数据
page_size = 1000
offset = 0

while True:
    results = collection.query(
        expr="score > 0.5",
        output_fields=["id", "text", "score"],
        limit=page_size,
        offset=offset
    )
    
    if not results:
        break
        
    print(f"Page {offset // page_size + 1}: {len(results)} results")
    
    # 处理结果
    for result in results:
        # 处理每个结果
        pass
    
    offset += page_size
```

## 7. 索引管理

### 索引类型

#### FLAT索引
```python
# FLAT索引 - 精确搜索，适合小数据集
flat_index = {
    "index_type": "FLAT",
    "metric_type": "L2",
    "params": {}
}

collection.create_index(
    field_name="vector",
    index_params=flat_index
)
```

#### IVF索引
```python
# IVF_FLAT索引 - 平衡性能和精度
ivf_flat_index = {
    "index_type": "IVF_FLAT",
    "metric_type": "L2",
    "params": {
        "nlist": 128  # 聚类中心数量
    }
}

# IVF_PQ索引 - 压缩存储，适合大数据集
ivf_pq_index = {
    "index_type": "IVF_PQ",
    "metric_type": "L2",
    "params": {
        "nlist": 128,
        "m": 16,  # PQ分段数
        "nbits": 8  # 每段的位数
    }
}

collection.create_index(field_name="vector", index_params=ivf_pq_index)
```

#### HNSW索引
```python
# HNSW索引 - 高性能近似搜索
hnsw_index = {
    "index_type": "HNSW",
    "metric_type": "L2",
    "params": {
        "M": 16,  # 每层的最大连接数
        "efConstruction": 200  # 构建时的搜索深度
    }
}

collection.create_index(field_name="vector", index_params=hnsw_index)
```

#### ANNOY索引
```python
# ANNOY索引 - 内存友好
annoy_index = {
    "index_type": "ANNOY",
    "metric_type": "L2",
    "params": {
        "n_trees": 8  # 树的数量
    }
}

collection.create_index(field_name="vector", index_params=annoy_index)
```

### 距离度量

#### 欧几里得距离（L2）
```python
l2_index = {
    "index_type": "IVF_FLAT",
    "metric_type": "L2",  # 欧几里得距离
    "params": {"nlist": 128}
}
```

#### 内积（IP）
```python
ip_index = {
    "index_type": "IVF_FLAT",
    "metric_type": "IP",  # 内积
    "params": {"nlist": 128}
}
```

#### 余弦相似度
```python
# 余弦相似度需要先归一化向量，然后使用IP
import numpy as np

def normalize_vectors(vectors):
    """归一化向量以使用余弦相似度"""
    vectors = np.array(vectors)
    norms = np.linalg.norm(vectors, axis=1, keepdims=True)
    return (vectors / norms).tolist()

# 插入归一化后的向量
normalized_vectors = normalize_vectors(original_vectors)
entities = [ids, normalized_vectors, texts, scores, timestamps, metadata]
collection.insert(entities)

# 使用IP度量进行余弦相似度搜索
cosine_index = {
    "index_type": "IVF_FLAT",
    "metric_type": "IP",
    "params": {"nlist": 128}
}
```

### 索引管理操作

#### 查看索引信息
```python
# 获取索引信息
index_info = collection.index()
print(f"Index type: {index_info.params['index_type']}")
print(f"Metric type: {index_info.params['metric_type']}")
print(f"Index params: {index_info.params['params']}")

# 检查索引构建进度
from pymilvus import utility
index_progress = utility.index_building_progress("collection_name")
print(f"Index building progress: {index_progress}")
```

#### 重建索引
```python
# 删除现有索引
collection.drop_index()

# 创建新索引
new_index = {
    "index_type": "HNSW",
    "metric_type": "L2",
    "params": {"M": 32, "efConstruction": 400}
}

collection.create_index(field_name="vector", index_params=new_index)

# 等待索引构建完成
import time
while True:
    progress = utility.index_building_progress(collection.name)
    if progress['pending_index_rows'] == 0:
        break
    print(f"Index building progress: {progress}")
    time.sleep(5)

print("Index building completed")
```

## 8. 性能优化

### 搜索参数优化

#### IVF索引优化
```python
# 根据数据量调整nlist
data_size = collection.num_entities
optimal_nlist = int(np.sqrt(data_size))
optimal_nlist = max(128, min(optimal_nlist, 4096))  # 限制在合理范围内

# 搜索时调整nprobe
search_params = {
    "metric_type": "L2",
    "params": {
        "nprobe": min(optimal_nlist // 4, 64)  # 通常设置为nlist的1/4
    }
}
```

#### HNSW索引优化
```python
# 构建时参数
hnsw_build_params = {
    "index_type": "HNSW",
    "metric_type": "L2",
    "params": {
        "M": 16,  # 连接数，影响精度和内存
        "efConstruction": 200  # 构建时搜索深度
    }
}

# 搜索时参数
hnsw_search_params = {
    "metric_type": "L2",
    "params": {
        "ef": 100  # 搜索时的候选数量，越大精度越高但速度越慢
    }
}
```

### 内存管理

#### 集合加载策略
```python
# 部分加载 - 只加载需要的字段
collection.load(replica_number=1, _resource_groups=["default"])

# 释放不需要的集合
collection.release()

# 检查内存使用
from pymilvus import utility
memory_info = utility.get_query_segment_info(collection.name)
for info in memory_info:
    print(f"Segment {info.segmentID}: {info.mem_size} bytes")
```

#### 分区加载
```python
# 只加载特定分区
collection.load(partition_names=["partition_2024"])

# 动态加载/释放分区
def load_partition_by_date(date_str):
    partition_name = f"partition_{date_str}"
    if partition_name in [p.name for p in collection.partitions]:
        collection.load(partition_names=[partition_name])
        return True
    return False

def release_old_partitions(keep_days=7):
    from datetime import datetime, timedelta
    cutoff_date = datetime.now() - timedelta(days=keep_days)
    
    for partition in collection.partitions:
        if partition.name.startswith("partition_"):
            date_str = partition.name.replace("partition_", "")
            try:
                partition_date = datetime.strptime(date_str, "%Y%m%d")
                if partition_date < cutoff_date:
                    collection.release(partition_names=[partition.name])
                    print(f"Released partition: {partition.name}")
            except ValueError:
                continue
```

### 批处理优化

#### 批量插入优化
```python
def optimized_batch_insert(collection, data, batch_size=10000):
    """优化的批量插入函数"""
    total_entities = len(data[0])
    
    for i in range(0, total_entities, batch_size):
        end_idx = min(i + batch_size, total_entities)
        batch_data = [field_data[i:end_idx] for field_data in data]
        
        # 插入批次
        collection.insert(batch_data)
        
        # 定期刷新
        if (i + batch_size) % 50000 == 0:
            collection.flush()
            print(f"Inserted and flushed {i + batch_size} entities")
    
    # 最终刷新
    collection.flush()
    print(f"Completed insertion of {total_entities} entities")
```

#### 并行搜索
```python
import concurrent.futures
import threading

def parallel_search(collection, query_vectors, search_params, max_workers=4):
    """并行执行多个搜索请求"""
    def search_batch(vectors_batch):
        return collection.search(
            data=vectors_batch,
            anns_field="vector",
            param=search_params,
            limit=10
        )
    
    # 将查询向量分批
    batch_size = len(query_vectors) // max_workers
    batches = [query_vectors[i:i+batch_size] 
               for i in range(0, len(query_vectors), batch_size)]
    
    # 并行执行搜索
    with concurrent.futures.ThreadPoolExecutor(max_workers=max_workers) as executor:
        future_to_batch = {executor.submit(search_batch, batch): batch 
                          for batch in batches}
        
        all_results = []
        for future in concurrent.futures.as_completed(future_to_batch):
            batch_results = future.result()
            all_results.extend(batch_results)
    
    return all_results
```

### 连接池管理

```python
from pymilvus import connections
import threading

class MilvusConnectionPool:
    def __init__(self, host='localhost', port='19530', pool_size=10):
        self.host = host
        self.port = port
        self.pool_size = pool_size
        self.connections = []
        self.lock = threading.Lock()
        self._initialize_pool()
    
    def _initialize_pool(self):
        for i in range(self.pool_size):
            alias = f"connection_{i}"
            connections.connect(
                alias=alias,
                host=self.host,
                port=self.port
            )
            self.connections.append(alias)
    
    def get_connection(self):
        with self.lock:
            if self.connections:
                return self.connections.pop()
            else:
                # 如果池为空，创建新连接
                alias = f"temp_connection_{threading.current_thread().ident}"
                connections.connect(
                    alias=alias,
                    host=self.host,
                    port=self.port
                )
                return alias
    
    def return_connection(self, alias):
        with self.lock:
            if len(self.connections) < self.pool_size:
                self.connections.append(alias)
            else:
                connections.disconnect(alias)

# 使用连接池
pool = MilvusConnectionPool()

def search_with_pool(query_vector):
    alias = pool.get_connection()
    try:
        # 使用指定连接执行搜索
        connections.connect(alias=alias)
        collection = Collection("documents", using=alias)
        results = collection.search(
            data=[query_vector],
            anns_field="vector",
            param={"metric_type": "L2", "params": {"nprobe": 16}},
            limit=10
        )
        return results
    finally:
        pool.return_connection(alias)
```

## 9. 集群部署

### Kubernetes集群部署

#### 1. 准备配置文件
```yaml
# milvus-cluster-values.yaml
cluster:
  enabled: true

image:
  all:
    repository: milvusdb/milvus
    tag: v2.3.0
    pullPolicy: IfNotPresent

service:
  type: LoadBalancer
  port: 19530
  portName: milvus
  nodePort: 30530

rootCoordinator:
  replicas: 1
  resources:
    limits:
      cpu: 1
      memory: 2Gi
    requests:
      cpu: 0.5
      memory: 1Gi

queryCoordinator:
  replicas: 1
  resources:
    limits:
      cpu: 1
      memory: 2Gi
    requests:
      cpu: 0.5
      memory: 1Gi

queryNode:
  replicas: 2
  resources:
    limits:
      cpu: 2
      memory: 8Gi
    requests:
      cpu: 1
      memory: 4Gi

indexNode:
  replicas: 1
  resources:
    limits:
      cpu: 2
      memory: 4Gi
    requests:
      cpu: 1
      memory: 2Gi

dataNode:
  replicas: 2
  resources:
    limits:
      cpu: 1
      memory: 4Gi
    requests:
      cpu: 0.5
      memory: 2Gi

proxy:
  replicas: 2
  resources:
    limits:
      cpu: 1
      memory: 2Gi
    requests:
      cpu: 0.5
      memory: 1Gi

# 存储配置
minio:
  enabled: true
  mode: distributed
  replicas: 4
  persistence:
    enabled: true
    size: 100Gi
    storageClass: "fast-ssd"

etcd:
  enabled: true
  replicaCount: 3
  persistence:
    enabled: true
    size: 10Gi
    storageClass: "fast-ssd"

pulsar:
  enabled: true
  components:
    broker: true
    bookkeeper: true
    zookeeper: true
  zookeeper:
    replicaCount: 3
  bookkeeper:
    replicaCount: 3
  broker:
    replicaCount: 2
```

#### 2. 部署集群
```bash
# 创建命名空间
kubectl create namespace milvus-cluster

# 部署Milvus集群
helm install milvus-cluster milvus/milvus \
  --namespace milvus-cluster \
  --values milvus-cluster-values.yaml

# 检查部署状态
kubectl get pods -n milvus-cluster
kubectl get services -n milvus-cluster
```

#### 3. 配置负载均衡
```yaml
# milvus-ingress.yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: milvus-ingress
  namespace: milvus-cluster
  annotations:
    nginx.ingress.kubernetes.io/backend-protocol: "GRPC"
    nginx.ingress.kubernetes.io/grpc-backend: "true"
spec:
  ingressClassName: nginx
  rules:
  - host: milvus.example.com
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: milvus-cluster
            port:
              number: 19530
```

### 高可用配置

#### 多副本配置
```python
# 连接到集群
connections.connect(
    alias="cluster",
    host='milvus.example.com',
    port='19530'
)

# 创建集合时指定副本数
collection = Collection("ha_collection", schema=schema)
collection.create_index(field_name="vector", index_params=index_params)

# 加载时指定副本数
collection.load(replica_number=2)

# 检查副本状态
from pymilvus import utility
replica_info = utility.get_replicas(collection.name)
for replica in replica_info:
    print(f"Replica {replica.id}: {replica.node_ids}")
```

#### 故障转移测试
```python
def test_failover(collection):
    """测试故障转移能力"""
    import time
    import random
    
    query_vector = [random.random() for _ in range(256)]
    
    # 持续查询测试
    success_count = 0
    total_count = 0
    
    for i in range(100):
        try:
            results = collection.search(
                data=[query_vector],
                anns_field="vector",
                param={"metric_type": "L2", "params": {"nprobe": 16}},
                limit=10
            )
            success_count += 1
            print(f"Query {i}: Success")
        except Exception as e:
            print(f"Query {i}: Failed - {e}")
        
        total_count += 1
        time.sleep(1)
    
    print(f"Success rate: {success_count/total_count*100:.2f}%")
```

### 数据分片策略

#### 基于时间的分片
```python
from datetime import datetime, timedelta

def create_time_based_partitions(collection, start_date, end_date):
    """创建基于时间的分区"""
    current_date = start_date
    
    while current_date <= end_date:
        partition_name = f"partition_{current_date.strftime('%Y%m%d')}"
        try:
            collection.create_partition(partition_name)
            print(f"Created partition: {partition_name}")
        except Exception as e:
            print(f"Partition {partition_name} already exists or error: {e}")
        
        current_date += timedelta(days=1)

def insert_with_time_partition(collection, entities, timestamp_field_idx=4):
    """根据时间戳插入到对应分区"""
    # 按时间戳分组数据
    partition_data = {}
    
    for i, timestamp in enumerate(entities[timestamp_field_idx]):
        date_str = datetime.fromtimestamp(timestamp).strftime('%Y%m%d')
        partition_name = f"partition_{date_str}"
        
        if partition_name not in partition_data:
            partition_data[partition_name] = [[] for _ in entities]
        
        for j, field_data in enumerate(entities):
            partition_data[partition_name][j].append(field_data[i])
    
    # 分别插入到各个分区
    for partition_name, partition_entities in partition_data.items():
        try:
            collection.insert(partition_entities, partition_name=partition_name)
            print(f"Inserted {len(partition_entities[0])} entities to {partition_name}")
        except Exception as e:
            print(f"Failed to insert to {partition_name}: {e}")
```

#### 基于哈希的分片
```python
import hashlib

def create_hash_based_partitions(collection, num_partitions=8):
    """创建基于哈希的分区"""
    for i in range(num_partitions):
        partition_name = f"partition_hash_{i}"
        try:
            collection.create_partition(partition_name)
            print(f"Created partition: {partition_name}")
        except Exception as e:
            print(f"Partition {partition_name} already exists or error: {e}")

def insert_with_hash_partition(collection, entities, key_field_idx=0, num_partitions=8):
    """根据键值哈希插入到对应分区"""
    partition_data = {f"partition_hash_{i}": [[] for _ in entities] 
                     for i in range(num_partitions)}
    
    for i, key in enumerate(entities[key_field_idx]):
        # 计算哈希值确定分区
        hash_value = int(hashlib.md5(str(key).encode()).hexdigest(), 16)
        partition_idx = hash_value % num_partitions
        partition_name = f"partition_hash_{partition_idx}"
        
        for j, field_data in enumerate(entities):
            partition_data[partition_name][j].append(field_data[i])
    
    # 插入到各个分区
    for partition_name, partition_entities in partition_data.items():
        if partition_entities[0]:  # 如果分区有数据
            collection.insert(partition_entities, partition_name=partition_name)
            print(f"Inserted {len(partition_entities[0])} entities to {partition_name}")
```

## 10. 监控运维

### 系统监控

#### Prometheus监控配置
```yaml
# prometheus-config.yaml
global:
  scrape_interval: 15s

scrape_configs:
  - job_name: 'milvus'
    static_configs:
      - targets: ['milvus:9091']
    metrics_path: /metrics
    scrape_interval: 15s

  - job_name: 'milvus-cluster'
    kubernetes_sd_configs:
      - role: pod
        namespaces:
          names:
            - milvus-cluster
    relabel_configs:
      - source_labels: [__meta_kubernetes_pod_label_app_kubernetes_io_name]
        action: keep
        regex: milvus
      - source_labels: [__meta_kubernetes_pod_annotation_prometheus_io_scrape]
        action: keep
        regex: true
      - source_labels: [__meta_kubernetes_pod_annotation_prometheus_io_port]
        action: replace
        target_label: __address__
        regex: (.+)
        replacement: ${1}:9091
```

#### Grafana仪表板
```json
{
  "dashboard": {
    "title": "Milvus Monitoring",
    "panels": [
      {
        "title": "QPS (Queries Per Second)",
        "type": "graph",
        "targets": [
          {
            "expr": "rate(milvus_proxy_search_vectors_count[5m])",
            "legendFormat": "Search QPS"
          },
          {
            "expr": "rate(milvus_proxy_insert_vectors_count[5m])",
            "legendFormat": "Insert QPS"
          }
        ]
      },
      {
        "title": "Response Time",
        "type": "graph",
        "targets": [
          {
            "expr": "histogram_quantile(0.95, rate(milvus_proxy_search_latency_bucket[5m]))",
            "legendFormat": "Search P95 Latency"
          },
          {
            "expr": "histogram_quantile(0.99, rate(milvus_proxy_search_latency_bucket[5m]))",
            "legendFormat": "Search P99 Latency"
          }
        ]
      },
      {
        "title": "Memory Usage",
        "type": "graph",
        "targets": [
          {
            "expr": "milvus_querynode_memory_usage_bytes",
            "legendFormat": "QueryNode Memory"
          },
          {
            "expr": "milvus_indexnode_memory_usage_bytes",
            "legendFormat": "IndexNode Memory"
          }
        ]
      },
      {
        "title": "Collection Statistics",
        "type": "table",
        "targets": [
          {
            "expr": "milvus_collection_num_entities",
            "format": "table"
          }
        ]
      }
    ]
  }
}
```

### 性能监控

#### 自定义监控脚本
```python
import time
import psutil
import threading
from pymilvus import connections, Collection, utility
from datetime import datetime

class MilvusMonitor:
    def __init__(self, collection_name, interval=60):
        self.collection_name = collection_name
        self.interval = interval
        self.running = False
        self.metrics = []
        
    def start_monitoring(self):
        self.running = True
        monitor_thread = threading.Thread(target=self._monitor_loop)
        monitor_thread.daemon = True
        monitor_thread.start()
        
    def stop_monitoring(self):
        self.running = False
        
    def _monitor_loop(self):
        while self.running:
            try:
                metrics = self._collect_metrics()
                self.metrics.append(metrics)
                print(f"[{metrics['timestamp']}] {metrics}")
                
                # 保留最近1000条记录
                if len(self.metrics) > 1000:
                    self.metrics = self.metrics[-1000:]
                    
            except Exception as e:
                print(f"Monitoring error: {e}")
                
            time.sleep(self.interval)
            
    def _collect_metrics(self):
        collection = Collection(self.collection_name)
        
        # 集合统计信息
        stats = collection.get_stats()
        num_entities = int(stats['row_count'])
        
        # 系统资源
        cpu_percent = psutil.cpu_percent()
        memory = psutil.virtual_memory()
        disk = psutil.disk_usage('/')
        
        # 查询性能测试
        start_time = time.time()
        try:
            test_vector = [0.1] * 256
            collection.search(
                data=[test_vector],
                anns_field="vector",
                param={"metric_type": "L2", "params": {"nprobe": 16}},
                limit=10
            )
            query_latency = (time.time() - start_time) * 1000  # ms
        except Exception as e:
            query_latency = -1
            
        return {
            'timestamp': datetime.now().isoformat(),
            'collection_entities': num_entities,
            'cpu_percent': cpu_percent,
            'memory_percent': memory.percent,
            'memory_used_gb': memory.used / (1024**3),
            'disk_percent': disk.percent,
            'disk_used_gb': disk.used / (1024**3),
            'query_latency_ms': query_latency
        }
        
    def get_metrics_summary(self, last_n=100):
        """获取最近N条记录的统计摘要"""
        if not self.metrics:
            return None
            
        recent_metrics = self.metrics[-last_n:]
        
        latencies = [m['query_latency_ms'] for m in recent_metrics if m['query_latency_ms'] > 0]
        cpu_usage = [m['cpu_percent'] for m in recent_metrics]
        memory_usage = [m['memory_percent'] for m in recent_metrics]
        
        return {
            'avg_query_latency_ms': sum(latencies) / len(latencies) if latencies else 0,
            'max_query_latency_ms': max(latencies) if latencies else 0,
            'avg_cpu_percent': sum(cpu_usage) / len(cpu_usage),
            'max_cpu_percent': max(cpu_usage),
            'avg_memory_percent': sum(memory_usage) / len(memory_usage),
            'max_memory_percent': max(memory_usage),
            'total_entities': recent_metrics[-1]['collection_entities'] if recent_metrics else 0
        }

# 使用监控器
monitor = MilvusMonitor("documents", interval=30)
monitor.start_monitoring()

# 运行一段时间后查看摘要
time.sleep(300)  # 5分钟
summary = monitor.get_metrics_summary()
print(f"Performance Summary: {summary}")

monitor.stop_monitoring()
```

### 日志管理

#### 日志配置
```yaml
# milvus-log-config.yaml
log:
  level: info
  file:
    rootPath: "/var/log/milvus"
    maxSize: 100  # MB
    maxAge: 7     # days
    maxBackups: 10
  format: json
  
# 在Kubernetes中配置日志收集
apiVersion: v1
kind: ConfigMap
metadata:
  name: fluent-bit-config
  namespace: milvus-cluster
data:
  fluent-bit.conf: |
    [SERVICE]
        Flush         1
        Log_Level     info
        Daemon        off
        Parsers_File  parsers.conf

    [INPUT]
        Name              tail
        Path              /var/log/milvus/*.log
        Parser            json
        Tag               milvus.*
        Refresh_Interval  5

    [OUTPUT]
        Name  es
        Match milvus.*
        Host  elasticsearch.logging.svc.cluster.local
        Port  9200
        Index milvus-logs
        Type  _doc
```

#### 日志分析脚本
```python
import json
import re
from datetime import datetime, timedelta
from collections import defaultdict

def analyze_milvus_logs(log_file_path, hours=24):
    """分析Milvus日志文件"""
    cutoff_time = datetime.now() - timedelta(hours=hours)
    
    error_counts = defaultdict(int)
    warning_counts = defaultdict(int)
    performance_metrics = []
    
    with open(log_file_path, 'r') as f:
        for line in f:
            try:
                log_entry = json.loads(line.strip())
                log_time = datetime.fromisoformat(log_entry.get('time', '').replace('Z', '+00:00'))
                
                if log_time < cutoff_time:
                    continue
                    
                level = log_entry.get('level', '').upper()
                message = log_entry.get('msg', '')
                
                # 统计错误和警告
                if level == 'ERROR':
                    error_counts[message] += 1
                elif level == 'WARN':
                    warning_counts[message] += 1
                    
                # 提取性能指标
                if 'latency' in message.lower():
                    latency_match = re.search(r'latency[:\s]+(\d+(?:\.\d+)?)\s*(ms|μs)', message)
                    if latency_match:
                        latency_value = float(latency_match.group(1))
                        latency_unit = latency_match.group(2)
                        
                        if latency_unit == 'μs':
                            latency_value /= 1000  # 转换为ms
                            
                        performance_metrics.append({
                            'timestamp': log_time,
                            'latency_ms': latency_value,
                            'operation': extract_operation(message)
                        })
                        
            except (json.JSONDecodeError, ValueError) as e:
                continue
                
    return {
        'error_summary': dict(error_counts),
        'warning_summary': dict(warning_counts),
        'performance_metrics': performance_metrics
    }

def extract_operation(message):
    """从日志消息中提取操作类型"""
    if 'search' in message.lower():
        return 'search'
    elif 'insert' in message.lower():
        return 'insert'
    elif 'index' in message.lower():
        return 'index'
    else:
        return 'unknown'

def generate_log_report(analysis_result):
    """生成日志分析报告"""
    print("=== Milvus Log Analysis Report ===")
    print(f"Analysis time: {datetime.now()}")
    print()
    
    # 错误摘要
    print("Top Errors:")
    sorted_errors = sorted(analysis_result['error_summary'].items(), 
                          key=lambda x: x[1], reverse=True)
    for error, count in sorted_errors[:10]:
        print(f"  {count:4d} - {error[:100]}...")
    print()
    
    # 警告摘要
    print("Top Warnings:")
    sorted_warnings = sorted(analysis_result['warning_summary'].items(), 
                           key=lambda x: x[1], reverse=True)
    for warning, count in sorted_warnings[:10]:
        print(f"  {count:4d} - {warning[:100]}...")
    print()
    
    # 性能摘要
    metrics = analysis_result['performance_metrics']
    if metrics:
        latencies = [m['latency_ms'] for m in metrics]
        print("Performance Summary:")
        print(f"  Total operations: {len(metrics)}")
        print(f"  Average latency: {sum(latencies)/len(latencies):.2f} ms")
        print(f"  Max latency: {max(latencies):.2f} ms")
        print(f"  Min latency: {min(latencies):.2f} ms")
        
        # 按操作类型分组
        by_operation = defaultdict(list)
        for metric in metrics:
            by_operation[metric['operation']].append(metric['latency_ms'])
            
        print("\nPerformance by Operation:")
        for operation, latencies in by_operation.items():
            if latencies:
                avg_latency = sum(latencies) / len(latencies)
                print(f"  {operation.capitalize()}:")
                print(f"    Count: {len(latencies)}")
                print(f"    Avg latency: {avg_latency:.2f} ms")
                print(f"    Max latency: {max(latencies):.2f} ms")
    else:
        print("No performance metrics found.")

# 使用示例
if __name__ == "__main__":
    log_file = "/path/to/milvus.log"
    result = analyze_milvus_logs(log_file, hours=24)
    generate_log_report(result)
```

## 11. 最佳实践

### 1. 数据建模最佳实践

#### Collection设计原则
```python
# 良好的Collection设计示例
def create_production_collection(name, vector_dim, expected_size):
    """生产环境Collection设计"""
    # 根据数据规模选择分片数
    shard_num = min(max(expected_size // 1000000, 2), 16)
    
    fields = [
        # 主键字段 - 使用有意义的ID
        FieldSchema(
            name="id", 
            dtype=DataType.INT64, 
            is_primary=True, 
            auto_id=False,
            description="Document unique identifier"
        ),
        # 时间戳字段 - 便于数据管理
        FieldSchema(
            name="created_at", 
            dtype=DataType.INT64,
            description="Creation timestamp"
        ),
        # 分类字段 - 用于过滤
        FieldSchema(
            name="category", 
            dtype=DataType.VARCHAR, 
            max_length=50,
            description="Document category"
        ),
        # 向量字段 - 核心搜索字段
        FieldSchema(
            name="embedding", 
            dtype=DataType.FLOAT_VECTOR, 
            dim=vector_dim,
            description="Document embedding vector"
        ),
        # 元数据字段 - 存储额外信息
        FieldSchema(
            name="metadata", 
            dtype=DataType.JSON,
            description="Additional metadata"
        )
    ]
    
    schema = CollectionSchema(
        fields=fields,
        description=f"Production collection for {expected_size} documents",
        enable_dynamic_field=True  # 允许动态字段
    )
    
    collection = Collection(
        name=name,
        schema=schema,
        shards_num=shard_num,
        consistency_level="Strong"  # 生产环境建议强一致性
    )
    
    return collection
```

#### 分区策略
```python
# 基于时间的分区策略
def create_time_based_partitions(collection, start_date, end_date):
    """创建基于时间的分区"""
    from datetime import datetime, timedelta
    
    current_date = start_date
    while current_date <= end_date:
        partition_name = f"partition_{current_date.strftime('%Y%m%d')}"
        try:
            collection.create_partition(partition_name)
            print(f"Created partition: {partition_name}")
        except Exception as e:
            print(f"Partition {partition_name} already exists or error: {e}")
        current_date += timedelta(days=1)

# 基于类别的分区策略
def create_category_partitions(collection, categories):
    """创建基于类别的分区"""
    for category in categories:
        partition_name = f"category_{category.lower()}"
        try:
            collection.create_partition(partition_name)
            print(f"Created partition: {partition_name}")
        except Exception as e:
            print(f"Partition {partition_name} already exists or error: {e}")
```

### 2. 性能优化最佳实践

#### 索引选择策略
```python
def choose_optimal_index(data_size, memory_budget, latency_requirement, accuracy_requirement):
    """根据需求选择最优索引"""
    
    if data_size < 100000:
        # 小数据集使用FLAT
        return {
            "index_type": "FLAT",
            "metric_type": "L2",
            "params": {}
        }
    
    elif latency_requirement == "ultra_low" and memory_budget == "high":
        # 超低延迟需求使用HNSW
        return {
            "index_type": "HNSW",
            "metric_type": "L2",
            "params": {
                "M": 32,
                "efConstruction": 400
            }
        }
    
    elif memory_budget == "low":
        # 内存受限使用PQ压缩
        return {
            "index_type": "IVF_PQ",
            "metric_type": "L2",
            "params": {
                "nlist": min(4 * int(np.sqrt(data_size)), 4096),
                "m": 16,
                "nbits": 8
            }
        }
    
    else:
        # 平衡选择IVF_FLAT
        return {
            "index_type": "IVF_FLAT",
            "metric_type": "L2",
            "params": {
                "nlist": min(4 * int(np.sqrt(data_size)), 4096)
            }
        }

# 动态调整搜索参数
def get_adaptive_search_params(index_type, accuracy_level="medium", data_size=None):
    """根据索引类型和精度要求动态调整搜索参数"""
    
    if index_type == "IVF_FLAT" or index_type == "IVF_PQ":
        nprobe_map = {
            "low": max(8, int(np.sqrt(data_size)) // 100) if data_size else 8,
            "medium": max(16, int(np.sqrt(data_size)) // 50) if data_size else 16,
            "high": max(32, int(np.sqrt(data_size)) // 25) if data_size else 32
        }
        return {"nprobe": nprobe_map[accuracy_level]}
    
    elif index_type == "HNSW":
        ef_map = {"low": 64, "medium": 128, "high": 256}
        return {"ef": ef_map[accuracy_level]}
    
    elif index_type == "ANNOY":
        search_k_map = {"low": 100, "medium": 200, "high": 400}
        return {"search_k": search_k_map[accuracy_level]}
    
    return {}
```

#### 批处理优化
```python
class OptimizedBatchProcessor:
    """优化的批处理器"""
    
    def __init__(self, collection, batch_size=10000, flush_interval=50000):
        self.collection = collection
        self.batch_size = batch_size
        self.flush_interval = flush_interval
        self.total_inserted = 0
    
    def insert_batch(self, data):
        """批量插入数据"""
        total_entities = len(data[0])
        
        for i in range(0, total_entities, self.batch_size):
            end_idx = min(i + self.batch_size, total_entities)
            batch_data = [field_data[i:end_idx] for field_data in data]
            
            try:
                result = self.collection.insert(batch_data)
                self.total_inserted += len(result.primary_keys)
                
                # 定期刷新
                if self.total_inserted % self.flush_interval == 0:
                    self.collection.flush()
                    print(f"Flushed after inserting {self.total_inserted} entities")
                    
            except Exception as e:
                print(f"Error inserting batch {i//self.batch_size}: {e}")
                continue
        
        # 最终刷新
        self.collection.flush()
        return self.total_inserted
    
    def parallel_search(self, query_vectors, search_params, max_workers=4):
        """并行搜索"""
        import concurrent.futures
        
        def search_single(query_vector):
            return self.collection.search(
                data=[query_vector],
                anns_field="embedding",
                param=search_params,
                limit=10
            )
        
        with concurrent.futures.ThreadPoolExecutor(max_workers=max_workers) as executor:
            futures = [executor.submit(search_single, qv) for qv in query_vectors]
            results = [future.result() for future in concurrent.futures.as_completed(futures)]
        
        return results
```

### 3. 数据质量最佳实践

#### 向量预处理
```python
import numpy as np
from sklearn.preprocessing import normalize

def preprocess_vectors(vectors, normalization="l2", dimension_check=True):
    """向量预处理管道"""
    vectors = np.array(vectors, dtype=np.float32)
    
    # 维度检查
    if dimension_check and len(vectors.shape) != 2:
        raise ValueError(f"Expected 2D array, got {len(vectors.shape)}D")
    
    # 检查NaN和无穷值
    if np.any(np.isnan(vectors)) or np.any(np.isinf(vectors)):
        print("Warning: Found NaN or infinite values, replacing with zeros")
        vectors = np.nan_to_num(vectors, nan=0.0, posinf=0.0, neginf=0.0)
    
    # 归一化
    if normalization == "l2":
        vectors = normalize(vectors, norm='l2', axis=1)
    elif normalization == "minmax":
        from sklearn.preprocessing import MinMaxScaler
        scaler = MinMaxScaler()
        vectors = scaler.fit_transform(vectors)
    
    return vectors.tolist()

# 数据验证
def validate_data_quality(data, schema):
    """验证数据质量"""
    issues = []
    
    # 检查数据长度一致性
    field_lengths = [len(field_data) for field_data in data]
    if len(set(field_lengths)) > 1:
        issues.append(f"Inconsistent field lengths: {field_lengths}")
    
    # 检查向量维度
    for i, field in enumerate(schema.fields):
        if field.dtype == DataType.FLOAT_VECTOR:
            vectors = data[i]
            expected_dim = field.params.get('dim')
            for j, vector in enumerate(vectors[:100]):  # 检查前100个
                if len(vector) != expected_dim:
                    issues.append(f"Vector {j} has dimension {len(vector)}, expected {expected_dim}")
                    break
    
    return issues
```

### 4. 监控和运维最佳实践

#### 健康检查系统
```python
import time
import logging
from datetime import datetime

class MilvusHealthMonitor:
    """Milvus健康监控系统"""
    
    def __init__(self, collection_names, alert_thresholds=None):
        self.collection_names = collection_names
        self.alert_thresholds = alert_thresholds or {
            'query_latency': 1.0,  # 秒
            'memory_usage': 0.8,   # 80%
            'error_rate': 0.05     # 5%
        }
        self.logger = self._setup_logger()
    
    def _setup_logger(self):
        logger = logging.getLogger('milvus_monitor')
        logger.setLevel(logging.INFO)
        handler = logging.FileHandler('milvus_health.log')
        formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
        handler.setFormatter(formatter)
        logger.addHandler(handler)
        return logger
    
    def check_connection_health(self):
        """检查连接健康状态"""
        try:
            from pymilvus import utility
            version = utility.get_server_version()
            self.logger.info(f"Milvus server version: {version}")
            return True
        except Exception as e:
            self.logger.error(f"Connection health check failed: {e}")
            return False
    
    def check_collection_health(self, collection_name):
        """检查集合健康状态"""
        try:
            collection = Collection(collection_name)
            
            # 检查集合状态
            stats = collection.get_stats()
            num_entities = collection.num_entities
            
            # 执行测试查询
            start_time = time.time()
            test_vector = [[0.1] * 128]  # 假设128维向量
            results = collection.search(
                data=test_vector,
                anns_field="embedding",
                param={"metric_type": "L2", "params": {"nprobe": 10}},
                limit=1
            )
            query_latency = time.time() - start_time
            
            # 记录指标
            metrics = {
                'collection': collection_name,
                'num_entities': num_entities,
                'query_latency': query_latency,
                'timestamp': datetime.now().isoformat()
            }
            
            self.logger.info(f"Collection health: {metrics}")
            
            # 检查告警阈值
            if query_latency > self.alert_thresholds['query_latency']:
                self.logger.warning(f"High query latency: {query_latency:.3f}s")
            
            return metrics
            
        except Exception as e:
            self.logger.error(f"Collection health check failed for {collection_name}: {e}")
            return None
    
    def run_continuous_monitoring(self, interval=60):
        """持续监控"""
        while True:
            try:
                # 检查连接
                if not self.check_connection_health():
                    self.logger.critical("Milvus connection lost!")
                
                # 检查所有集合
                for collection_name in self.collection_names:
                    self.check_collection_health(collection_name)
                
                time.sleep(interval)
                
            except KeyboardInterrupt:
                self.logger.info("Monitoring stopped by user")
                break
            except Exception as e:
                self.logger.error(f"Monitoring error: {e}")
                time.sleep(interval)
```

#### 备份和恢复策略
```python
import json
import os
from datetime import datetime

class MilvusBackupManager:
    """Milvus备份管理器"""
    
    def __init__(self, backup_dir="./milvus_backups"):
        self.backup_dir = backup_dir
        os.makedirs(backup_dir, exist_ok=True)
    
    def backup_collection_metadata(self, collection_name):
        """备份集合元数据"""
        try:
            collection = Collection(collection_name)
            
            # 收集元数据
            metadata = {
                'name': collection.name,
                'description': collection.description,
                'schema': {
                    'fields': [
                        {
                            'name': field.name,
                            'dtype': str(field.dtype),
                            'params': field.params,
                            'is_primary': field.is_primary,
                            'auto_id': field.auto_id
                        }
                        for field in collection.schema.fields
                    ],
                    'enable_dynamic_field': collection.schema.enable_dynamic_field
                },
                'num_entities': collection.num_entities,
                'partitions': [p.name for p in collection.partitions],
                'indexes': []
            }
            
            # 获取索引信息
            try:
                index_info = collection.index()
                if index_info:
                    metadata['indexes'].append({
                        'field_name': 'embedding',  # 假设向量字段名
                        'index_params': index_info.params
                    })
            except:
                pass
            
            # 保存元数据
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            backup_file = f"{self.backup_dir}/{collection_name}_metadata_{timestamp}.json"
            
            with open(backup_file, 'w') as f:
                json.dump(metadata, f, indent=2)
            
            print(f"Metadata backup saved: {backup_file}")
            return backup_file
            
        except Exception as e:
            print(f"Backup failed: {e}")
            return None
    
    def restore_collection_from_metadata(self, backup_file):
        """从元数据恢复集合结构"""
        try:
            with open(backup_file, 'r') as f:
                metadata = json.load(f)
            
            # 重建字段
            fields = []
            for field_info in metadata['schema']['fields']:
                field = FieldSchema(
                    name=field_info['name'],
                    dtype=getattr(DataType, field_info['dtype'].split('.')[-1]),
                    is_primary=field_info.get('is_primary', False),
                    auto_id=field_info.get('auto_id', False),
                    **field_info.get('params', {})
                )
                fields.append(field)
            
            # 重建schema
            schema = CollectionSchema(
                fields=fields,
                description=metadata['description'],
                enable_dynamic_field=metadata['schema']['enable_dynamic_field']
            )
            
            # 创建集合
            collection = Collection(
                name=metadata['name'],
                schema=schema
            )
            
            # 重建分区
            for partition_name in metadata['partitions']:
                if partition_name != '_default':
                    collection.create_partition(partition_name)
            
            print(f"Collection {metadata['name']} restored from backup")
            return collection
            
        except Exception as e:
            print(f"Restore failed: {e}")
            return None
```

### 5. 安全最佳实践

#### 访问控制
```python
from pymilvus import connections

# 安全连接配置
def secure_connect(host, port, username, password, secure=True):
    """安全连接到Milvus"""
    try:
        connections.connect(
            alias="secure_connection",
            host=host,
            port=port,
            user=username,
            password=password,
            secure=secure,
            server_pem_path="/path/to/server.pem",  # TLS证书路径
            server_name="milvus-server",
            timeout=30
        )
        print("Secure connection established")
        return True
    except Exception as e:
        print(f"Secure connection failed: {e}")
        return False

# 输入验证
def validate_search_input(query_vectors, limit, expr=None):
    """验证搜索输入"""
    # 验证向量
    if not isinstance(query_vectors, list) or not query_vectors:
        raise ValueError("Query vectors must be a non-empty list")
    
    # 验证limit
    if not isinstance(limit, int) or limit <= 0 or limit > 10000:
        raise ValueError("Limit must be a positive integer <= 10000")
    
    # 验证表达式（防止注入）
    if expr:
        dangerous_keywords = ['DROP', 'DELETE', 'UPDATE', 'INSERT', 'CREATE']
        expr_upper = expr.upper()
        for keyword in dangerous_keywords:
            if keyword in expr_upper:
                raise ValueError(f"Dangerous keyword '{keyword}' found in expression")
    
    return True
```

## 12. 常见问题

### Q1: 如何选择合适的索引类型？

**A**: 索引选择主要考虑以下因素：

- **数据规模**：
  - < 10万条：FLAT索引（精确搜索）
  - 10万-100万条：IVF_FLAT索引
  - 大于100万条：HNSW或IVF_PQ索引

- **内存预算**：
  - 内存充足：HNSW索引（最高性能）
  - 内存有限：IVF_PQ索引（压缩存储）
  - 极度受限：ANNOY索引

- **精度要求**：
  - 需要精确结果：FLAT索引
  - 高精度近似：HNSW索引
  - 平衡精度性能：IVF_FLAT索引

- **查询模式**：
  - 频繁查询：HNSW索引
  - 批量查询：IVF系列索引
  - 静态数据：ANNOY索引

### Q2: 搜索性能慢怎么优化？

**A**: 性能优化策略：

1. **索引优化**：
   ```python
   # 调整索引参数
   # IVF索引：增加nlist，减少nprobe
   # HNSW索引：增加M值，调整ef参数
   ```

2. **查询优化**：
   ```python
   # 使用分区过滤
   results = collection.search(
       data=query_vectors,
       anns_field="embedding",
       param=search_params,
       limit=10,
       partition_names=["recent_partition"]  # 只搜索相关分区
   )
   ```

3. **硬件优化**：
   - 使用SSD存储
   - 增加内存容量
   - 使用GPU加速（如果支持）

4. **并发优化**：
   ```python
   # 并行搜索多个查询
   import concurrent.futures
   
   def parallel_search(queries):
       with concurrent.futures.ThreadPoolExecutor(max_workers=4) as executor:
           futures = [executor.submit(collection.search, [q], "embedding", search_params, 10) 
                     for q in queries]
           return [f.result() for f in futures]
   ```

### Q3: 内存使用过高怎么处理？

**A**: 内存优化方法：

1. **使用压缩索引**：
   ```python
   # 使用PQ压缩
   pq_index = {
       "index_type": "IVF_PQ",
       "metric_type": "L2",
       "params": {"nlist": 128, "m": 16, "nbits": 8}
   }
   ```

2. **分区管理**：
   ```python
   # 只加载需要的分区
   collection.load(partition_names=["active_partition"])
   
   # 释放不用的分区
   collection.release(partition_names=["old_partition"])
   ```

3. **配置调整**：
   ```yaml
   # 在milvus.yaml中调整缓存大小
   queryCoord:
     cache:
       size: 2GB  # 减少缓存大小
   ```

### Q4: 数据一致性问题如何解决？

**A**: 确保数据一致性：

1. **设置一致性级别**：
   ```python
   # 创建集合时设置强一致性
   collection = Collection(
       name="consistent_collection",
       schema=schema,
       consistency_level="Strong"
   )
   ```

2. **及时刷新数据**：
   ```python
   # 插入后立即刷新
   collection.insert(entities)
   collection.flush()
   
   # 等待刷新完成
   import time
   time.sleep(1)
   ```

3. **检查数据状态**：
   ```python
   # 检查索引构建状态
   from pymilvus import utility
   progress = utility.index_building_progress(collection.name)
   print(f"Index progress: {progress}")
   ```

### Q5: 集群部署常见问题

**A**: 集群部署注意事项：

1. **资源规划**：
   ```yaml
   # Kubernetes资源配置示例
   resources:
     requests:
       memory: "8Gi"
       cpu: "4"
     limits:
       memory: "16Gi"
       cpu: "8"
   ```

2. **网络配置**：
   ```yaml
   # 确保节点间网络畅通
   service:
     type: ClusterIP
     ports:
       - port: 19530
         targetPort: 19530
   ```

3. **存储配置**：
   ```yaml
   # 使用持久化存储
   persistence:
     enabled: true
     storageClass: "fast-ssd"
     size: 100Gi
   ```

### Q6: 向量维度不匹配错误

**A**: 解决维度不匹配：

1. **检查向量维度**：
   ```python
   def validate_vector_dimension(vectors, expected_dim):
       for i, vector in enumerate(vectors):
           if len(vector) != expected_dim:
               raise ValueError(f"Vector {i} has dimension {len(vector)}, expected {expected_dim}")
   ```

2. **统一向量维度**：
   ```python
   def normalize_vector_dimension(vectors, target_dim):
       normalized = []
       for vector in vectors:
           if len(vector) < target_dim:
               # 填充零值
               vector.extend([0.0] * (target_dim - len(vector)))
           elif len(vector) > target_dim:
               # 截断
               vector = vector[:target_dim]
           normalized.append(vector)
       return normalized
   ```

### Q7: 连接超时和网络问题

**A**: 网络问题排查：

1. **增加超时时间**：
   ```python
   connections.connect(
       alias="default",
       host="milvus-server",
       port="19530",
       timeout=60  # 增加超时时间
   )
   ```

2. **连接池配置**：
   ```python
   # 配置连接池
   connections.configure(
       alias="production",
       host="milvus-server",
       port="19530",
       pool_size=10,
       timeout=30
   )
   ```

3. **健康检查**：
   ```python
   def check_connection():
       try:
           from pymilvus import utility
           utility.get_server_version()
           return True
       except Exception as e:
           print(f"Connection check failed: {e}")
           return False
   ```

### Q8: 搜索结果为空

**A**: 排查搜索结果为空：

1. **检查数据是否加载**：
   ```python
   # 确保集合已加载
   collection.load()
   
   # 检查加载状态
   print(f"Collection loaded: {collection.has_index()}")
   print(f"Number of entities: {collection.num_entities}")
   ```

2. **检查搜索参数**：
   ```python
   # 放宽搜索条件
   results = collection.search(
       data=query_vectors,
       anns_field="embedding",
       param={"metric_type": "L2", "params": {"nprobe": 128}},  # 增加nprobe
       limit=100,  # 增加返回数量
       expr=None  # 移除过滤条件
   )
   ```

3. **验证查询向量**：
   ```python
   # 检查查询向量是否有效
   def validate_query_vector(vector, collection_schema):
       vector_field = None
       for field in collection_schema.fields:
           if field.dtype == DataType.FLOAT_VECTOR:
               vector_field = field
               break
       
       if vector_field and len(vector) != vector_field.params['dim']:
           raise ValueError(f"Query vector dimension mismatch")
       
       if all(v == 0 for v in vector):
           print("Warning: Query vector is all zeros")
   ```

### 进一步学习资源

- [Milvus官方文档](https://milvus.io/docs)
- [Milvus GitHub仓库](https://github.com/milvus-io/milvus)
- [Milvus社区论坛](https://discuss.milvus.io/)
- [向量数据库最佳实践](https://milvus.io/blog)

