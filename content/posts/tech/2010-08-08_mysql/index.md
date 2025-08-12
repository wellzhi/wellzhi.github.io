+++
date = '2010-08-08T00:00:00+08:00'
draft = false
title = 'MySQL常用命令'
comments = false
tags = ["MySQL", "数据库", "Docker", "命令"]
tocOpen = true
summary = "MySQL 是一个开源的关系型数据库管理系统，它被广泛用于 Web 应用程序的后端数据存储。本文详细介绍了MySQL的常用命令，包括数据库操作、表操作、数据操作以及Docker容器管理等实用技巧。"
+++

## 1. 简介

MySQL 是一个开源的关系型数据库管理系统，它被广泛用于 Web 应用程序的后端数据存储。MySQL 提供了强大的功能和灵活的配置选项，使得它成为了最流行的数据库之一。本文将详细介绍MySQL的常用命令。

## 2. 连接与退出

### 2.1 连接MySQL

```bash
# 连接本地MySQL
mysql -u username -p

# 连接远程MySQL
mysql -h hostname -u username -p

# 指定端口连接
mysql -h hostname -P 3306 -u username -p

# 连接指定数据库
mysql -u username -p database_name
```

### 2.2 退出MySQL

```sql
-- 退出MySQL
EXIT;
-- 或者
QUIT;
-- 或者使用快捷键
\q
```

## 3. 数据库操作

### 3.1 查看数据库

```sql
-- 显示所有数据库
SHOW DATABASES;

-- 查看当前使用的数据库
SELECT DATABASE();

-- 查看数据库创建语句
SHOW CREATE DATABASE database_name;
```

### 3.2 创建数据库

```sql
-- 创建数据库
CREATE DATABASE database_name;

-- 创建数据库并指定字符集
CREATE DATABASE database_name CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- 如果不存在则创建
CREATE DATABASE IF NOT EXISTS database_name;
```

### 3.3 使用数据库

```sql
-- 切换到指定数据库
USE database_name;
```

### 3.4 删除数据库

```sql
-- 删除数据库
DROP DATABASE database_name;

-- 如果存在则删除
DROP DATABASE IF EXISTS database_name;
```

## 4. 表操作

### 4.1 查看表

```sql
-- 显示当前数据库中的所有表
SHOW TABLES;

-- 显示表结构
DESC table_name;
-- 或者
DESCRIBE table_name;
-- 或者
SHOW COLUMNS FROM table_name;

-- 查看表创建语句
SHOW CREATE TABLE table_name;

-- 查看表状态信息
SHOW TABLE STATUS LIKE 'table_name';
```

### 4.2 创建表

```sql
-- 创建基本表
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    email VARCHAR(100) NOT NULL,
    password VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);

-- 创建表并指定存储引擎和字符集
CREATE TABLE products (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    description TEXT,
    category_id INT,
    INDEX idx_category (category_id),
    FOREIGN KEY (category_id) REFERENCES categories(id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```

### 4.3 修改表结构

```sql
-- 添加列
ALTER TABLE table_name ADD COLUMN column_name VARCHAR(50);

-- 修改列类型
ALTER TABLE table_name MODIFY COLUMN column_name VARCHAR(100);

-- 重命名列
ALTER TABLE table_name CHANGE old_column_name new_column_name VARCHAR(50);

-- 删除列
ALTER TABLE table_name DROP COLUMN column_name;

-- 添加索引
ALTER TABLE table_name ADD INDEX index_name (column_name);

-- 添加主键
ALTER TABLE table_name ADD PRIMARY KEY (column_name);

-- 删除索引
ALTER TABLE table_name DROP INDEX index_name;
```

### 4.4 删除表

```sql
-- 删除表
DROP TABLE table_name;

-- 如果存在则删除
DROP TABLE IF EXISTS table_name;

-- 清空表数据但保留表结构
TRUNCATE TABLE table_name;
```

## 5. 数据操作

### 5.1 插入数据

```sql
-- 插入单条记录
INSERT INTO users (username, email, password) 
VALUES ('john_doe', 'john@example.com', 'hashed_password');

-- 插入多条记录
INSERT INTO users (username, email, password) VALUES 
('user1', 'user1@example.com', 'password1'),
('user2', 'user2@example.com', 'password2'),
('user3', 'user3@example.com', 'password3');

-- 插入或更新（如果主键冲突则更新）
INSERT INTO users (id, username, email) 
VALUES (1, 'updated_user', 'updated@example.com')
ON DUPLICATE KEY UPDATE 
username = VALUES(username), 
email = VALUES(email);
```

### 5.2 查询数据

