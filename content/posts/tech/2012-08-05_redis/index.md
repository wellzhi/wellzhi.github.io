+++
date = '2012-08-05T00:00:00+08:00'
draft = false
title = 'Redis常用命令'
comments = false
tags = ["Redis", "缓存", "Docker", "命令", "数据库"]
tocOpen = true
summary = "Redis 是一个开源的内存数据结构存储系统，它被广泛用于缓存、会话存储、实时分析等场景。本文详细介绍了Redis的常用命令，包括数据结构操作、事务管理、持久化配置以及集群管理等实用技巧。"
+++

## 1. 简介

Redis（Remote Dictionary Server）是一个开源的内存数据结构存储系统，它可以用作数据库、缓存和消息代理。Redis 支持多种数据结构，如字符串、哈希、列表、集合、有序集合等，并提供了丰富的操作命令。本文将详细介绍Redis的常用命令，快速掌握基本操作和高级特性。

## 2. 连接与基本操作

### 2.1 连接Redis

```bash
# 连接本地Redis（默认端口6379）
redis-cli

# 连接指定主机和端口
redis-cli -h hostname -p port

# 使用密码连接
redis-cli -h hostname -p port -a password

# 连接并选择数据库
redis-cli -h hostname -p port -n database_number
```

### 2.2 基本命令

```bash
# 测试连接
PING

# 查看服务器信息
INFO

# 选择数据库（0-15）
SELECT 0

# 查看当前数据库键的数量
DBSIZE

# 清空当前数据库
FLUSHDB

# 清空所有数据库
FLUSHALL

# 退出
QUIT
```

## 3. 键（Key）操作

### 3.1 键的基本操作

```bash
# 查看所有键
KEYS *

# 查看匹配模式的键
KEYS pattern
KEYS user:*
KEYS *name*

# 检查键是否存在
EXISTS key
EXISTS username

# 删除键
DEL key [key ...]
DEL username email

# 重命名键
RENAME old_key new_key
RENAME username user_name

# 仅当新键不存在时重命名
RENAMENX old_key new_key
```

### 3.2 键的过期时间

```bash
# 设置键的过期时间（秒）
EXPIRE key seconds
EXPIRE session:123 3600

# 设置键的过期时间（毫秒）
PEXPIRE key milliseconds
PEXPIRE session:123 3600000

# 设置键的过期时间戳（秒）
EXPIREAT key timestamp
EXPIREAT session:123 1609459200

# 设置键的过期时间戳（毫秒）
PEXPIREAT key milliseconds-timestamp

# 查看键的剩余生存时间（秒）
TTL key
TTL session:123

# 查看键的剩余生存时间（毫秒）
PTTL key

# 移除键的过期时间
PERSIST key
PERSIST session:123
```

### 3.3 键的类型和信息

```bash
# 查看键的数据类型
TYPE key
TYPE username

# 随机返回一个键
RANDOMKEY

# 查看键的内存使用情况
MEMORY USAGE key
MEMORY USAGE username
```

## 4. 字符串（String）操作

### 4.1 基本字符串操作

```bash
# 设置字符串值
SET key value
SET username "john_doe"

# 获取字符串值
GET key
GET username

# 设置多个键值对
MSET key1 value1 key2 value2
MSET name "John" age "25" city "New York"

# 获取多个键的值
MGET key1 key2 key3
MGET name age city

# 仅当键不存在时设置
SETNX key value
SETNX username "jane_doe"

# 设置键值并指定过期时间
SETEX key seconds value
SETEX session:123 3600 "user_data"

# 设置键值并指定过期时间（毫秒）
PSETEX key milliseconds value
PSETEX session:123 3600000 "user_data"
```

### 4.2 字符串操作

```bash
# 追加字符串
APPEND key value
APPEND message " World"

# 获取字符串长度
STRLEN key
STRLEN message

# 获取子字符串
GETRANGE key start end
GETRANGE message 0 4

# 设置子字符串
SETRANGE key offset value
SETRANGE message 6 "Redis"

# 获取并设置新值
GETSET key value
GETSET counter 0
```

### 4.3 数值操作

```bash
# 递增
INCR key
INCR counter

# 递减
DECR key
DECR counter

# 按指定值递增
INCRBY key increment
INCRBY counter 5

# 按指定值递减
DECRBY key decrement
DECRBY counter 3

# 浮点数递增
INCRBYFLOAT key increment
INCRBYFLOAT price 10.5
```

