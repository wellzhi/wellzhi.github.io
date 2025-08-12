+++
date = '2013-08-05T00:00:00+08:00'
draft = false
title = 'Elasticsearch常用命令与实战教程'
comments = false
tags = ["Elasticsearch", "数据库", "Docker", "命令", "搜索引擎", "分布式"]
tocOpen = true
summary = "Elasticsearch 是一个开源的分布式搜索和分析引擎，它被广泛用于构建实时搜索应用程序。本文详细介绍了Elasticsearch的常用命令，包括索引操作、文档操作、查询操作、聚合操作以及Docker容器管理等实用技巧。"
+++

## 1. 简介

Elasticsearch是一个基于Apache Lucene的开源分布式搜索和分析引擎，它提供了一个分布式多用户能力的全文搜索引擎，基于RESTful web接口。Elasticsearch是用Java开发的，并作为Apache许可条款下的开放源码发布，是当前流行的企业级搜索引擎。

### 1.1 核心概念

- **索引(Index)**: 类似于关系数据库中的数据库
- **类型(Type)**: 类似于关系数据库中的表（ES 7.x后已废弃）
- **文档(Document)**: 类似于关系数据库中的行
- **字段(Field)**: 类似于关系数据库中的列
- **映射(Mapping)**: 类似于关系数据库中的表结构
- **分片(Shard)**: 索引的物理分割
- **副本(Replica)**: 分片的备份

## 2. 连接和基本操作

### 2.1 连接Elasticsearch

```bash
# 检查集群健康状态
curl -X GET "localhost:9200/_cluster/health?pretty"

# 查看集群信息
curl -X GET "localhost:9200/"

# 查看节点信息
curl -X GET "localhost:9200/_nodes?pretty"
```

### 2.2 基本配置查看

```bash
# 查看集群设置
curl -X GET "localhost:9200/_cluster/settings?pretty"

# 查看所有索引
curl -X GET "localhost:9200/_cat/indices?v"

# 查看分片信息
curl -X GET "localhost:9200/_cat/shards?v"
```

## 3. 索引操作

### 3.1 创建索引

```bash
# 创建简单索引
curl -X PUT "localhost:9200/my_index"

# 创建带设置的索引
curl -X PUT "localhost:9200/my_index" -H 'Content-Type: application/json' -d'
{
  "settings": {
    "number_of_shards": 3,
    "number_of_replicas": 1
  }
}'

# 创建带映射的索引
curl -X PUT "localhost:9200/products" -H 'Content-Type: application/json' -d'
{
  "settings": {
    "number_of_shards": 1,
    "number_of_replicas": 0
  },
  "mappings": {
    "properties": {
      "name": {
        "type": "text",
        "analyzer": "standard"
      },
      "price": {
        "type": "double"
      },
      "description": {
        "type": "text"
      },
      "category": {
        "type": "keyword"
      },
      "created_at": {
        "type": "date"
      }
    }
  }
}'
```

### 3.2 查看索引信息

```bash
# 查看索引映射
curl -X GET "localhost:9200/products/_mapping?pretty"

# 查看索引设置
curl -X GET "localhost:9200/products/_settings?pretty"

# 查看索引统计信息
curl -X GET "localhost:9200/products/_stats?pretty"
```

### 3.3 修改索引

```bash
# 更新索引设置
curl -X PUT "localhost:9200/products/_settings" -H 'Content-Type: application/json' -d'
{
  "number_of_replicas": 1
}'

# 添加字段映射
curl -X PUT "localhost:9200/products/_mapping" -H 'Content-Type: application/json' -d'
{
  "properties": {
    "tags": {
      "type": "keyword"
    }
  }
}'
```

### 3.4 删除索引

```bash
# 删除单个索引
curl -X DELETE "localhost:9200/my_index"

# 删除多个索引
curl -X DELETE "localhost:9200/index1,index2"

# 使用通配符删除索引
curl -X DELETE "localhost:9200/log-*"
```

## 4. 文档操作

### 4.1 添加文档

