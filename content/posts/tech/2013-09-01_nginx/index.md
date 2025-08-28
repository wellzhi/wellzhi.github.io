---
date: 2013-09-01T00:00:00-08:00
draft: false
params:
  author: wellzhi
title: Nginx 配置
summary: Nginx是一个高性能的HTTP和反向代理服务器，也是一个IMAP/POP3/SMTP服务器。以其高性能、稳定性、丰富的功能集、简单的配置文件和低系统资源消耗而闻名。
tags:
  - nginx
weight: 10
hideMeta: true
tocOpen: true
---


## 1. Nginx 简介

Nginx是一个高性能的HTTP和反向代理服务器，也是一个IMAP/POP3/SMTP服务器。以其高性能、稳定性、丰富的功能集、简单的配置文件和低系统资源消耗而闻名。

### 1.1 主要特性

- 高并发处理能力
- 低内存消耗
- 高可靠性
- 热部署
- 负载均衡
- 反向代理
- 静态文件服务
- SSL/TLS支持

## 2. 安装 Nginx

### 2.1 Ubuntu/Debian 系统

```bash
# 更新包列表
sudo apt update

# 安装nginx
sudo apt install nginx

# 启动nginx
sudo systemctl start nginx

# 设置开机自启
sudo systemctl enable nginx
```

### 2.2 CentOS/RHEL 系统

```bash
# 安装EPEL仓库
sudo yum install epel-release

# 安装nginx
sudo yum install nginx

# 启动nginx
sudo systemctl start nginx

# 设置开机自启
sudo systemctl enable nginx
```

### 2.3 macOS 系统

```bash
# 使用Homebrew安装
brew install nginx

# 启动nginx
brew services start nginx
```

### 2.4 编译安装

```bash
# 下载源码
wget http://nginx.org/download/nginx-1.24.0.tar.gz
tar -zxvf nginx-1.24.0.tar.gz
cd nginx-1.24.0

# 配置编译选项
./configure --prefix=/etc/nginx \
            --sbin-path=/usr/sbin/nginx \
            --conf-path=/etc/nginx/nginx.conf \
            --error-log-path=/var/log/nginx/error.log \
            --http-log-path=/var/log/nginx/access.log \
            --pid-path=/var/run/nginx.pid \
            --lock-path=/var/run/nginx.lock \
            --with-http_ssl_module \
            --with-http_realip_module \
            --with-http_gzip_static_module

# 编译安装
make && sudo make install
```

## 3. Nginx 目录结构

### 3.1 主要目录

```shell
/etc/nginx/                 # 主配置目录
├── nginx.conf             # 主配置文件
├── conf.d/                # 额外配置文件目录
├── sites-available/       # 可用站点配置
├── sites-enabled/         # 启用站点配置
├── snippets/              # 配置片段
└── modules-enabled/       # 启用的模块

/var/log/nginx/            # 日志目录
├── access.log             # 访问日志
└── error.log              # 错误日志

/var/www/html/             # 默认网站根目录
/usr/share/nginx/html/     # 默认页面目录
```

## 4. 基础配置

### 4.1 主配置文件结构

```nginx
# /etc/nginx/nginx.conf

# 全局块
user nginx;
worker_processes auto;
error_log /var/log/nginx/error.log;
pid /run/nginx.pid;

# events块
events {
    worker_connections 1024;
    use epoll;
    multi_accept on;
}

# http块
http {
    # 基础设置
    include       /etc/nginx/mime.types;
    default_type  application/octet-stream;
    
    # 日志格式
    log_format main '$remote_addr - $remote_user [$time_local] "$request" '
                    '$status $body_bytes_sent "$http_referer" '
                    '"$http_user_agent" "$http_x_forwarded_for"';
    
    access_log /var/log/nginx/access.log main;
    
    # 性能优化
    sendfile on;
    tcp_nopush on;
    tcp_nodelay on;
    keepalive_timeout 65;
    types_hash_max_size 2048;
    
    # Gzip压缩
    gzip on;
    gzip_vary on;
    gzip_min_length 1024;
    gzip_types text/plain text/css application/json application/javascript text/xml application/xml;
    
    # 包含其他配置文件
    include /etc/nginx/conf.d/*.conf;
    include /etc/nginx/sites-enabled/*;
}
```