## 5. 哈希（Hash）操作

### 5.1 基本哈希操作

```bash
# 设置哈希字段
HSET key field value
HSET user:1 name "John"
HSET user:1 age 25

# 获取哈希字段值
HGET key field
HGET user:1 name

# 设置多个哈希字段
HMSET key field1 value1 field2 value2
HMSET user:1 name "John" age 25 city "New York"

# 获取多个哈希字段值
HMGET key field1 field2
HMGET user:1 name age

# 获取所有字段和值
HGETALL key
HGETALL user:1

# 仅当字段不存在时设置
HSETNX key field value
HSETNX user:1 email "john@example.com"
```

### 5.2 哈希字段操作

```bash
# 删除哈希字段
HDEL key field [field ...]
HDEL user:1 age city

# 检查字段是否存在
HEXISTS key field
HEXISTS user:1 name

# 获取所有字段名
HKEYS key
HKEYS user:1

# 获取所有字段值
HVALS key
HVALS user:1

# 获取字段数量
HLEN key
HLEN user:1

# 获取字段值的字符串长度
HSTRLEN key field
HSTRLEN user:1 name
```

### 5.3 哈希数值操作

```bash
# 字段值递增
HINCRBY key field increment
HINCRBY user:1 age 1

# 字段值浮点递增
HINCRBYFLOAT key field increment
HINCRBYFLOAT user:1 score 10.5
```

## 6. 列表（List）操作

### 6.1 基本列表操作

```bash
# 从左侧推入元素
LPUSH key element [element ...]
LPUSH mylist "world" "hello"

# 从右侧推入元素
RPUSH key element [element ...]
RPUSH mylist "redis" "tutorial"

# 从左侧弹出元素
LPOP key
LPOP mylist

# 从右侧弹出元素
RPOP key
RPOP mylist

# 获取列表长度
LLEN key
LLEN mylist

# 获取列表范围内的元素
LRANGE key start stop
LRANGE mylist 0 -1  # 获取所有元素
LRANGE mylist 0 2   # 获取前3个元素
```

### 6.2 列表索引操作

```bash
# 获取指定索引的元素
LINDEX key index
LINDEX mylist 0

# 设置指定索引的元素值
LSET key index element
LSET mylist 0 "new_value"

# 在指定元素前/后插入新元素
LINSERT key BEFORE|AFTER pivot element
LINSERT mylist BEFORE "world" "beautiful"
LINSERT mylist AFTER "hello" "wonderful"
```

### 6.3 列表修剪和删除

```bash
# 保留指定范围内的元素
LTRIM key start stop
LTRIM mylist 1 -1

# 删除指定值的元素
LREM key count element
LREM mylist 2 "hello"  # 删除2个"hello"
LREM mylist -1 "world" # 从尾部删除1个"world"
LREM mylist 0 "redis"  # 删除所有"redis"
```

### 6.4 阻塞操作

```bash
# 阻塞式左侧弹出
BLPOP key [key ...] timeout
BLPOP mylist 10

# 阻塞式右侧弹出
BRPOP key [key ...] timeout
BRPOP mylist 10

# 阻塞式右侧弹出并左侧推入另一个列表
BRPOPLPUSH source destination timeout
BRPOPLPUSH list1 list2 10
```

## 7. 集合（Set）操作

### 7.1 基本集合操作

```bash
# 添加成员
SADD key member [member ...]
SADD myset "apple" "banana" "orange"

# 获取所有成员
SMEMBERS key
SMEMBERS myset

# 检查成员是否存在
SISMEMBER key member
SISMEMBER myset "apple"

# 获取集合大小
SCARD key
SCARD myset

# 删除成员
SREM key member [member ...]
SREM myset "banana"

# 随机获取成员
SRANDMEMBER key [count]
SRANDMEMBER myset
SRANDMEMBER myset 2

# 随机弹出成员
SPOP key [count]
SPOP myset
SPOP myset 2
```

### 7.2 集合运算

```bash
# 交集
SINTER key [key ...]
SINTER set1 set2

# 并集
SUNION key [key ...]
SUNION set1 set2

# 差集
SDIFF key [key ...]
SDIFF set1 set2

# 交集并存储到新集合
SINTERSTORE destination key [key ...]
SINTERSTORE result_set set1 set2

# 并集并存储到新集合
SUNIONSTORE destination key [key ...]
SUNIONSTORE result_set set1 set2

# 差集并存储到新集合
SDIFFSTORE destination key [key ...]
SDIFFSTORE result_set set1 set2
```