```bash
# 指定ID添加文档
curl -X PUT "localhost:9200/products/_doc/1" -H 'Content-Type: application/json' -d'
{
  "name": "iPhone 13",
  "price": 999.99,
  "description": "Latest iPhone model",
  "category": "electronics",
  "created_at": "2023-01-01T00:00:00Z"
}'

# 自动生成ID添加文档
curl -X POST "localhost:9200/products/_doc" -H 'Content-Type: application/json' -d'
{
  "name": "Samsung Galaxy S21",
  "price": 799.99,
  "description": "Android smartphone",
  "category": "electronics",
  "created_at": "2023-01-02T00:00:00Z"
}'

# 批量添加文档
curl -X POST "localhost:9200/_bulk" -H 'Content-Type: application/json' -d'
{"index":{"_index":"products","_id":"3"}}
{"name":"MacBook Pro","price":1999.99,"description":"Professional laptop","category":"computers","created_at":"2023-01-03T00:00:00Z"}
{"index":{"_index":"products","_id":"4"}}
{"name":"iPad Air","price":599.99,"description":"Tablet device","category":"electronics","created_at":"2023-01-04T00:00:00Z"}
'
```

### 4.2 查询文档

```bash
# 根据ID查询文档
curl -X GET "localhost:9200/products/_doc/1?pretty"

# 查询所有文档
curl -X GET "localhost:9200/products/_search?pretty"

# 简单查询
curl -X GET "localhost:9200/products/_search?q=iPhone&pretty"

# 结构化查询
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match": {
      "name": "iPhone"
    }
  }
}'
```

### 4.3 更新文档

```bash
# 完整更新文档
curl -X PUT "localhost:9200/products/_doc/1" -H 'Content-Type: application/json' -d'
{
  "name": "iPhone 13 Pro",
  "price": 1099.99,
  "description": "Professional iPhone model",
  "category": "electronics",
  "created_at": "2023-01-01T00:00:00Z"
}'

# 部分更新文档
curl -X POST "localhost:9200/products/_update/1" -H 'Content-Type: application/json' -d'
{
  "doc": {
    "price": 1199.99
  }
}'

# 使用脚本更新
curl -X POST "localhost:9200/products/_update/1" -H 'Content-Type: application/json' -d'
{
  "script": {
    "source": "ctx._source.price += params.increment",
    "params": {
      "increment": 100
    }
  }
}'
```

### 4.4 删除文档

```bash
# 根据ID删除文档
curl -X DELETE "localhost:9200/products/_doc/1"

# 根据查询删除文档
curl -X POST "localhost:9200/products/_delete_by_query" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match": {
      "category": "electronics"
    }
  }
}'
```

## 5. 高级查询操作

### 5.1 基本查询

```bash
# Match查询
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match": {
      "description": "smartphone"
    }
  }
}'

# Term查询（精确匹配）
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "term": {
      "category": "electronics"
    }
  }
}'

# Range查询
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "range": {
      "price": {
        "gte": 500,
        "lte": 1000
      }
    }
  }
}'
```

### 5.2 复合查询

```bash
# Bool查询
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "bool": {
      "must": [
        {"match": {"category": "electronics"}}
      ],
      "filter": [
        {"range": {"price": {"gte": 500}}}
      ],
      "must_not": [
        {"match": {"name": "Samsung"}}
      ]
    }
  }
}'

# Multi-match查询
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "multi_match": {
      "query": "iPhone",
      "fields": ["name", "description"]
    }
  }
}'
```

### 5.3 排序和分页

```bash
# 排序查询
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match_all": {}
  },
  "sort": [
    {"price": {"order": "desc"}},
    {"created_at": {"order": "asc"}}
  ]
}'

# 分页查询
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match_all": {}
  },
  "from": 0,
  "size": 10
}'
```

### 5.4 字段过滤

```bash
# 指定返回字段
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match_all": {}
  },
  "_source": ["name", "price"]
}'

# 排除字段
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match_all": {}
  },
  "_source": {
    "excludes": ["description"]
  }
}'
```

## 6. 聚合操作

### 6.1 指标聚合

```bash
# 平均值聚合
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "size": 0,
  "aggs": {
    "avg_price": {
      "avg": {
        "field": "price"
      }
    }
  }
}'

# 多个指标聚合
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "size": 0,
  "aggs": {
    "price_stats": {
      "stats": {
        "field": "price"
      }
    }
  }
}'
```

### 6.2 桶聚合

```bash
# Terms聚合（分组）
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "size": 0,
  "aggs": {
    "categories": {
      "terms": {
        "field": "category"
      }
    }
  }
}'

# 日期直方图聚合
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "size": 0,
  "aggs": {
    "sales_over_time": {
      "date_histogram": {
        "field": "created_at",
        "calendar_interval": "month"
      }
    }
  }
}'
```

