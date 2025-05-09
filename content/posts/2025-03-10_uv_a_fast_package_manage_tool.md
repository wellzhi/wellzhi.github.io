+++
date = '2025-03-10T11:51:06+08:00'
draft = true
title = 'uv, a fast package manage tool'
+++


以下是 `uv` 工具的核心用法整理，涵盖安装、项目管理、依赖管理、Python 版本控制等场景，方便快速查阅。

---

### **一、安装与配置**
1. **一键安装**  
   ```bash
   # macOS/Linux
   curl -LsSf https://astral.sh/uv/install.sh | sh

   # Windows
   irm https://astral.sh/uv/install.ps1 | iex
   ```
   - 支持通过 `pip` 安装：`pip install uv`

2. **版本验证与更新**  
   ```bash
   uv --version      # 检查版本
   uv self update    # 自动更新到最新版本
   ```

---

### **二、项目管理**
1. **初始化项目**  
   ```bash
   uv init my_project      # 创建项目目录并生成配置文件
   uv init . --python 3.12 # 在当前目录初始化并指定 Python 版本
   ```
   - 生成文件：`pyproject.toml`（主依赖）、`uv.lock`（精确锁文件）

2. **依赖管理**  
   ```bash
   uv add requests         # 添加主依赖
   uv add --dev pytest     # 添加开发依赖
   uv remove flask         # 移除依赖
   uv lock                 # 生成/更新锁文件
   uv sync                 # 同步依赖到虚拟环境
   ```

3. **运行脚本**  
   ```bash
   uv run main.py          # 自动创建隔离环境并执行脚本
   uv run --python 3.11 script.py  # 指定 Python 版本运行
   ```

---

### **三、虚拟环境管理**
1. **创建与激活**  
   ```bash
   uv venv                 # 默认创建 .venv 环境
   uv venv myenv --python 3.12  # 指定名称和 Python 版本
   source .venv/bin/activate    # 激活环境（类 Unix）
   .venv\Scripts\activate      # 激活环境（Windows）
   ```

2. **依赖批量安装**  
   ```bash
   uv pip install -r requirements.txt    # 安装文件中的依赖
   uv pip compile requirements.in        # 生成跨平台依赖文件
   ```

---

### **四、Python 版本管理**
1. **多版本安装与切换**  
   ```bash
   uv python install 3.10 3.11 3.12  # 安装多个版本
   uv python pin 3.11                # 固定当前目录使用的版本
   ```

2. **指定版本运行**  
   ```bash
   uv run --python 3.12 script.py    # 临时使用指定版本
   ```

---

### **五、工具与 CLI 管理**
1. **全局工具安装**  
   ```bash
   uv tool install ruff      # 类似 pipx，全局安装工具
   uvx pycowsay "Hello!"     # 临时运行工具（自动创建隔离环境）
   ```

2. **工具版本管理**  
   ```bash
   uv tool list             # 查看已安装工具
   uv tool uninstall ruff  # 卸载工具
   ```

---

### **六、高级用法**
1. **依赖检查与修复**  
   ```bash
   uv pip check           # 验证依赖兼容性
   uv pip --force-reinstall package  # 强制重装包
   ```

2. **跨平台协作**  
   - 提交 `uv.lock` 文件，确保团队成员依赖版本一致。

3. **性能优化场景**  
   ```bash
   uv venv --no-cache     # 跳过缓存（调试用）
   UV_CACHE_DIR=/path uv pip install...  # 自定义缓存路径
   ```

---

### **七、常用命令速查表**
| 场景               | 命令示例                              |
|--------------------|---------------------------------------|
| **创建项目**       | `uv init my_project`                 |
| **添加依赖**       | `uv add flask`                       |
| **生成锁文件**     | `uv lock`                            |
| **同步依赖**       | `uv sync`                            |
| **运行脚本**       | `uv run script.py`                   |
| **安装 Python**    | `uv python install 3.12`             |
| **全局工具**       | `uv tool install black`              |

---

### **适用场景推荐**
- **快速原型开发**：`uv run` + 临时依赖声明  
- **CI/CD 流水线**：利用极速依赖安装优化构建时间  
- **多版本项目维护**：通过 `uv python pin` 管理不同项目的 Python 版本  

[uv 官方 GitHub](https://github.com/astral-sh/uv)。