### 7.3 集合移动

```bash
# 移动成员到另一个集合
SMOVE source destination member
SMOVE set1 set2 "apple"
```

## 8. 有序集合（Sorted Set）操作

### 8.1 基本有序集合操作

```bash
# 添加成员
ZADD key [NX|XX] [CH] [INCR] score member [score member ...]
ZADD leaderboard 100 "player1" 200 "player2" 150 "player3"

# 获取成员分数
ZSCORE key member
ZSCORE leaderboard "player1"

# 获取成员排名（从0开始，分数从低到高）
ZRANK key member
ZRANK leaderboard "player1"

# 获取成员排名（从0开始，分数从高到低）
ZREVRANK key member
ZREVRANK leaderboard "player1"

# 获取有序集合大小
ZCARD key
ZCARD leaderboard

# 删除成员
ZREM key member [member ...]
ZREM leaderboard "player1"
```

### 8.2 范围查询

```bash
# 按排名范围获取成员（分数从低到高）
ZRANGE key start stop [WITHSCORES]
ZRANGE leaderboard 0 2
ZRANGE leaderboard 0 2 WITHSCORES

# 按排名范围获取成员（分数从高到低）
ZREVRANGE key start stop [WITHSCORES]
ZREVRANGE leaderboard 0 2 WITHSCORES

# 按分数范围获取成员
ZRANGEBYSCORE key min max [WITHSCORES] [LIMIT offset count]
ZRANGEBYSCORE leaderboard 100 200
ZRANGEBYSCORE leaderboard 100 200 WITHSCORES
ZRANGEBYSCORE leaderboard 100 200 LIMIT 0 10

# 按分数范围获取成员（从高到低）
ZREVRANGEBYSCORE key max min [WITHSCORES] [LIMIT offset count]
ZREVRANGEBYSCORE leaderboard 200 100 WITHSCORES
```

### 8.3 分数操作

```bash
# 增加成员分数
ZINCRBY key increment member
ZINCRBY leaderboard 10 "player1"

# 统计分数范围内的成员数量
ZCOUNT key min max
ZCOUNT leaderboard 100 200

# 删除排名范围内的成员
ZREMRANGEBYRANK key start stop
ZREMRANGEBYRANK leaderboard 0 2

# 删除分数范围内的成员
ZREMRANGEBYSCORE key min max
ZREMRANGEBYSCORE leaderboard 100 150
```

### 8.4 有序集合运算

```bash
# 交集
ZINTERSTORE destination numkeys key [key ...] [WEIGHTS weight [weight ...]] [AGGREGATE SUM|MIN|MAX]
ZINTERSTORE result 2 zset1 zset2

# 并集
ZUNIONSTORE destination numkeys key [key ...] [WEIGHTS weight [weight ...]] [AGGREGATE SUM|MIN|MAX]
ZUNIONSTORE result 2 zset1 zset2
```

## 9. 事务管理

### 9.1 基本事务操作

```bash
# 开始事务
MULTI

# 执行事务
EXEC

# 取消事务
DISCARD

# 监视键（乐观锁）
WATCH key [key ...]

# 取消监视
UNWATCH
```

### 9.2 事务示例

```bash
# 转账事务示例
WATCH account:1 account:2
MULTI
DECRBY account:1 100
INCRBY account:2 100
EXEC

# 如果在WATCH和EXEC之间，被监视的键被修改，事务将被取消
```

## 10. 发布订阅

### 10.1 基本发布订阅

```bash
# 订阅频道
SUBSCRIBE channel [channel ...]
SUBSCRIBE news sports

# 取消订阅
UNSUBSCRIBE [channel [channel ...]]
UNSUBSCRIBE news

# 发布消息
PUBLISH channel message
PUBLISH news "Breaking news!"

# 模式订阅
PSUBSCRIBE pattern [pattern ...]
PSUBSCRIBE news:*

# 取消模式订阅
PUNSUBSCRIBE [pattern [pattern ...]]
PUNSUBSCRIBE news:*
```

### 10.2 发布订阅信息

```bash
# 查看活跃频道
PUBSUB CHANNELS [pattern]
PUBSUB CHANNELS
PUBSUB CHANNELS news:*

# 查看频道订阅数
PUBSUB NUMSUB [channel [channel ...]]
PUBSUB NUMSUB news sports

# 查看模式订阅数
PUBSUB NUMPAT
```