### 6.3 嵌套聚合

```bash
# 分组后计算平均值
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "size": 0,
  "aggs": {
    "categories": {
      "terms": {
        "field": "category"
      },
      "aggs": {
        "avg_price": {
          "avg": {
            "field": "price"
          }
        }
      }
    }
  }
}'
```

## 7. 索引模板和别名

### 7.1 索引模板

```bash
# 创建索引模板
curl -X PUT "localhost:9200/_template/product_template" -H 'Content-Type: application/json' -d'
{
  "index_patterns": ["product-*"],
  "settings": {
    "number_of_shards": 1,
    "number_of_replicas": 0
  },
  "mappings": {
    "properties": {
      "name": {"type": "text"},
      "price": {"type": "double"},
      "category": {"type": "keyword"}
    }
  }
}'

# 查看模板
curl -X GET "localhost:9200/_template/product_template?pretty"

# 删除模板
curl -X DELETE "localhost:9200/_template/product_template"
```

### 7.2 索引别名

```bash
# 创建别名
curl -X POST "localhost:9200/_aliases" -H 'Content-Type: application/json' -d'
{
  "actions": [
    {
      "add": {
        "index": "products",
        "alias": "current_products"
      }
    }
  ]
}'

# 查看别名
curl -X GET "localhost:9200/_alias?pretty"

# 删除别名
curl -X POST "localhost:9200/_aliases" -H 'Content-Type: application/json' -d'
{
  "actions": [
    {
      "remove": {
        "index": "products",
        "alias": "current_products"
      }
    }
  ]
}'
```

## 8. 集群管理

### 8.1 集群健康和状态

```bash
# 查看集群健康状态
curl -X GET "localhost:9200/_cluster/health?pretty"

# 查看集群状态
curl -X GET "localhost:9200/_cluster/state?pretty"

# 查看集群统计信息
curl -X GET "localhost:9200/_cluster/stats?pretty"

# 查看待处理任务
curl -X GET "localhost:9200/_cluster/pending_tasks?pretty"
```

### 8.2 节点管理

```bash
# 查看节点信息
curl -X GET "localhost:9200/_nodes?pretty"

# 查看节点统计信息
curl -X GET "localhost:9200/_nodes/stats?pretty"

# 查看特定节点信息
curl -X GET "localhost:9200/_nodes/node1?pretty"
```

### 8.3 分片管理

```bash
# 查看分片分配
curl -X GET "localhost:9200/_cat/allocation?v"

# 手动分配分片
curl -X POST "localhost:9200/_cluster/reroute" -H 'Content-Type: application/json' -d'
{
  "commands": [
    {
      "move": {
        "index": "products",
        "shard": 0,
        "from_node": "node1",
        "to_node": "node2"
      }
    }
  ]
}'
```

## 9. Docker容器管理

### 9.1 单节点Elasticsearch

```bash
# 运行单节点Elasticsearch
docker run -d \
  --name elasticsearch \
  -p 9200:9200 \
  -p 9300:9300 \
  -e "discovery.type=single-node" \
  -e "ES_JAVA_OPTS=-Xms512m -Xmx512m" \
  elasticsearch:8.11.0

# 查看容器日志
docker logs elasticsearch

# 进入容器
docker exec -it elasticsearch bash
```

### 9.2 Docker Compose集群