### 4.2 虚拟主机配置

```nginx
# /etc/nginx/sites-available/example.com
server {
    listen 80;
    listen [::]:80;
    server_name example.com www.example.com;
    root /var/www/example.com/html;
    index index.html index.htm index.nginx-debian.html;
    
    # 访问日志
    access_log /var/log/nginx/example.com.access.log;
    error_log /var/log/nginx/example.com.error.log;
    
    # 静态文件处理
    location / {
        try_files $uri $uri/ =404;
    }
    
    # 静态资源缓存
    location ~* \.(jpg|jpeg|png|gif|ico|css|js)$ {
        expires 1y;
        add_header Cache-Control "public, immutable";
    }
    
    # 安全设置
    location ~ /\. {
        deny all;
    }
}
```

## 5. 反向代理配置

### 5.1 基本反向代理

```nginx
server {
    listen 80;
    server_name api.example.com;
    
    location / {
        proxy_pass http://127.0.0.1:3000;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
        
        # 超时设置
        proxy_connect_timeout 30s;
        proxy_send_timeout 30s;
        proxy_read_timeout 30s;
    }
}
```

### 5.2 负载均衡

```nginx
# 定义上游服务器组
upstream backend {
    # 轮询（默认）
    server 192.168.1.10:8080;
    server 192.168.1.11:8080;
    server 192.168.1.12:8080;
    
    # 权重负载均衡
    # server 192.168.1.10:8080 weight=3;
    # server 192.168.1.11:8080 weight=2;
    # server 192.168.1.12:8080 weight=1;
    
    # IP哈希
    # ip_hash;
    
    # 最少连接
    # least_conn;
    
    # 健康检查
    server 192.168.1.10:8080 max_fails=3 fail_timeout=30s;
}

server {
    listen 80;
    server_name app.example.com;
    
    location / {
        proxy_pass http://backend;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    }
}
```

## 6. SSL/HTTPS 配置

### 6.1 SSL 证书配置

```nginx
server {
    listen 443 ssl http2;
    listen [::]:443 ssl http2;
    server_name example.com www.example.com;
    
    # SSL证书配置
    ssl_certificate /etc/ssl/certs/example.com.crt;
    ssl_certificate_key /etc/ssl/private/example.com.key;
    
    # SSL安全配置
    ssl_protocols TLSv1.2 TLSv1.3;
    ssl_ciphers ECDHE-RSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384;
    ssl_prefer_server_ciphers off;
    ssl_session_cache shared:SSL:10m;
    ssl_session_timeout 10m;
    
    # HSTS
    add_header Strict-Transport-Security "max-age=31536000; includeSubDomains" always;
    
    # 其他安全头
    add_header X-Frame-Options DENY;
    add_header X-Content-Type-Options nosniff;
    add_header X-XSS-Protection "1; mode=block";
    
    root /var/www/example.com/html;
    index index.html;
    
    location / {
        try_files $uri $uri/ =404;
    }
}

# HTTP重定向到HTTPS
server {
    listen 80;
    listen [::]:80;
    server_name example.com www.example.com;
    return 301 https://$server_name$request_uri;
}
```

### 6.2 Let's Encrypt 免费证书

```bash
# 安装certbot
sudo apt install certbot python3-certbot-nginx

# 获取证书
sudo certbot --nginx -d example.com -d www.example.com

# 自动续期
sudo crontab -e
# 添加以下行
0 12 * * * /usr/bin/certbot renew --quiet
```

## 7. 性能优化

### 7.1 工作进程优化

```nginx
# 设置工作进程数（通常等于CPU核心数）
worker_processes auto;

# 绑定工作进程到CPU核心
worker_cpu_affinity auto;

# 工作进程连接数
events {
    worker_connections 2048;
    use epoll;
    multi_accept on;
}
```

### 7.2 缓存配置