## 11. 持久化配置

### 11.1 RDB持久化

```bash
# 手动触发RDB快照
SAVE

# 异步触发RDB快照
BGSAVE

# 查看最后一次成功保存的时间
LASTSAVE

# 配置自动保存（在redis.conf中）
# save 900 1      # 900秒内至少1个键被修改
# save 300 10     # 300秒内至少10个键被修改
# save 60 10000   # 60秒内至少10000个键被修改
```

### 11.2 AOF持久化

```bash
# 手动重写AOF文件
BGREWRITEAOF

# 配置AOF（在redis.conf中）
# appendonly yes
# appendfilename "appendonly.aof"
# appendfsync everysec  # always, everysec, no
```

### 11.3 持久化相关配置

```ini
# redis.conf 持久化配置示例

# RDB配置
save 900 1
save 300 10
save 60 10000
stop-writes-on-bgsave-error yes
rdbcompression yes
rdbchecksum yes
dbfilename dump.rdb
dir /var/lib/redis

# AOF配置
appendonly yes
appendfilename "appendonly.aof"
appendfsync everysec
no-appendfsync-on-rewrite no
auto-aof-rewrite-percentage 100
auto-aof-rewrite-min-size 64mb
aof-load-truncated yes
```

## 12. 集群管理

### 12.1 Redis Sentinel（哨兵模式）

```bash
# 启动Sentinel
redis-sentinel /path/to/sentinel.conf

# Sentinel配置文件示例
# sentinel.conf
port 26379
sentinel monitor mymaster 127.0.0.1 6379 2
sentinel down-after-milliseconds mymaster 5000
sentinel failover-timeout mymaster 10000
sentinel parallel-syncs mymaster 1
```

### 12.2 Redis Cluster（集群模式）

```bash
# 创建集群
redis-cli --cluster create 127.0.0.1:7000 127.0.0.1:7001 127.0.0.1:7002 127.0.0.1:7003 127.0.0.1:7004 127.0.0.1:7005 --cluster-replicas 1

# 检查集群状态
redis-cli --cluster check 127.0.0.1:7000

# 查看集群信息
redis-cli -c -p 7000
CLUSTER INFO
CLUSTER NODES

# 添加节点
redis-cli --cluster add-node 127.0.0.1:7006 127.0.0.1:7000

# 删除节点
redis-cli --cluster del-node 127.0.0.1:7000 node_id

# 重新分片
redis-cli --cluster reshard 127.0.0.1:7000
```

### 12.3 集群相关命令

```bash
# 集群信息
CLUSTER INFO

# 集群节点
CLUSTER NODES

# 集群状态
CLUSTER SLOTS

# 键所在的槽
CLUSTER KEYSLOT key

# 槽中的键数量
CLUSTER COUNTKEYSINSLOT slot

# 获取槽中的键
CLUSTER GETKEYSINSLOT slot count
```

## 13. Redis Docker 容器管理

### 13.1 运行Redis容器

```bash
# 基本运行
docker run --name redis-container -d redis:7.0

# 指定端口和密码
docker run --name redis-container \
  -p 6379:6379 \
  -d redis:7.0 \
  redis-server --requirepass mypassword

# 使用配置文件
docker run --name redis-container \
  -p 6379:6379 \
  -v /path/to/redis.conf:/usr/local/etc/redis/redis.conf \
  -v redis-data:/data \
  -d redis:7.0 \
  redis-server /usr/local/etc/redis/redis.conf

# 持久化数据
docker run --name redis-container \
  -p 6379:6379 \
  -v redis-data:/data \
  -d redis:7.0 \
  redis-server --appendonly yes
```

### 13.2 Docker Compose 配置

```yaml
# docker-compose.yml
version: '3.8'
services:
  redis:
    image: redis:7.0
    container_name: redis-container
    restart: always
    ports:
      - "6379:6379"
    volumes:
      - redis-data:/data
      - ./redis.conf:/usr/local/etc/redis/redis.conf
    command: redis-server /usr/local/etc/redis/redis.conf
    environment:
      - REDIS_PASSWORD=mypassword
    networks:
      - app-network

  redis-commander:
    image: rediscommander/redis-commander:latest
    container_name: redis-commander
    restart: always
    ports:
      - "8081:8081"
    environment:
      - REDIS_HOSTS=local:redis:6379
      - REDIS_PASSWORD=mypassword
    depends_on:
      - redis
    networks:
      - app-network

volumes:
  redis-data:

networks:
  app-network:
    driver: bridge
```