```yaml
# docker-compose.yml
version: '3.8'
services:
  es01:
    image: elasticsearch:8.11.0
    container_name: es01
    environment:
      - node.name=es01
      - cluster.name=es-docker-cluster
      - discovery.seed_hosts=es02,es03
      - cluster.initial_master_nodes=es01,es02,es03
      - bootstrap.memory_lock=true
      - "ES_JAVA_OPTS=-Xms512m -Xmx512m"
      - xpack.security.enabled=false
    ulimits:
      memlock:
        soft: -1
        hard: -1
    volumes:
      - data01:/usr/share/elasticsearch/data
    ports:
      - 9200:9200
    networks:
      - elastic

  es02:
    image: elasticsearch:8.11.0
    container_name: es02
    environment:
      - node.name=es02
      - cluster.name=es-docker-cluster
      - discovery.seed_hosts=es01,es03
      - cluster.initial_master_nodes=es01,es02,es03
      - bootstrap.memory_lock=true
      - "ES_JAVA_OPTS=-Xms512m -Xmx512m"
      - xpack.security.enabled=false
    ulimits:
      memlock:
        soft: -1
        hard: -1
    volumes:
      - data02:/usr/share/elasticsearch/data
    networks:
      - elastic

  es03:
    image: elasticsearch:8.11.0
    container_name: es03
    environment:
      - node.name=es03
      - cluster.name=es-docker-cluster
      - discovery.seed_hosts=es01,es02
      - cluster.initial_master_nodes=es01,es02,es03
      - bootstrap.memory_lock=true
      - "ES_JAVA_OPTS=-Xms512m -Xmx512m"
      - xpack.security.enabled=false
    ulimits:
      memlock:
        soft: -1
        hard: -1
    volumes:
      - data03:/usr/share/elasticsearch/data
    networks:
      - elastic

  kibana:
    image: kibana:8.11.0
    container_name: kibana
    ports:
      - 5601:5601
    environment:
      ELASTICSEARCH_URL: http://es01:9200
      ELASTICSEARCH_HOSTS: '["http://es01:9200","http://es02:9200","http://es03:9200"]'
    networks:
      - elastic

volumes:
  data01:
    driver: local
  data02:
    driver: local
  data03:
    driver: local

networks:
  elastic:
    driver: bridge
```

```bash
# 启动集群
docker-compose up -d

# 查看集群状态
docker-compose ps

# 停止集群
docker-compose down

# 停止并删除数据
docker-compose down -v
```

### 9.3 容器管理命令

```bash
# 查看Elasticsearch容器状态
docker ps | grep elasticsearch

# 重启容器
docker restart elasticsearch

# 查看容器资源使用
docker stats elasticsearch

# 备份数据
docker exec elasticsearch curl -X PUT "localhost:9200/_snapshot/my_backup" -H 'Content-Type: application/json' -d'
{
  "type": "fs",
  "settings": {
    "location": "/usr/share/elasticsearch/backup"
  }
}'

# 创建快照
docker exec elasticsearch curl -X PUT "localhost:9200/_snapshot/my_backup/snapshot_1?wait_for_completion=true"
```

## 10. 性能优化和监控

### 10.1 性能监控

```bash
# 查看索引性能统计
curl -X GET "localhost:9200/_stats?pretty"

# 查看慢查询日志
curl -X GET "localhost:9200/_nodes/stats/indices/search?pretty"

# 查看JVM信息
curl -X GET "localhost:9200/_nodes/stats/jvm?pretty"

# 查看线程池信息
curl -X GET "localhost:9200/_nodes/stats/thread_pool?pretty"
```

### 10.2 性能优化设置

```bash
# 设置刷新间隔
curl -X PUT "localhost:9200/products/_settings" -H 'Content-Type: application/json' -d'
{
  "refresh_interval": "30s"
}'

# 设置副本数量
curl -X PUT "localhost:9200/products/_settings" -H 'Content-Type: application/json' -d'
{
  "number_of_replicas": 0
}'

# 禁用动态映射
curl -X PUT "localhost:9200/products/_mapping" -H 'Content-Type: application/json' -d'
{
  "dynamic": false
}'
```

### 10.3 缓存管理

```bash
# 清除缓存
curl -X POST "localhost:9200/_cache/clear"

# 清除特定索引缓存
curl -X POST "localhost:9200/products/_cache/clear"

# 查看缓存统计
curl -X GET "localhost:9200/_stats/indices/query_cache,request_cache?pretty"
```

## 11. 数据备份和恢复

### 11.1 快照备份

```bash
# 创建快照仓库
curl -X PUT "localhost:9200/_snapshot/my_backup" -H 'Content-Type: application/json' -d'
{
  "type": "fs",
  "settings": {
    "location": "/path/to/backup"
  }
}'

# 创建快照
curl -X PUT "localhost:9200/_snapshot/my_backup/snapshot_1" -H 'Content-Type: application/json' -d'
{
  "indices": "products",
  "ignore_unavailable": true,
  "include_global_state": false
}'

# 查看快照状态
curl -X GET "localhost:9200/_snapshot/my_backup/snapshot_1?pretty"

# 恢复快照
curl -X POST "localhost:9200/_snapshot/my_backup/snapshot_1/_restore" -H 'Content-Type: application/json' -d'
{
  "indices": "products",
  "ignore_unavailable": true,
  "include_global_state": false,
  "rename_pattern": "products",
  "rename_replacement": "restored_products"
}'
```

### 11.2 数据导入导出