```nginx
http {
    # 文件缓存
    open_file_cache max=1000 inactive=20s;
    open_file_cache_valid 30s;
    open_file_cache_min_uses 2;
    open_file_cache_errors on;
    
    # 代理缓存
    proxy_cache_path /var/cache/nginx levels=1:2 keys_zone=my_cache:10m max_size=10g 
    inactive=60m use_temp_path=off;
    
    server {
        location / {
            proxy_cache my_cache;
            proxy_cache_valid 200 302 10m;
            proxy_cache_valid 404 1m;
            proxy_cache_use_stale error timeout updating http_500 http_502 http_503 http_504;
            proxy_cache_lock on;
            proxy_pass http://backend;
        }
    }
}
```

### 7.3 压缩优化

```nginx
# Gzip压缩
gzip on;
gzip_vary on;
gzip_min_length 1024;
gzip_comp_level 6;
gzip_types
    text/plain
    text/css
    text/xml
    text/javascript
    application/json
    application/javascript
    application/xml+rss
    application/atom+xml
    image/svg+xml;

# Brotli压缩（需要安装模块）
# brotli on;
# brotli_comp_level 6;
# brotli_types text/plain text/css application/json application/javascript text/xml application/xml;
```

## 8. 安全配置

### 8.1 基础安全设置

```nginx
# 隐藏Nginx版本
server_tokens off;

# 限制请求大小
client_max_body_size 10M;

# 限制请求速率
http {
    limit_req_zone $binary_remote_addr zone=login:10m rate=1r/s;
    limit_req_zone $binary_remote_addr zone=api:10m rate=10r/s;
    
    server {
        # 登录接口限流
        location /login {
            limit_req zone=login burst=5 nodelay;
            proxy_pass http://backend;
        }
        
        # API接口限流
        location /api/ {
            limit_req zone=api burst=20 nodelay;
            proxy_pass http://backend;
        }
    }
}
```

### 8.2 防止恶意访问

```nginx
# 阻止特定User-Agent
if ($http_user_agent ~* (bot|crawler|spider)) {
    return 403;
}

# 阻止特定IP
deny 192.168.1.100;
allow 192.168.1.0/24;
deny all;

# 防止SQL注入和XSS
if ($args ~* "(union|select|insert|delete|update|drop|script|alert)") {
    return 403;
}

# 防止目录遍历
location ~ /\. {
    deny all;
    access_log off;
    log_not_found off;
}

# 防止访问敏感文件
location ~* \.(htaccess|htpasswd|ini|log|sh|sql|conf)$ {
    deny all;
}
```

## 9. 日志管理

### 9.1 自定义日志格式

```nginx
http {
    # 详细日志格式
    log_format detailed '$remote_addr - $remote_user [$time_local] '
                       '"$request" $status $body_bytes_sent '
                       '"$http_referer" "$http_user_agent" '
                       '$request_time $upstream_response_time '
                       '$pipe $upstream_cache_status';

    # JSON格式日志
    log_format json escape=json
        '{'
            '"time_local":"$time_local",'
            '"remote_addr":"$remote_addr",'
            '"request":"$request",'
            '"status":$status,'
            '"body_bytes_sent":$body_bytes_sent,'
            '"http_referer":"$http_referer",'
            '"http_user_agent":"$http_user_agent",'
            '"request_time":$request_time'
        '}';

    access_log /var/log/nginx/access.log detailed;
}
```

### 9.2 日志轮转

```bash
# /etc/logrotate.d/nginx
/var/log/nginx/*.log {
    daily
    missingok
    rotate 52
    compress
    delaycompress
    notifempty
    create 644 nginx adm
    postrotate
        if [ -f /var/run/nginx.pid ]; then
            kill -USR1 `cat /var/run/nginx.pid`
        fi
    endscript
}
```

## 10. 常用命令

### 10.1 服务管理

```bash
# 启动nginx
sudo systemctl start nginx
# 停止nginx
sudo systemctl stop nginx
# 重启nginx
sudo systemctl restart nginx
# 重新加载配置
sudo systemctl reload nginx
# 查看状态
sudo systemctl status nginx
# 测试配置文件
sudo nginx -t
# 查看配置文件路径
nginx -T
# 查看版本信息
nginx -v
nginx -V
```