### 13.3 Redis集群Docker配置

```yaml
# redis-cluster-compose.yml
version: '3.8'
services:
  redis-node-1:
    image: redis:7.0
    container_name: redis-node-1
    ports:
      - "7001:6379"
      - "17001:16379"
    volumes:
      - ./redis-cluster.conf:/usr/local/etc/redis/redis.conf
    command: redis-server /usr/local/etc/redis/redis.conf
    networks:
      - redis-cluster

  redis-node-2:
    image: redis:7.0
    container_name: redis-node-2
    ports:
      - "7002:6379"
      - "17002:16379"
    volumes:
      - ./redis-cluster.conf:/usr/local/etc/redis/redis.conf
    command: redis-server /usr/local/etc/redis/redis.conf
    networks:
      - redis-cluster

  redis-node-3:
    image: redis:7.0
    container_name: redis-node-3
    ports:
      - "7003:6379"
      - "17003:16379"
    volumes:
      - ./redis-cluster.conf:/usr/local/etc/redis/redis.conf
    command: redis-server /usr/local/etc/redis/redis.conf
    networks:
      - redis-cluster

  redis-node-4:
    image: redis:7.0
    container_name: redis-node-4
    ports:
      - "7004:6379"
      - "17004:16379"
    volumes:
      - ./redis-cluster.conf:/usr/local/etc/redis/redis.conf
    command: redis-server /usr/local/etc/redis/redis.conf
    networks:
      - redis-cluster

  redis-node-5:
    image: redis:7.0
    container_name: redis-node-5
    ports:
      - "7005:6379"
      - "17005:16379"
    volumes:
      - ./redis-cluster.conf:/usr/local/etc/redis/redis.conf
    command: redis-server /usr/local/etc/redis/redis.conf
    networks:
      - redis-cluster

  redis-node-6:
    image: redis:7.0
    container_name: redis-node-6
    ports:
      - "7006:6379"
      - "17006:16379"
    volumes:
      - ./redis-cluster.conf:/usr/local/etc/redis/redis.conf
    command: redis-server /usr/local/etc/redis/redis.conf
    networks:
      - redis-cluster

networks:
  redis-cluster:
    driver: bridge
```

### 13.4 容器管理命令

```bash
# 启动容器
docker start redis-container

# 停止容器
docker stop redis-container

# 重启容器
docker restart redis-container

# 查看容器状态
docker ps
docker ps -a

# 查看容器日志
docker logs redis-container
docker logs -f redis-container  # 实时查看

# 进入容器
docker exec -it redis-container bash
docker exec -it redis-container redis-cli

# 连接Redis（带密码）
docker exec -it redis-container redis-cli -a mypassword

# 删除容器
docker rm redis-container
docker rm -f redis-container  # 强制删除
```

### 13.5 数据备份和恢复（Docker）

```bash
# 备份Redis数据
docker exec redis-container redis-cli BGSAVE
docker cp redis-container:/data/dump.rdb ./backup/

# 恢复Redis数据
docker cp ./backup/dump.rdb redis-container:/data/
docker restart redis-container

# 备份数据卷
docker run --rm -v redis-data:/data -v $(pwd):/backup alpine tar czf /backup/redis-backup.tar.gz -C /data .

# 恢复数据卷
docker run --rm -v redis-data:/data -v $(pwd):/backup alpine tar xzf /backup/redis-backup.tar.gz -C /data
```

## 14. 性能监控和优化

### 14.1 性能监控命令

```bash
# 实时监控Redis命令
MONITOR

# 查看Redis统计信息
INFO
INFO server
INFO memory
INFO stats
INFO replication
INFO cpu

# 查看慢查询日志
SLOWLOG GET [count]
SLOWLOG GET 10

# 查看慢查询日志长度
SLOWLOG LEN

# 重置慢查询日志
SLOWLOG RESET

# 查看客户端连接
CLIENT LIST

# 查看内存使用情况
MEMORY STATS
MEMORY USAGE key

# 内存分析
MEMORY DOCTOR
```

### 14.2 性能配置优化

