+++
date = '2020-01-01T00:00:00+08:00'
draft = false
title = 'Docker常用命令'
comments = false
tags = ["Docker"]
tocOpen = true
summary = "Docker 是一个开源的容器化平台，它允许开发者将应用程序及其依赖项打包到轻量级、可移植的容器中。通过使用容器技术，Docker 实现了应用程序的快速部署、扩展和管理，解决了传统虚拟化技术资源消耗大、启动慢的问题。本文详细介绍了Docker的常用命令，包括镜像管理、容器操作、网络配置、数据卷管理等核心功能，帮助开发者快速掌握Docker的使用技巧和最佳实践。"
+++

Docker 是一个开源的容器化平台，它允许开发者将应用程序及其依赖项打包到轻量级、可移植的容器中。

## 1. Docker 基础命令

### 1.1 查看 Docker 版本和信息

```bash
# 查看 Docker 版本
docker --version
docker version

# 查看 Docker 系统信息
docker info

# 查看 Docker 帮助
docker --help
docker <command> --help
```

### 1.2 登录和登出 Docker Hub

```bash
# 登录 Docker Hub
docker login

# 登录指定仓库
docker login registry.example.com

# 登出
docker logout
```

## 2. 镜像管理命令

### 2.1 搜索和拉取镜像

```bash
# 搜索镜像
docker search nginx
docker search --limit 10 nginx

# 拉取镜像
docker pull nginx
docker pull nginx:1.20
docker pull nginx:latest

# 从指定仓库拉取
docker pull registry.example.com/nginx:latest
```

### 2.2 查看和管理镜像

```bash
# 列出本地镜像
docker images
docker image ls

# 查看镜像详细信息
docker inspect nginx:latest

# 查看镜像历史
docker history nginx:latest

# 删除镜像
docker rmi nginx:latest
docker image rm nginx:latest

# 强制删除镜像
docker rmi -f nginx:latest

# 删除所有未使用的镜像
docker image prune
docker image prune -a  # 删除所有未被容器使用的镜像
```

### 2.3 构建镜像

```bash
# 从 Dockerfile 构建镜像
docker build -t myapp:latest .
docker build -t myapp:v1.0 -f Dockerfile.prod .

# 构建时传递参数
docker build --build-arg VERSION=1.0 -t myapp:latest .

# 不使用缓存构建
docker build --no-cache -t myapp:latest .
```

### 2.4 标记和推送镜像

```bash
# 给镜像打标签
docker tag myapp:latest username/myapp:latest
docker tag myapp:latest registry.example.com/myapp:v1.0

# 推送镜像到仓库
docker push username/myapp:latest
docker push registry.example.com/myapp:v1.0
```

### 2.5 保存和加载镜像

```bash
# 将镜像保存为 tar 文件
docker save -o myapp.tar myapp:latest
docker save myapp:latest > myapp.tar

# 从 tar 文件加载镜像
docker load -i myapp.tar
docker load < myapp.tar

# 导出容器为镜像
docker export container_name > container.tar

# 从导出文件创建镜像
docker import container.tar myapp:imported
```

## 3. 容器管理命令

### 3.1 运行容器

```bash
# 基本运行
docker run nginx

# 后台运行
docker run -d nginx

# 交互式运行
docker run -it ubuntu:20.04 /bin/bash

# 指定容器名称
docker run --name my-nginx -d nginx

# 端口映射
docker run -d -p 8080:80 nginx
docker run -d -p 127.0.0.1:8080:80 nginx

# 环境变量
docker run -d -e MYSQL_ROOT_PASSWORD=password mysql:8.0

# 挂载数据卷
docker run -d -v /host/path:/container/path nginx
docker run -d -v my-volume:/var/lib/mysql mysql:8.0

# 网络设置
docker run -d --network my-network nginx

# 资源限制
docker run -d --memory=512m --cpus=1.0 nginx

# 自动重启
docker run -d --restart=always nginx
docker run -d --restart=unless-stopped nginx

# 完整示例
docker run -d \\
  --name my-app \\
  -p 8080:80 \\
  -e ENV=production \\
  -v /host/data:/app/data \\
  --restart=unless-stopped \\
  --memory=1g \\
  --cpus=2.0 \\
  myapp:latest
```