```bash
# 使用elasticdump导出数据
npm install -g elasticdump
elasticdump --input=http://localhost:9200/products --output=products.json --type=data

# 导入数据
elasticdump --input=products.json --output=http://localhost:9200/products --type=data

# 导出映射
elasticdump --input=http://localhost:9200/products --output=products_mapping.json --type=mapping
```

## 12. 安全配置

### 12.1 基本安全设置

```bash
# 启用安全功能（需要在elasticsearch.yml中配置）
# xpack.security.enabled: true

# 设置用户密码
curl -X POST "localhost:9200/_security/user/elastic/_password" -H 'Content-Type: application/json' -d'
{
  "password": "new_password"
}'

# 创建用户
curl -X POST "localhost:9200/_security/user/myuser" -H 'Content-Type: application/json' -d'
{
  "password": "mypassword",
  "roles": ["kibana_user", "monitoring_user"],
  "full_name": "My User",
  "email": "myuser@example.com"
}'
```

### 12.2 角色和权限

```bash
# 创建角色
curl -X POST "localhost:9200/_security/role/my_role" -H 'Content-Type: application/json' -d'
{
  "cluster": ["monitor"],
  "indices": [
    {
      "names": ["products*"],
      "privileges": ["read", "write"]
    }
  ]
}'

# 查看用户权限
curl -X GET "localhost:9200/_security/user/myuser?pretty"
```

## 13. 常用工具和技巧

### 13.1 Kibana集成

```bash
# Kibana Dev Tools中的常用命令
GET /_cluster/health
GET /products/_search
POST /products/_doc
{
  "name": "New Product",
  "price": 99.99
}
```

### 13.2 批量操作技巧

```bash
# 批量索引优化
curl -X POST "localhost:9200/_bulk" -H 'Content-Type: application/json' --data-binary @bulk_data.json

# 批量更新
curl -X POST "localhost:9200/products/_update_by_query" -H 'Content-Type: application/json' -d'
{
  "script": {
    "source": "ctx._source.price = ctx._source.price * 1.1"
  },
  "query": {
    "term": {
      "category": "electronics"
    }
  }
}'
```

### 13.3 调试和故障排除

```bash
# 查看慢查询
curl -X GET "localhost:9200/_nodes/stats/indices/search?pretty" | grep slow

# 分析查询性能
curl -X GET "localhost:9200/products/_search" -H 'Content-Type: application/json' -d'
{
  "profile": true,
  "query": {
    "match": {
      "name": "iPhone"
    }
  }
}'

# 验证查询
curl -X GET "localhost:9200/products/_validate/query?explain" -H 'Content-Type: application/json' -d'
{
  "query": {
    "match": {
      "name": "iPhone"
    }
  }
}'
```

### 13.4 实用脚本

```bash
#!/bin/bash
# 健康检查脚本
check_es_health() {
    local health=$(curl -s "localhost:9200/_cluster/health" | jq -r '.status')
    if [ "$health" = "green" ]; then
        echo "Elasticsearch cluster is healthy"
    elif [ "$health" = "yellow" ]; then
        echo "Elasticsearch cluster has warnings"
    else
        echo "Elasticsearch cluster is unhealthy"
    fi
}

# 索引大小监控
check_index_size() {
    curl -s "localhost:9200/_cat/indices?v&h=index,store.size&s=store.size:desc"
}

# 执行检查
check_es_health
check_index_size
```

## 14. 最佳实践

### 14.1 索引设计

- 合理设置分片数量：小索引使用1个分片，大索引根据数据量和查询需求设置
- 避免过多的字段：每个文档的字段数量建议不超过1000个
- 使用合适的数据类型：避免使用text类型存储不需要全文搜索的数据
- 设置合理的映射：禁用不需要的功能如_all字段

### 14.2 查询优化

- 使用filter而不是query进行精确匹配
- 避免深度分页，使用scroll API处理大量数据
- 合理使用缓存：query cache和request cache
- 避免使用通配符开头的查询

### 14.3 集群管理

- 定期监控集群健康状态
- 设置合理的堆内存大小（不超过32GB）
- 使用SSD存储提高性能
- 定期备份重要数据
- 监控磁盘使用率，避免磁盘满载

### 14.4 安全建议

- 启用身份验证和授权
- 使用HTTPS加密传输
- 定期更新Elasticsearch版本
- 限制网络访问，不要将Elasticsearch直接暴露到公网
- 使用专用用户运行Elasticsearch服务