```sql
-- 基本查询
SELECT * FROM users;

-- 指定列查询
SELECT username, email FROM users;

-- 条件查询
SELECT * FROM users WHERE id = 1;
SELECT * FROM users WHERE username LIKE '%john%';
SELECT * FROM users WHERE created_at > '2023-01-01';

-- 排序
SELECT * FROM users ORDER BY created_at DESC;
SELECT * FROM users ORDER BY username ASC, id DESC;

-- 限制结果数量
SELECT * FROM users LIMIT 10;
SELECT * FROM users LIMIT 10 OFFSET 20;

-- 分组和聚合
SELECT COUNT(*) FROM users;
SELECT category_id, COUNT(*) as product_count 
FROM products 
GROUP BY category_id;

-- 连接查询
SELECT u.username, p.name as product_name
FROM users u
JOIN orders o ON u.id = o.user_id
JOIN products p ON o.product_id = p.id;
```

### 5.3 更新数据

```sql
-- 更新单条记录
UPDATE users SET email = 'newemail@example.com' WHERE id = 1;

-- 更新多个字段
UPDATE users SET 
    email = 'newemail@example.com',
    updated_at = NOW()
WHERE id = 1;

-- 批量更新
UPDATE products SET price = price * 1.1 WHERE category_id = 1;
```

### 5.4 删除数据

```sql
-- 删除指定记录
DELETE FROM users WHERE id = 1;

-- 条件删除
DELETE FROM users WHERE created_at < '2022-01-01';

-- 删除所有记录（保留表结构）
DELETE FROM table_name;
```

## 6. 索引操作

```sql
-- 创建索引
CREATE INDEX idx_username ON users(username);
CREATE UNIQUE INDEX idx_email ON users(email);
CREATE INDEX idx_name_category ON products(name, category_id);

-- 查看索引
SHOW INDEX FROM table_name;

-- 删除索引
DROP INDEX index_name ON table_name;

-- 分析表性能
EXPLAIN SELECT * FROM users WHERE username = 'john_doe';
```

## 7. 用户和权限管理

```sql
-- 创建用户
CREATE USER 'newuser'@'localhost' IDENTIFIED BY 'password';
CREATE USER 'remoteuser'@'%' IDENTIFIED BY 'password';

-- 授权
GRANT ALL PRIVILEGES ON database_name.* TO 'username'@'localhost';
GRANT SELECT, INSERT, UPDATE ON table_name TO 'username'@'localhost';

-- 查看权限
SHOW GRANTS FOR 'username'@'localhost';

-- 撤销权限
REVOKE ALL PRIVILEGES ON database_name.* FROM 'username'@'localhost';

-- 刷新权限
FLUSH PRIVILEGES;

-- 删除用户
DROP USER 'username'@'localhost';

-- 修改密码
ALTER USER 'username'@'localhost' IDENTIFIED BY 'new_password';
```

## 8. 备份和恢复

### 8.1 数据备份

```bash
# 备份单个数据库
mysqldump -u username -p database_name > backup.sql

# 备份多个数据库
mysqldump -u username -p --databases db1 db2 > backup.sql

# 备份所有数据库
mysqldump -u username -p --all-databases > all_backup.sql

# 只备份表结构
mysqldump -u username -p --no-data database_name > structure.sql

# 只备份数据
mysqldump -u username -p --no-create-info database_name > data.sql

# 备份并压缩
mysqldump -u username -p database_name | gzip > backup.sql.gz
```

### 8.2 数据恢复

```bash
# 恢复数据库
mysql -u username -p database_name < backup.sql

# 恢复压缩备份
gunzip < backup.sql.gz | mysql -u username -p database_name
```

## MySQL Docker 容器管理

### 9.1 运行MySQL容器

```bash
# 基本运行
docker run --name mysql-container -e MYSQL_ROOT_PASSWORD=rootpassword -d mysql:8.0

# 指定端口和数据卷
docker run --name mysql-container \
  -e MYSQL_ROOT_PASSWORD=rootpassword \
  -e MYSQL_DATABASE=myapp \
  -e MYSQL_USER=appuser \
  -e MYSQL_PASSWORD=apppassword \
  -p 3306:3306 \
  -v mysql-data:/var/lib/mysql \
  -d mysql:8.0

# 使用配置文件
docker run --name mysql-container \
  -e MYSQL_ROOT_PASSWORD=rootpassword \
  -p 3306:3306 \
  -v /path/to/my.cnf:/etc/mysql/my.cnf \
  -v mysql-data:/var/lib/mysql \
  -d mysql:8.0
```

### 9.2 Docker Compose 配置

```yaml
# docker-compose.yml
version: '3.8'
services:
  mysql:
    image: mysql:8.0
    container_name: mysql-container
    restart: always
    environment:
      MYSQL_ROOT_PASSWORD: rootpassword
      MYSQL_DATABASE: myapp
      MYSQL_USER: appuser
      MYSQL_PASSWORD: apppassword
    ports:
      - "3306:3306"
    volumes:
      - mysql-data:/var/lib/mysql
      - ./mysql-config:/etc/mysql/conf.d
      - ./init-scripts:/docker-entrypoint-initdb.d
    networks:
      - app-network

volumes:
  mysql-data:

networks:
  app-network:
    driver: bridge
```