### 3.2 查看和管理容器

```bash
# 列出运行中的容器
docker ps

# 列出所有容器（包括停止的）
docker ps -a

# 查看容器详细信息
docker inspect container_name

# 查看容器日志
docker logs container_name
docker logs -f container_name  # 实时查看
docker logs --tail 100 container_name  # 查看最后100行
docker logs --since 2023-01-01 container_name  # 查看指定时间后的日志

# 查看容器进程
docker top container_name

# 查看容器资源使用情况
docker stats
docker stats container_name
```

### 3.3 容器操作

```bash
# 启动容器
docker start container_name

# 停止容器
docker stop container_name
docker stop -t 30 container_name  # 30秒后强制停止

# 重启容器
docker restart container_name

# 暂停和恢复容器
docker pause container_name
docker unpause container_name

# 强制杀死容器
docker kill container_name
docker kill -s SIGTERM container_name

# 删除容器
docker rm container_name
docker rm -f container_name  # 强制删除运行中的容器

# 删除所有停止的容器
docker container prune
```

### 3.4 进入容器

```bash
# 进入运行中的容器
docker exec -it container_name /bin/bash
docker exec -it container_name /bin/sh

# 以指定用户身份进入
docker exec -it -u root container_name /bin/bash

# 在容器中执行命令
docker exec container_name ls -la
docker exec -d container_name touch /tmp/test.txt

# 附加到容器（不推荐）
docker attach container_name
```

### 3.5 容器文件操作

```bash
# 从容器复制文件到主机
docker cp container_name:/path/to/file /host/path/

# 从主机复制文件到容器
docker cp /host/path/file container_name:/path/to/

# 复制目录
docker cp container_name:/app/logs/ ./logs/
```

## 4. 网络管理命令

### 4.1 网络基础操作

```bash
# 列出网络
docker network ls

# 查看网络详细信息
docker network inspect bridge

# 创建网络
docker network create my-network
docker network create --driver bridge my-bridge
docker network create --driver overlay my-overlay

# 创建自定义网络
docker network create \\
  --driver bridge \\
  --subnet=172.20.0.0/16 \\
  --ip-range=172.20.240.0/20 \\
  --gateway=172.20.0.1 \\
  my-custom-network

# 删除网络
docker network rm my-network

# 清理未使用的网络
docker network prune
```

### 4.2 容器网络连接

```bash
# 将容器连接到网络
docker network connect my-network container_name

# 断开容器网络连接
docker network disconnect my-network container_name

# 运行容器时指定网络
docker run -d --network my-network nginx

# 指定容器在网络中的IP
docker run -d --network my-network --ip 172.20.0.10 nginx
```

## 5. 数据卷管理命令

### 5.1 数据卷操作

```bash
# 列出数据卷
docker volume ls

# 创建数据卷
docker volume create my-volume

# 查看数据卷详细信息
docker volume inspect my-volume

# 删除数据卷
docker volume rm my-volume

# 清理未使用的数据卷
docker volume prune
```

### 5.2 使用数据卷

```bash
# 挂载命名数据卷
docker run -d -v my-volume:/var/lib/mysql mysql:8.0

# 挂载主机目录
docker run -d -v /host/path:/container/path nginx

# 只读挂载
docker run -d -v /host/path:/container/path:ro nginx

# 使用 --mount 语法（推荐）
docker run -d \\
  --mount type=volume,source=my-volume,target=/var/lib/mysql \\
  mysql:8.0

docker run -d \\
  --mount type=bind,source=/host/path,target=/container/path,readonly \\
  nginx
```

## 6. Docker Compose 命令

### 6.1 基础操作

```bash
# 启动服务
docker-compose up
docker-compose up -d  # 后台运行

# 停止服务
docker-compose down
docker-compose down -v  # 同时删除数据卷

# 查看服务状态
docker-compose ps

# 查看日志
docker-compose logs
docker-compose logs -f service_name

# 重启服务
docker-compose restart
docker-compose restart service_name

# 构建镜像
docker-compose build
docker-compose build --no-cache

# 拉取镜像
docker-compose pull

# 执行命令
docker-compose exec service_name /bin/bash

# 运行一次性命令
docker-compose run service_name command

# 扩展服务
docker-compose up --scale web=3
```