### 10.2 信号控制

```bash
# 优雅停止
sudo nginx -s quit
# 快速停止
sudo nginx -s stop
# 重新加载配置
sudo nginx -s reload
# 重新打开日志文件
sudo nginx -s reopen
```

## 11. 故障排查

### 11.1 常见错误及解决方案

#### 11.1.1 403 Forbidden

```bash
# 检查文件权限
ls -la /var/www/html/

# 修改权限
sudo chown -R nginx:nginx /var/www/html/
sudo chmod -R 755 /var/www/html/

# 检查SELinux（CentOS/RHEL）
sudo setsebool -P httpd_can_network_connect 1
```

#### 11.1.2 502 Bad Gateway

```bash
# 检查后端服务是否运行
netstat -tlnp | grep :3000

# 检查防火墙
sudo ufw status
sudo firewall-cmd --list-all

# 检查nginx错误日志
sudo tail -f /var/log/nginx/error.log
```

#### 11.1.3 504 Gateway Timeout

```nginx
# 增加超时时间
location / {
    proxy_connect_timeout 60s;
    proxy_send_timeout 60s;
    proxy_read_timeout 60s;
    proxy_pass http://backend;
}
```

### 11.2 性能监控

```bash
# 查看连接状态
ss -tuln | grep :80

# 监控nginx进程
top -p $(pgrep nginx)

# 查看访问统计
awk '{print $1}' /var/log/nginx/access.log | sort | uniq -c | sort -nr | head -10

# 分析响应时间
awk '{print $NF}' /var/log/nginx/access.log | sort -n | tail -10
```

## 12. 高级配置

### 12.1 动态模块加载

```nginx
# 加载模块
load_module modules/ngx_http_geoip_module.so;

http {
    # 使用GeoIP
    geoip_country /usr/share/GeoIP/GeoIP.dat;
    map $geoip_country_code $allowed_country {
        default no;
        CN yes;
        US yes;
    }
    server {
        if ($allowed_country = no) {
            return 403;
        }
    }
}
```

### 12.2 Lua脚本集成

```nginx
# 需要安装lua模块
location /api {
    access_by_lua_block {
        local headers = ngx.req.get_headers()
        if not headers["authorization"] then
            ngx.status = 401
            ngx.say("Unauthorized")
            ngx.exit(401)
        end
    }
    
    proxy_pass http://backend;
}
```

### 12.3 实时配置更新

```nginx
# 使用nginx-plus的API
location /api {
    api write=on;
    allow 127.0.0.1;
    deny all;
}

# 动态添加上游服务器
# curl -X POST -d '{"server":"192.168.1.100:8080"}' \
#      http://localhost/api/6/http/upstreams/backend/servers
```

## 13. 最佳实践

### 13.1 配置文件组织

```nginx
/etc/nginx/
├── nginx.conf             # 主配置文件
├── conf.d/
│   ├── gzip.conf          # 压缩配置
│   ├── security.conf      # 安全配置
│   └── ssl.conf           # SSL配置
├── sites-available/
│   ├── example.com        # 站点配置
│   └── api.example.com    # API配置
└── sites-enabled/         # 启用的站点（软链接）

```

### 13.2 配置模板

```nginx
# /etc/nginx/conf.d/security.conf
# 安全头配置
add_header X-Frame-Options DENY always;
add_header X-Content-Type-Options nosniff always;
add_header X-XSS-Protection "1; mode=block" always;
add_header Referrer-Policy "strict-origin-when-cross-origin" always;

# 隐藏服务器信息
server_tokens off;
more_clear_headers Server;
```

### 13.3 监控和告警

```bash
#!/bin/bash
# nginx_monitor.sh
# 检查nginx状态
if ! systemctl is-active --quiet nginx; then
    echo "Nginx is down!" | mail -s "Nginx Alert" admin@example.com
fi

# 检查错误日志
error_count=$(tail -100 /var/log/nginx/error.log | grep "$(date '+%Y/%m/%d')" | wc -l)
if [ $error_count -gt 10 ]; then
    echo "High error rate: $error_count errors today" | mail -s "Nginx Error Alert" admin@example.com
fi
```