```ini
# redis.conf 性能优化配置

# 内存配置
maxmemory 2gb
maxmemory-policy allkeys-lru

# 网络配置
tcp-keepalive 300
timeout 0
tcp-backlog 511

# 慢查询配置
slowlog-log-slower-than 10000
slowlog-max-len 128

# 客户端配置
maxclients 10000

# 后台任务配置
lazy-expire-disabled no
lazyfree-lazy-eviction yes
lazyfree-lazy-expire yes
lazyfree-lazy-server-del yes

# 哈希配置
hash-max-ziplist-entries 512
hash-max-ziplist-value 64

# 列表配置
list-max-ziplist-size -2
list-compress-depth 0

# 集合配置
set-max-intset-entries 512

# 有序集合配置
zset-max-ziplist-entries 128
zset-max-ziplist-value 64
```

### 14.3 内存优化

```bash
# 查看内存使用情况
INFO memory

# 手动触发内存清理
MEMORY PURGE

# 分析大键
redis-cli --bigkeys

# 分析内存使用
redis-cli --memkeys

# 查看键的内存使用
MEMORY USAGE key [SAMPLES count]
```

## 15. 安全配置

### 15.1 认证和授权

```bash
# 设置密码
CONFIG SET requirepass mypassword

# 使用密码认证
AUTH password

# 重命名危险命令（在redis.conf中）
# rename-command FLUSHDB ""
# rename-command FLUSHALL ""
# rename-command CONFIG "CONFIG_9a90f2b4c2d3e5f6"
```

### 15.2 网络安全

```ini
# redis.conf 安全配置

# 绑定IP地址
bind 127.0.0.1 192.168.1.100

# 保护模式
protected-mode yes

# 端口配置
port 6379

# 禁用危险命令
rename-command FLUSHDB ""
rename-command FLUSHALL ""
rename-command EVAL ""
rename-command DEBUG ""
rename-command CONFIG "CONFIG_a1b2c3d4e5f6"
```

## 16. 常用工具和技巧

### 16.1 Redis客户端工具

```bash
# redis-cli 常用选项
redis-cli -h hostname -p port -a password -n database

# 执行命令并退出
redis-cli -h hostname -p port -a password -c "GET key"

# 批量执行命令
echo "SET key1 value1\nSET key2 value2" | redis-cli -h hostname -p port -a password

# 从文件执行命令
redis-cli -h hostname -p port -a password < commands.txt

# 导出数据
redis-cli -h hostname -p port -a password --rdb dump.rdb

# 实时监控
redis-cli -h hostname -p port -a password --latency
redis-cli -h hostname -p port -a password --latency-history
redis-cli -h hostname -p port -a password --latency-dist
```

### 16.2 数据迁移工具

```bash
# redis-dump-load
npm install -g redis-dump-load
redis-dump-load -h source_host -p source_port -a source_password -d destination_host:destination_port:destination_password

# redis-migrate-tool
./redis-migrate-tool -c config.conf

# 使用MIGRATE命令
MIGRATE host port key destination-db timeout [COPY | REPLACE]
MIGRATE 192.168.1.100 6379 mykey 0 5000
```

### 16.3 实用技巧

```bash
# 批量删除键
redis-cli KEYS "pattern*" | xargs redis-cli DEL

# 统计键的数量
redis-cli INFO keyspace

# 查找大键
redis-cli --bigkeys

# 扫描键
SCAN cursor [MATCH pattern] [COUNT count]
SCAN 0 MATCH user:* COUNT 100

# 扫描哈希字段
HSCAN key cursor [MATCH pattern] [COUNT count]
HSCAN user:1 0 MATCH name* COUNT 10

# 扫描集合成员
SSCAN key cursor [MATCH pattern] [COUNT count]
SSCAN myset 0 MATCH a* COUNT 10

# 扫描有序集合成员
ZSCAN key cursor [MATCH pattern] [COUNT count]
ZSCAN leaderboard 0 MATCH player* COUNT 10
```

## 17. 注意事项

1. **合理选择数据结构**：根据业务需求选择最适合的Redis数据结构
2. **设置合理的过期时间**：避免内存泄漏，合理设置键的TTL
3. **监控性能指标**：定期检查慢查询、内存使用情况等
4. **配置持久化策略**：根据数据重要性选择RDB或AOF持久化
5. **安全配置**：设置密码、绑定IP、重命名危险命令
6. **集群部署**：对于高可用需求，使用Sentinel或Cluster模式
7. **容器化部署**：使用Docker简化Redis的部署和管理