### 6.2 配置和验证

```bash
# 验证配置文件
docker-compose config

# 查看配置
docker-compose config --services
docker-compose config --volumes

# 使用指定配置文件
docker-compose -f docker-compose.prod.yml up

# 使用多个配置文件
docker-compose -f docker-compose.yml -f docker-compose.override.yml up
```

## 7. 系统信息和清理命令

### 7.1 系统信息

```bash
# 查看 Docker 系统信息
docker system info

# 查看磁盘使用情况
docker system df
docker system df -v  # 详细信息

# 查看系统事件
docker system events
docker system events --since 2023-01-01
```

### 7.2 系统清理

```bash
# 清理系统（删除未使用的容器、网络、镜像等）
docker system prune
docker system prune -a  # 包括未使用的镜像
docker system prune -f  # 强制清理，不询问

# 分别清理不同资源
docker container prune  # 清理停止的容器
docker image prune      # 清理未使用的镜像
docker network prune    # 清理未使用的网络
docker volume prune     # 清理未使用的数据卷

# 清理所有资源
docker system prune -a --volumes
```

## 8. 实用技巧和最佳实践

### 8.1 常用组合命令

```shell
# 停止所有容器
docker stop $(docker ps -q)

# 删除所有容器
docker rm $(docker ps -aq)

# 删除所有镜像
docker rmi $(docker images -q)

# 删除悬空镜像
docker rmi $(docker images -f \"dangling=true\" -q)

# 查看容器IP地址
docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' container_name

# 查看容器端口映射
docker port container_name

# 实时监控容器资源使用
watch docker stats
```

### 8.2 Dockerfile 最佳实践

```dockerfile
# 使用官方基础镜像
FROM node:16-alpine

# 设置工作目录
WORKDIR /app

# 复制依赖文件并安装（利用缓存）
COPY package*.json ./
RUN npm ci --only=production

# 复制应用代码
COPY . .

# 创建非root用户
RUN addgroup -g 1001 -S nodejs
RUN adduser -S nextjs -u 1001
USER nextjs

# 暴露端口
EXPOSE 3000

# 健康检查
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \\
  CMD curl -f http://localhost:3000/health || exit 1

# 启动命令
CMD [\"npm\", \"start\"]
```

### 8.3 安全建议


1. **不要以 root 用户运行容器**
```bash
docker run -u 1000:1000 myapp
```

2. **限制容器资源**
```bash
docker run --memory=512m --cpus=1.0 myapp
```

3. **使用只读文件系统**
```bash
docker run --read-only --tmpfs /tmp myapp
```

4. **扫描镜像漏洞**
```bash
docker scan myapp:latest
```

### 8.4 调试技巧

```bash
# 查看容器启动失败原因
docker logs container_name

# 进入容器调试
docker run -it --entrypoint /bin/bash myapp:latest

# 查看容器文件系统变化
docker diff container_name

# 创建容器但不启动
docker create --name debug-container myapp:latest

# 从失败的容器创建镜像
docker commit container_name debug-image:latest
```

### 8.5 性能优化

```bash
# 使用多阶段构建减小镜像大小
# 在 Dockerfile 中
FROM node:16 AS builder
WORKDIR /app
COPY package*.json ./
RUN npm install
COPY . .
RUN npm run build

FROM node:16-alpine
WORKDIR /app
COPY --from=builder /app/dist ./dist
COPY --from=builder /app/node_modules ./node_modules
CMD [\"node\", \"dist/index.js\"]

# 使用 .dockerignore 文件
echo \"node_modules\\n.git\\n*.log\" > .dockerignore
```

## 9. 注意事项

- 始终使用具体的镜像标签而不是 `latest`
- 定期清理未使用的资源以节省磁盘空间
- 使用 Docker Compose 管理多容器应用
- 遵循安全最佳实践，不要以 root 用户运行容器
- 利用多阶段构建优化镜像大小