### 9.3 容器管理命令

```bash
# 启动容器
docker start mysql-container

# 停止容器
docker stop mysql-container

# 重启容器
docker restart mysql-container

# 查看容器状态
docker ps
docker ps -a

# 查看容器日志
docker logs mysql-container
docker logs -f mysql-container  # 实时查看

# 进入容器
docker exec -it mysql-container bash
docker exec -it mysql-container mysql -u root -p

# 删除容器
docker rm mysql-container
docker rm -f mysql-container  # 强制删除
```

### 9.4 数据备份和恢复（Docker）

```bash
# 备份数据库
docker exec mysql-container mysqldump -u root -prootpassword myapp > backup.sql

# 恢复数据库
docker exec -i mysql-container mysql -u root -prootpassword myapp < backup.sql

# 备份数据卷
docker run --rm -v mysql-data:/data -v $(pwd):/backup alpine tar czf /backup/mysql-backup.tar.gz -C /data .

# 恢复数据卷
docker run --rm -v mysql-data:/data -v $(pwd):/backup alpine tar xzf /backup/mysql-backup.tar.gz -C /data
```

## 10. 性能优化和监控

### 10.1 查看系统状态

```sql
-- 查看MySQL版本
SELECT VERSION();

-- 查看系统变量
SHOW VARIABLES LIKE 'innodb%';
SHOW VARIABLES LIKE 'max_connections';

-- 查看系统状态
SHOW STATUS LIKE 'Threads_connected';
SHOW STATUS LIKE 'Slow_queries';

-- 查看进程列表
SHOW PROCESSLIST;
SHOW FULL PROCESSLIST;

-- 查看InnoDB状态
SHOW ENGINE INNODB STATUS;
```

### 10.2 慢查询分析

```sql
-- 开启慢查询日志
SET GLOBAL slow_query_log = 'ON';
SET GLOBAL long_query_time = 2;

-- 查看慢查询配置
SHOW VARIABLES LIKE 'slow_query%';
SHOW VARIABLES LIKE 'long_query_time';
```

### 10.3 常用配置优化

```ini
# my.cnf 配置示例
[mysqld]
# 基本配置
port = 3306
socket = /var/lib/mysql/mysql.sock

# 字符集
character-set-server = utf8mb4
collation-server = utf8mb4_unicode_ci

# 连接配置
max_connections = 200
max_connect_errors = 10

# 缓存配置
innodb_buffer_pool_size = 1G
query_cache_size = 64M
query_cache_type = 1

# 日志配置
log-error = /var/log/mysql/error.log
slow_query_log = 1
slow_query_log_file = /var/log/mysql/slow.log
long_query_time = 2

# InnoDB配置
innodb_file_per_table = 1
innodb_flush_log_at_trx_commit = 2
innodb_log_buffer_size = 16M
innodb_log_file_size = 256M
```

## 11. 常用工具和技巧

### 11.1 MySQL客户端工具

```bash
# 使用mysql客户端
mysql -u username -p -h hostname -P port database_name

# 执行SQL文件
mysql -u username -p database_name < script.sql

# 执行单条SQL命令
mysql -u username -p -e "SELECT * FROM users LIMIT 5;"

# 输出为CSV格式
mysql -u username -p -e "SELECT * FROM users;" --batch --raw | sed 's/\t/,/g' > output.csv
```

### 11.2 实用技巧

```sql
-- 查看表大小
SELECT 
    table_name,
    ROUND(((data_length + index_length) / 1024 / 1024), 2) AS 'Size (MB)'
FROM information_schema.tables 
WHERE table_schema = 'database_name'
ORDER BY (data_length + index_length) DESC;

-- 查看数据库大小
SELECT 
    table_schema AS 'Database',
    ROUND(SUM(data_length + index_length) / 1024 / 1024, 2) AS 'Size (MB)'
FROM information_schema.tables 
GROUP BY table_schema;

-- 重置自增ID
ALTER TABLE table_name AUTO_INCREMENT = 1;

-- 查看表的自增值
SHOW TABLE STATUS LIKE 'table_name';

-- 修复表
REPAIR TABLE table_name;

-- 优化表
OPTIMIZE TABLE table_name;

-- 检查表
CHECK TABLE table_name;
```

## 12. 注意事项

1. **安全第一**：始终使用强密码，合理分配用户权限
2. **定期备份**：制定备份策略，确保数据安全
3. **性能监控**：定期检查慢查询，优化数据库性能
4. **容器化部署**：使用Docker简化MySQL的部署和管理
5. **版本管理**：保持MySQL版本更新，关注安全补丁