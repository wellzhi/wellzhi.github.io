---
date: 2024-01-01T04:14:54-08:00
draft: false
params:
  author: wellzhi
title: "Kubernetes使用指南"
weight: 10
hideMeta: true
tags:
  - Kubernetes
  - 容器化
  - 微服务
  - 分布式系统
  - 云原生
---

<!-- # Kubernetes使用指南：从入门到实践

## 目录

1. [Kubernetes简介](#1-kubernetes简介)
2. [核心概念与架构](#2-核心概念与架构)
3. [环境搭建与安装](#3-环境搭建与安装)
4. [基础资源对象](#4-基础资源对象)
5. [工作负载管理](#5-工作负载管理)
6. [服务发现与负载均衡](#6-服务发现与负载均衡)
7. [存储管理](#7-存储管理)
8. [配置管理](#8-配置管理)
9. [网络管理](#9-网络管理)
10. [安全管理](#10-安全管理)
11. [监控与日志](#11-监控与日志)
12. [高级特性](#12-高级特性)
13. [故障排查](#13-故障排查)
14. [最佳实践](#14-最佳实践)
15. [生产环境部署](#15-生产环境部署) -->

## 1. Kubernetes简介 

### 1.1 什么是Kubernetes

Kubernetes（简称K8s）是一个开源的容器编排平台，用于自动化部署、扩展和管理容器化应用程序。它最初由Google开发，现在由Cloud Native Computing Foundation（CNCF）维护。

### 1.2 核心特性

- **自动化部署和回滚**：支持应用的自动化部署和版本回滚
- **服务发现和负载均衡**：自动分配IP地址和DNS名称，并在多个容器间分配负载
- **存储编排**：自动挂载存储系统，包括本地存储、公有云存储等
- **自我修复**：重启失败的容器，替换和重新调度节点上的容器
- **密钥和配置管理**：管理敏感信息和应用配置
- **水平扩展**：根据CPU使用率或其他指标自动扩展应用

### 1.3 使用场景

- 微服务架构部署
- 容器化应用管理
- 多云和混合云部署
- DevOps和CI/CD流水线
- 大规模应用编排

## 2. 核心概念与架构

### 2.1 集群架构

Kubernetes集群由以下组件组成：

#### 2.1.1 Master节点

- **API Server**：集群的统一入口，提供RESTful API
- **etcd**：分布式键值存储，保存集群状态数据
- **Controller Manager**：运行控制器进程，管理集群状态
- **Scheduler**：负责Pod的调度决策

#### 2.1.2 Worker节点

- **kubelet**：节点代理，管理Pod生命周期
- **kube-proxy**：网络代理，实现服务的网络规则
- **Container Runtime**：容器运行时（如Docker、containerd）

### 2.2 核心对象

#### 2.2.1 Pod

最小的部署单元，包含一个或多个容器：

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: nginx-pod
  labels:
    app: nginx
spec:
  containers:
  - name: nginx
    image: nginx:1.20
    ports:
    - containerPort: 80
    resources:
      requests:
        memory: "64Mi"
        cpu: "250m"
      limits:
        memory: "128Mi"
        cpu: "500m"
```

#### 2.2.2 Service

为Pod提供稳定的网络访问：

```yaml
apiVersion: v1
kind: Service
metadata:
  name: nginx-service
spec:
  selector:
    app: nginx
  ports:
  - protocol: TCP
    port: 80
    targetPort: 80
  type: ClusterIP
```

#### 2.2.3 Deployment

管理Pod的副本和更新：

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx-deployment
spec:
  replicas: 3
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:1.20
        ports:
        - containerPort: 80
```

## 3. 环境搭建与安装

### 3.1 本地开发环境

#### 3.1.1 使用Minikube

```bash
# 安装Minikube
curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-darwin-amd64
sudo install minikube-darwin-amd64 /usr/local/bin/minikube

# 启动集群
minikube start --driver=docker

# 验证安装
kubectl cluster-info
kubectl get nodes
```

#### 3.1.2 使用Kind

```bash
# 安装Kind
go install sigs.k8s.io/kind@v0.17.0

# 创建集群
kind create cluster --name my-cluster

# 配置kubectl
kubectl cluster-info --context kind-my-cluster
```

### 3.2 生产环境安装

#### 3.2.1 使用kubeadm

```bash
# 安装Docker
sudo apt-get update
sudo apt-get install -y docker.io
sudo systemctl enable docker
sudo systemctl start docker

# 安装kubeadm、kubelet、kubectl
curl -s https://packages.cloud.google.com/apt/doc/apt-key.gpg | sudo apt-key add -
echo "deb https://apt.kubernetes.io/ kubernetes-xenial main" | sudo tee /etc/apt/sources.list.d/kubernetes.list
sudo apt-get update
sudo apt-get install -y kubelet kubeadm kubectl
sudo apt-mark hold kubelet kubeadm kubectl

# 初始化Master节点
sudo kubeadm init --pod-network-cidr=10.244.0.0/16

# 配置kubectl
mkdir -p $HOME/.kube
sudo cp -i /etc/kubernetes/admin.conf $HOME/.kube/config
sudo chown $(id -u):$(id -g) $HOME/.kube/config

# 安装网络插件（Flannel）
kubectl apply -f https://raw.githubusercontent.com/coreos/flannel/master/Documentation/kube-flannel.yml
```

## 4. 基础资源对象

### 4.1 Namespace

命名空间用于资源隔离：

```yaml
apiVersion: v1
kind: Namespace
metadata:
  name: development
  labels:
    env: dev
```

```bash
# 创建命名空间
kubectl create namespace development

# 查看命名空间
kubectl get namespaces

# 在指定命名空间中操作
kubectl get pods -n development
```

### 4.2 Labels和Selectors

标签用于资源分类和选择：

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: web-pod
  labels:
    app: web
    version: v1.0
    environment: production
spec:
  containers:
  - name: web
    image: nginx:1.20
```

```bash
# 基于标签查询
kubectl get pods -l app=web
kubectl get pods -l environment=production,version=v1.0

# 添加标签
kubectl label pods web-pod tier=frontend

# 删除标签
kubectl label pods web-pod tier-
```

### 4.3 Annotations

注解用于存储非标识性元数据：

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: annotated-pod
  annotations:
    description: "This is a web server pod"
    version: "1.0.0"
    maintainer: "team@example.com"
spec:
  containers:
  - name: web
    image: nginx:1.20
```

## 5. 工作负载管理

### 5.1 ReplicaSet

确保指定数量的Pod副本运行：

```yaml
apiVersion: apps/v1
kind: ReplicaSet
metadata:
  name: nginx-replicaset
spec:
  replicas: 3
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:1.20
        ports:
        - containerPort: 80
```

### 5.2 Deployment

提供声明式更新和回滚功能：

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx-deployment
spec:
  replicas: 3
  strategy:
    type: RollingUpdate
    rollingUpdate:
      maxSurge: 1
      maxUnavailable: 1
  selector:
    matchLabels:
      app: nginx
  template:
    metadata:
      labels:
        app: nginx
    spec:
      containers:
      - name: nginx
        image: nginx:1.20
        ports:
        - containerPort: 80
        livenessProbe:
          httpGet:
            path: /
            port: 80
          initialDelaySeconds: 30
          periodSeconds: 10
        readinessProbe:
          httpGet:
            path: /
            port: 80
          initialDelaySeconds: 5
          periodSeconds: 5
```

```bash
# 部署应用
kubectl apply -f nginx-deployment.yaml

# 查看部署状态
kubectl get deployments
kubectl rollout status deployment/nginx-deployment

# 更新镜像
kubectl set image deployment/nginx-deployment nginx=nginx:1.21

# 查看更新历史
kubectl rollout history deployment/nginx-deployment

# 回滚到上一个版本
kubectl rollout undo deployment/nginx-deployment

# 回滚到指定版本
kubectl rollout undo deployment/nginx-deployment --to-revision=2

# 扩缩容
kubectl scale deployment nginx-deployment --replicas=5
```

### 5.3 StatefulSet

无状态服务和有状态服务分别适用于哪些场景？

- 无状态服务适用于不需要数据持久化的场景，例如Nginx和Tomcat。
- 有状态服务适用于需要数据持久化的场景，例如MySQL数据库。Kafka 和ZooKeeper

管理有状态应用：

```yaml
apiVersion: apps/v1
kind: StatefulSet
metadata:
  name: mysql-statefulset
spec:
  serviceName: mysql
  replicas: 3
  selector:
    matchLabels:
      app: mysql
  template:
    metadata:
      labels:
        app: mysql
    spec:
      containers:
      - name: mysql
        image: mysql:8.0
        env:
        - name: MYSQL_ROOT_PASSWORD
          value: "password"
        ports:
        - containerPort: 3306
        volumeMounts:
        - name: mysql-storage
          mountPath: /var/lib/mysql
  volumeClaimTemplates:
  - metadata:
      name: mysql-storage
    spec:
      accessModes: ["ReadWriteOnce"]
      resources:
        requests:
          storage: 10Gi
```

### 5.4 DaemonSet

在每个节点上运行一个Pod副本：

```yaml
apiVersion: apps/v1
kind: DaemonSet
metadata:
  name: fluentd-daemonset
spec:
  selector:
    matchLabels:
      app: fluentd
  template:
    metadata:
      labels:
        app: fluentd
    spec:
      containers:
      - name: fluentd
        image: fluentd:v1.14
        volumeMounts:
        - name: varlog
          mountPath: /var/log
        - name: varlibdockercontainers
          mountPath: /var/lib/docker/containers
          readOnly: true
      volumes:
      - name: varlog
        hostPath:
          path: /var/log
      - name: varlibdockercontainers
        hostPath:
          path: /var/lib/docker/containers
```

### 5.5 Job和CronJob

#### 5.5.1 Job

运行一次性任务：

```yaml
apiVersion: batch/v1
kind: Job
metadata:
  name: pi-calculation
spec:
  template:
    spec:
      containers:
      - name: pi
        image: perl
        command: ["perl", "-Mbignum=bpi", "-wle", "print bpi(2000)"]
      restartPolicy: Never
  backoffLimit: 4
```

#### 5.5.2 CronJob

定时任务：

```yaml
apiVersion: batch/v1
kind: CronJob
metadata:
  name: backup-cronjob
spec:
  schedule: "0 2 * * *"  # 每天凌晨2点执行
  jobTemplate:
    spec:
      template:
        spec:
          containers:
          - name: backup
            image: backup-tool:latest
            command:
            - /bin/sh
            - -c
            - "echo 'Running backup at $(date)'"
          restartPolicy: OnFailure
```

## 6. 服务发现与负载均衡

### 6.1 Service类型

#### 6.1.1 ClusterIP

集群内部访问：

```yaml
apiVersion: v1
kind: Service
metadata:
  name: nginx-clusterip
spec:
  type: ClusterIP
  selector:
    app: nginx
  ports:
  - port: 80
    targetPort: 80
    protocol: TCP
```

#### 6.1.2 NodePort

通过节点端口访问：

```yaml
apiVersion: v1
kind: Service
metadata:
  name: nginx-nodeport
spec:
  type: NodePort
  selector:
    app: nginx
  ports:
  - port: 80
    targetPort: 80
    nodePort: 30080
    protocol: TCP
```

#### 6.1.3 LoadBalancer

云平台负载均衡器：

```yaml
apiVersion: v1
kind: Service
metadata:
  name: nginx-loadbalancer
spec:
  type: LoadBalancer
  selector:
    app: nginx
  ports:
  - port: 80
    targetPort: 80
    protocol: TCP
```

### 6.2 Ingress

 HTTP/HTTPS路由管理：

```yaml
apiVersion: networking.k8s.io/v1
kind: Ingress
metadata:
  name: nginx-ingress
  annotations:
    nginx.ingress.kubernetes.io/rewrite-target: /
spec:
  rules:
  - host: example.com
    http:
      paths:
      - path: /
        pathType: Prefix
        backend:
          service:
            name: nginx-service
            port:
              number: 80
  - host: api.example.com
    http:
      paths:
      - path: /api
        pathType: Prefix
        backend:
          service:
            name: api-service
            port:
              number: 8080
```

### 6.3 Ingress Controller

安装Nginx Ingress Controller：

```bash
# 安装Nginx Ingress Controller
kubectl apply -f https://raw.githubusercontent.com/kubernetes/ingress-nginx/controller-v1.5.1/deploy/static/provider/cloud/deploy.yaml

# 验证安装
kubectl get pods -n ingress-nginx
kubectl get svc -n ingress-nginx
```

## 7. 存储管理

### 7.1 Volume类型

#### 7.1.1 EmptyDir

临时存储：

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: emptydir-pod
spec:
  containers:
  - name: app
    image: nginx
    volumeMounts:
    - name: cache-volume
      mountPath: /cache
  volumes:
  - name: cache-volume
    emptyDir: {}
```

#### 7.1.2 HostPath

主机路径挂载：

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: hostpath-pod
spec:
  containers:
  - name: app
    image: nginx
    volumeMounts:
    - name: host-volume
      mountPath: /host-data
  volumes:
  - name: host-volume
    hostPath:
      path: /data
      type: Directory
```

### 7.2 PersistentVolume和PersistentVolumeClaim

#### 7.2.1 PersistentVolume

```yaml
apiVersion: v1
kind: PersistentVolume
metadata:
  name: pv-example
spec:
  capacity:
    storage: 10Gi
  accessModes:
    - ReadWriteOnce
  persistentVolumeReclaimPolicy: Retain
  storageClassName: manual
  hostPath:
    path: /mnt/data
```

#### 7.2.2 PersistentVolumeClaim

```yaml
apiVersion: v1
kind: PersistentVolumeClaim
metadata:
  name: pvc-example
spec:
  accessModes:
    - ReadWriteOnce
  resources:
    requests:
      storage: 5Gi
  storageClassName: manual
```

#### 7.2.3 使用PVC的Pod

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: pvc-pod
spec:
  containers:
  - name: app
    image: nginx
    volumeMounts:
    - name: storage
      mountPath: /usr/share/nginx/html
  volumes:
  - name: storage
    persistentVolumeClaim:
      claimName: pvc-example
```

### 7.3 StorageClass

动态存储供应：

```yaml
apiVersion: storage.k8s.io/v1
kind: StorageClass
metadata:
  name: fast-ssd
provisioner: kubernetes.io/aws-ebs
parameters:
  type: gp2
  replication-type: none
allowVolumeExpansion: true
volumeBindingMode: WaitForFirstConsumer
```

## 8. 配置管理

### 8.1 ConfigMap

存储非敏感配置数据：

```yaml
apiVersion: v1
kind: ConfigMap
metadata:
  name: app-config
data:
  database_url: "mysql://localhost:3306/mydb"
  debug_mode: "true"
  app.properties: |
    server.port=8080
    server.servlet.context-path=/api
    logging.level.root=INFO
```

使用ConfigMap：

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: configmap-pod
spec:
  containers:
  - name: app
    image: myapp:latest
    env:
    - name: DATABASE_URL
      valueFrom:
        configMapKeyRef:
          name: app-config
          key: database_url
    - name: DEBUG_MODE
      valueFrom:
        configMapKeyRef:
          name: app-config
          key: debug_mode
    volumeMounts:
    - name: config-volume
      mountPath: /etc/config
  volumes:
  - name: config-volume
    configMap:
      name: app-config
```

### 8.2 Secret

存储敏感数据：

```yaml
apiVersion: v1
kind: Secret
metadata:
  name: app-secret
type: Opaque
data:
  username: YWRtaW4=  # base64编码的"admin"
  password: MWYyZDFlMmU2N2Rm  # base64编码的密码
```

```bash
# 创建Secret
kubectl create secret generic app-secret \
  --from-literal=username=admin \
  --from-literal=password=secretpassword

# 从文件创建Secret
kubectl create secret generic ssl-secret \
  --from-file=tls.crt=server.crt \
  --from-file=tls.key=server.key
```

使用Secret：

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: secret-pod
spec:
  containers:
  - name: app
    image: myapp:latest
    env:
    - name: DB_USERNAME
      valueFrom:
        secretKeyRef:
          name: app-secret
          key: username
    - name: DB_PASSWORD
      valueFrom:
        secretKeyRef:
          name: app-secret
          key: password
    volumeMounts:
    - name: secret-volume
      mountPath: /etc/secrets
      readOnly: true
  volumes:
  - name: secret-volume
    secret:
      secretName: app-secret
```

## 9. 网络管理

### 9.1 网络策略

控制Pod间的网络通信：

```yaml
apiVersion: networking.k8s.io/v1
kind: NetworkPolicy
metadata:
  name: deny-all
  namespace: production
spec:
  podSelector: {}
  policyTypes:
  - Ingress
  - Egress
```

允许特定流量的网络策略：

```yaml
apiVersion: networking.k8s.io/v1
kind: NetworkPolicy
metadata:
  name: allow-frontend-to-backend
spec:
  podSelector:
    matchLabels:
      app: backend
  policyTypes:
  - Ingress
  ingress:
  - from:
    - podSelector:
        matchLabels:
          app: frontend
    ports:
    - protocol: TCP
      port: 8080
```

### 9.2 DNS配置

自定义DNS配置：

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: dns-config-pod
spec:
  containers:
  - name: app
    image: nginx
  dnsPolicy: "None"
  dnsConfig:
    nameservers:
      - 8.8.8.8
      - 8.8.4.4
    searches:
      - example.com
    options:
      - name: ndots
        value: "2"
```

## 10. 安全管理

### 10.1 ServiceAccount

为Pod提供身份认证：

```yaml
apiVersion: v1
kind: ServiceAccount
metadata:
  name: my-service-account
  namespace: default
```

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: sa-pod
spec:
  serviceAccountName: my-service-account
  containers:
  - name: app
    image: nginx
```

### 10.2 RBAC

基于角色的访问控制：

#### 10.2.1 Role和RoleBinding

```yaml
apiVersion: rbac.authorization.k8s.io/v1
kind: Role
metadata:
  namespace: default
  name: pod-reader
rules:
- apiGroups: [""]
  resources: ["pods"]
  verbs: ["get", "watch", "list"]
---
apiVersion: rbac.authorization.k8s.io/v1
kind: RoleBinding
metadata:
  name: read-pods
  namespace: default
subjects:
- kind: ServiceAccount
  name: my-service-account
  namespace: default
roleRef:
  kind: Role
  name: pod-reader
  apiGroup: rbac.authorization.k8s.io
```

#### 10.2.2 ClusterRole和ClusterRoleBinding

```yaml
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRole
metadata:
  name: cluster-reader
rules:
- apiGroups: [""]
  resources: ["nodes", "namespaces"]
  verbs: ["get", "list"]
---
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  name: read-cluster
subjects:
- kind: ServiceAccount
  name: my-service-account
  namespace: default
roleRef:
  kind: ClusterRole
  name: cluster-reader
  apiGroup: rbac.authorization.k8s.io
```

### 10.3 Pod Security Standards

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: secure-pod
spec:
  securityContext:
    runAsNonRoot: true
    runAsUser: 1000
    fsGroup: 2000
  containers:
  - name: app
    image: nginx
    securityContext:
      allowPrivilegeEscalation: false
      readOnlyRootFilesystem: true
      capabilities:
        drop:
        - ALL
    volumeMounts:
    - name: tmp
      mountPath: /tmp
  volumes:
  - name: tmp
    emptyDir: {}
```

## 11. 监控与日志

### 11.1 资源监控

#### 11.1.1 Metrics Server

```bash
# 安装Metrics Server
kubectl apply -f https://github.com/kubernetes-sigs/metrics-server/releases/latest/download/components.yaml

# 查看节点资源使用
kubectl top nodes

# 查看Pod资源使用
kubectl top pods
```

#### 11.1.2 Prometheus监控

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: prometheus
spec:
  replicas: 1
  selector:
    matchLabels:
      app: prometheus
  template:
    metadata:
      labels:
        app: prometheus
    spec:
      containers:
      - name: prometheus
        image: prom/prometheus:latest
        ports:
        - containerPort: 9090
        volumeMounts:
        - name: config
          mountPath: /etc/prometheus
      volumes:
      - name: config
        configMap:
          name: prometheus-config
```

### 11.2 日志管理

#### 11.2.1 查看Pod日志

```bash
# 查看Pod日志
kubectl logs pod-name

# 查看特定容器日志
kubectl logs pod-name -c container-name

# 实时查看日志
kubectl logs -f pod-name

# 查看前N行日志
kubectl logs --tail=100 pod-name
```

#### 11.2.2 ELK Stack部署

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: elasticsearch
spec:
  replicas: 1
  selector:
    matchLabels:
      app: elasticsearch
  template:
    metadata:
      labels:
        app: elasticsearch
    spec:
      containers:
      - name: elasticsearch
        image: docker.elastic.co/elasticsearch/elasticsearch:7.15.0
        env:
        - name: discovery.type
          value: single-node
        - name: ES_JAVA_OPTS
          value: "-Xms512m -Xmx512m"
        ports:
        - containerPort: 9200
```

## 12. 高级特性

### 12.1 Horizontal Pod Autoscaler (HPA)

基于CPU使用率自动扩缩容：

```yaml
apiVersion: autoscaling/v2
kind: HorizontalPodAutoscaler
metadata:
  name: nginx-hpa
spec:
  scaleTargetRef:
    apiVersion: apps/v1
    kind: Deployment
    name: nginx-deployment
  minReplicas: 2
  maxReplicas: 10
  metrics:
  - type: Resource
    resource:
      name: cpu
      target:
        type: Utilization
        averageUtilization: 70
  - type: Resource
    resource:
      name: memory
      target:
        type: Utilization
        averageUtilization: 80
```

### 12.2 Vertical Pod Autoscaler (VPA)

```yaml
apiVersion: autoscaling.k8s.io/v1
kind: VerticalPodAutoscaler
metadata:
  name: nginx-vpa
spec:
  targetRef:
    apiVersion: apps/v1
    kind: Deployment
    name: nginx-deployment
  updatePolicy:
    updateMode: "Auto"
  resourcePolicy:
    containerPolicies:
    - containerName: nginx
      maxAllowed:
        cpu: 1
        memory: 500Mi
      minAllowed:
        cpu: 100m
        memory: 50Mi
```

### 12.3 Custom Resource Definitions (CRD)

```yaml
apiVersion: apiextensions.k8s.io/v1
kind: CustomResourceDefinition
metadata:
  name: applications.example.com
spec:
  group: example.com
  versions:
  - name: v1
    served: true
    storage: true
    schema:
      openAPIV3Schema:
        type: object
        properties:
          spec:
            type: object
            properties:
              image:
                type: string
              replicas:
                type: integer
                minimum: 1
                maximum: 10
  scope: Namespaced
  names:
    plural: applications
    singular: application
    kind: Application
```

### 12.4 Operators

使用Operator管理复杂应用：

```yaml
apiVersion: example.com/v1
kind: Application
metadata:
  name: my-app
spec:
  image: myapp:v1.0
  replicas: 3
  database:
    type: mysql
    version: "8.0"
    storage: 10Gi
```

## 13. 故障排查

### 13.1 常用调试命令

```bash
# 查看集群状态
kubectl cluster-info
kubectl get nodes
kubectl get componentstatuses

# 查看资源状态
kubectl get pods --all-namespaces
kubectl get events --sort-by=.metadata.creationTimestamp

# 详细描述资源
kubectl describe pod pod-name
kubectl describe node node-name

# 查看日志
kubectl logs pod-name
kubectl logs -f deployment/app-name

# 进入容器调试
kubectl exec -it pod-name -- /bin/bash
kubectl exec -it pod-name -c container-name -- /bin/sh

# 端口转发
kubectl port-forward pod/pod-name 8080:80
kubectl port-forward service/service-name 8080:80

# 复制文件
kubectl cp pod-name:/path/to/file /local/path
kubectl cp /local/path pod-name:/path/to/file
```

### 13.2 常见问题排查

#### 13.2.1 Pod启动失败

```bash
# 查看Pod状态和事件
kubectl get pods
kubectl describe pod pod-name
kubectl get events --field-selector involvedObject.name=pod-name

# 查看容器日志
kubectl logs pod-name
kubectl logs pod-name --previous  # 查看上一次运行的日志
```

#### 13.2.2 服务访问问题

```bash
# 检查Service和Endpoints
kubectl get svc
kubectl get endpoints
kubectl describe svc service-name

# 测试服务连通性
kubectl run test-pod --image=busybox --rm -it -- /bin/sh
# 在test-pod中执行
wget -qO- http://service-name:port
nslookup service-name
```

#### 13.2.3 网络问题

```bash
# 检查网络策略
kubectl get networkpolicies
kubectl describe networkpolicy policy-name

# 检查DNS
kubectl get pods -n kube-system | grep dns
kubectl logs -n kube-system deployment/coredns
```

### 13.3 性能调优

#### 13.3.1 资源限制调优

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: optimized-pod
spec:
  containers:
  - name: app
    image: myapp:latest
    resources:
      requests:
        memory: "256Mi"
        cpu: "250m"
      limits:
        memory: "512Mi"
        cpu: "500m"
    livenessProbe:
      httpGet:
        path: /health
        port: 8080
      initialDelaySeconds: 30
      periodSeconds: 10
      timeoutSeconds: 5
      failureThreshold: 3
    readinessProbe:
      httpGet:
        path: /ready
        port: 8080
      initialDelaySeconds: 5
      periodSeconds: 5
      timeoutSeconds: 3
      failureThreshold: 3
```

## 14. 最佳实践

### 14.1 资源管理最佳实践

1. **始终设置资源请求和限制**
2. **使用命名空间进行资源隔离**
3. **合理使用标签和选择器**
4. **定期清理未使用的资源**

### 14.2 安全最佳实践

1. **使用非root用户运行容器**
2. **启用RBAC并遵循最小权限原则**
3. **定期更新镜像和补丁**
4. **使用Secret管理敏感信息**
5. **启用网络策略**

### 14.3 部署最佳实践

1. **使用声明式配置**
2. **实施滚动更新策略**
3. **配置健康检查**
4. **使用多副本提高可用性**
5. **实施监控和日志记录**

### 14.4 配置管理最佳实践

```yaml
# 良好的Deployment配置示例
apiVersion: apps/v1
kind: Deployment
metadata:
  name: production-app
  labels:
    app: production-app
    version: v1.0
spec:
  replicas: 3
  strategy:
    type: RollingUpdate
    rollingUpdate:
      maxSurge: 1
      maxUnavailable: 0
  selector:
    matchLabels:
      app: production-app
  template:
    metadata:
      labels:
        app: production-app
        version: v1.0
    spec:
      securityContext:
        runAsNonRoot: true
        runAsUser: 1000
      containers:
      - name: app
        image: myapp:v1.0
        imagePullPolicy: Always
        ports:
        - containerPort: 8080
          name: http
        env:
        - name: ENV
          value: "production"
        envFrom:
        - configMapRef:
            name: app-config
        - secretRef:
            name: app-secrets
        resources:
          requests:
            memory: "256Mi"
            cpu: "250m"
          limits:
            memory: "512Mi"
            cpu: "500m"
        livenessProbe:
          httpGet:
            path: /health
            port: 8080
          initialDelaySeconds: 30
          periodSeconds: 10
        readinessProbe:
          httpGet:
            path: /ready
            port: 8080
          initialDelaySeconds: 5
          periodSeconds: 5
        volumeMounts:
        - name: app-storage
          mountPath: /data
      volumes:
      - name: app-storage
        persistentVolumeClaim:
          claimName: app-pvc
```

## 15. 生产环境部署

### 15.1 高可用集群架构

```yaml
# 高可用etcd配置
apiVersion: v1
kind: Pod
metadata:
  name: etcd
  namespace: kube-system
spec:
  containers:
  - name: etcd
    image: k8s.gcr.io/etcd:3.5.0-0
    command:
    - etcd
    - --name=etcd-1
    - --data-dir=/var/lib/etcd
    - --listen-client-urls=https://0.0.0.0:2379
    - --advertise-client-urls=https://10.0.0.1:2379
    - --listen-peer-urls=https://0.0.0.0:2380
    - --initial-advertise-peer-urls=https://10.0.0.1:2380
    - --initial-cluster=etcd-1=https://10.0.0.1:2380,etcd-2=https://10.0.0.2:2380,etcd-3=https://10.0.0.3:2380
    - --initial-cluster-state=new
    - --cert-file=/etc/kubernetes/pki/etcd/server.crt
    - --key-file=/etc/kubernetes/pki/etcd/server.key
    - --trusted-ca-file=/etc/kubernetes/pki/etcd/ca.crt
    - --peer-cert-file=/etc/kubernetes/pki/etcd/peer.crt
    - --peer-key-file=/etc/kubernetes/pki/etcd/peer.key
    - --peer-trusted-ca-file=/etc/kubernetes/pki/etcd/ca.crt
```

### 15.2 备份和恢复

#### 15.2.1 etcd备份

```bash
#!/bin/bash
# etcd备份脚本
ETCDCTL_API=3 etcdctl snapshot save /backup/etcd-snapshot-$(date +%Y%m%d-%H%M%S).db \
  --endpoints=https://127.0.0.1:2379 \
  --cacert=/etc/kubernetes/pki/etcd/ca.crt \
  --cert=/etc/kubernetes/pki/etcd/server.crt \
  --key=/etc/kubernetes/pki/etcd/server.key

# 验证备份
ETCDCTL_API=3 etcdctl snapshot status /backup/etcd-snapshot-$(date +%Y%m%d-%H%M%S).db
```

#### 15.2.2 etcd恢复

```bash
#!/bin/bash
# etcd恢复脚本
ETCDCTL_API=3 etcdctl snapshot restore /backup/etcd-snapshot.db \
  --data-dir=/var/lib/etcd-restore \
  --name=etcd-1 \
  --initial-cluster=etcd-1=https://10.0.0.1:2380 \
  --initial-advertise-peer-urls=https://10.0.0.1:2380
```

### 15.3 监控和告警

```yaml
# Prometheus告警规则
apiVersion: v1
kind: ConfigMap
metadata:
  name: prometheus-rules
data:
  kubernetes.rules: |
    groups:
    - name: kubernetes
      rules:
      - alert: PodCrashLooping
        expr: rate(kube_pod_container_status_restarts_total[15m]) > 0
        for: 5m
        labels:
          severity: warning
        annotations:
          summary: "Pod {{ $labels.pod }} is crash looping"
          description: "Pod {{ $labels.pod }} in namespace {{ $labels.namespace }} is restarting frequently"
      
      - alert: NodeNotReady
        expr: kube_node_status_condition{condition="Ready",status="true"} == 0
        for: 5m
        labels:
          severity: critical
        annotations:
          summary: "Node {{ $labels.node }} is not ready"
          description: "Node {{ $labels.node }} has been not ready for more than 5 minutes"
```

### 15.4 升级策略

```bash
#!/bin/bash
# Kubernetes集群升级脚本

# 1. 升级控制平面
sudo kubeadm upgrade plan
sudo kubeadm upgrade apply v1.25.0

# 2. 升级kubelet和kubectl
sudo apt-mark unhold kubelet kubectl
sudo apt-get update
sudo apt-get install -y kubelet=1.25.0-00 kubectl=1.25.0-00
sudo apt-mark hold kubelet kubectl

# 3. 重启kubelet
sudo systemctl daemon-reload
sudo systemctl restart kubelet

# 4. 验证升级
kubectl get nodes
kubectl version
```
