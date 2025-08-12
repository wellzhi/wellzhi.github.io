+++
date = '2010-08-07'
draft = false
title = 'Docker Compose 常用命令'
comments = true
tags = ["Docker", "Compose", "命令"]
+++

以下是 **Docker Compose** 的常用命令分类整理，结合功能场景归纳为 **5 类核心操作**，并附关键参数说明与示例，方便快速查阅：

---

### 🚀 **一、核心生命周期管理**

| 命令 | 功能 | 常用参数 | 示例 |
|:-----|:-----|:---------|:-----|
| `docker compose up` | 构建镜像、创建并启动所有服务 | `-d` 后台运行<br>`--build` 强制重建镜像<br>`--force-recreate` 强制重建容器<br>`--scale SERVICE=NUM` 扩缩容 | `docker compose up -d --build` |
| `docker compose down` | 停止并删除容器、网络、卷 | `-v` 删除数据卷<br>`--rmi all` 删除所有镜像<br>`--remove-orphans` 清理未定义容器 | `docker compose down -v --remove-orphans` |

---

### ⚙️ **二、服务状态控制**

| 命令 | 功能 | 常用参数 | 示例 |
|:-----|:-----|:---------|:-----|
| `docker compose start` | 启动已存在的容器 | `<服务名>` 指定服务 | `docker compose start web` |
| `docker compose stop` | 停止运行中的容器（保留容器） | `-t <秒>` 超时时间 | `docker compose stop -t 5` |
| `docker compose restart` | 重启服务（先停后启） | `-t <秒>` 超时时间 | `docker compose restart api` |
| `docker compose pause`<br>`docker compose unpause` | 暂停/恢复服务运行 | - | `docker compose pause db` |

---

### 🔍 **三、日志与调试**

| 命令 | 功能 | 常用参数 | 示例 |
|:-----|:-----|:---------|:-----|
| `docker compose logs` | 查看服务日志 | `-f` 实时跟踪<br>`--tail <行数>` 显示最后N行<br>`<服务名>` 指定服务 | `docker compose logs -f --tail 100 web` |
| `docker compose exec` | 在运行中的容器内执行命令 | `-d` 后台执行<br>`--user` 指定用户<br>`-T` 禁用TTY | `docker compose exec -u root db psql -U postgres` |
| `docker compose ps` | 列出所有服务的运行状态 | `-a` 显示所有容器 | `docker compose ps -a` |
| `docker compose port` | 查看容器端口映射的宿主机端口 | `--protocol=tcp/udp` 指定协议 | `docker compose port web 80` |

---

### 🛠️ **四、构建与配置**

| 命令 | 功能 | 常用参数 | 示例 |
|:-----|:-----|:---------|:-----|
| `docker compose build` | 构建或重新构建服务镜像 | `--no-cache` 禁用缓存<br>`--pull` 拉取最新基础镜像 | `docker compose build --no-cache api` |
| `docker compose pull` | 拉取服务依赖的镜像 | `--ignore-pull-failures` 忽略错误 | `docker compose pull --ignore-pull-failures` |
| `docker compose config` | 验证并输出最终配置（调试YAML文件） | `-q` 仅验证<br>`--services` 列出服务 | `docker compose config --services` |
| `docker compose run` | 启动新容器执行一次性命令（如数据库迁移） | `--rm` 执行后删除容器<br>`-e KEY=VAL` 设置环境变量 | `docker compose run --rm web python manage.py migrate` |

---

### 🧩 **五、其他实用命令**

| 命令 | 功能 | 说明 |
|:-----|:-----|:-----|
| `docker compose scale` | 设置服务副本数（旧版） | 新版推荐用 `up --scale` |
| `docker compose kill` | 强制终止服务（发送SIGKILL） | `-s SIGNAL` 指定信号，如 `SIGINT` |
| `docker compose images` | 列出服务使用的镜像 | - |
| `docker compose push` | 推送服务镜像到仓库 | `--ignore-push-failures` 忽略推送错误 |

---

### 📌 **速查表：高频命令组合**

| 场景 | 命令示例 |
|:-----|:---------|
| 启动服务（后台运行） | `docker compose up -d` |
| 停止并清理所有资源 | `docker compose down -v --rmi all` |
| 查看实时日志 | `docker compose logs -f --tail 50 web` |
| 进入容器执行命令 | `docker compose exec web bash` |
| 验证配置文件语法 | `docker compose config -q`（无输出表示正确） |

---

### 💡 **使用技巧**

1. **多环境配置**：用 `-f` 指定多个文件（如开发/生产环境）：

   ```bash
   docker compose -f docker-compose.yml -f docker-compose.prod.yml up -d
   ```

2. **项目命名**：避免资源冲突，用 `-p` 指定项目名：

   ```bash
   docker compose -p my_project up -d
   ```

3. **版本兼容**：

   - `docker compose`（v2）替代旧版 `docker-compose`（v1），命令格式更简洁。

> ⚠️ **提示**：完整命令列表可通过 `docker compose --help` 查看，或使用 `docker compose help <命令>` 获取详细参数说明（如 `docker compose help up`）。