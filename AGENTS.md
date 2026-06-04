# AGENTS.md

本文件用于指导 Codex、ChatGPT、Claude、Copilot 或其他 AI 代理在本项目中工作。任何代理开始修改前，应先阅读本文件，并遵守这里的项目边界、验证流程和 GitHub 上传规则。

## 项目概况

本项目是 `OpenHarmony 移植 ESP32 6.0 SDK`，基于 ESP-IDF 6.0.1，为 ESP32 系列芯片提供 OpenHarmony 6.0 LiteOS-M 的轻量级移植框架。

主要目标：

- 维护 OpenHarmony/LiteOS-M 到 ESP32 的移植层。
- 支持 ESP32 Xtensa 和 ESP32-C6 RISC-V 目标。
- 保持 Demo、板级配置、端口抽象和文档清晰可验证。
- 让仓库可直接在 GitHub 上展示、克隆、构建和继续迭代。

## 仓库位置和远程仓库

本地 WSL 路径：

```bash
/home/lc/esp32_sdk/ohos_esp32_port
```

Windows/Codex 路径：

```text
\\wsl.localhost\Ubuntu-20.04\home\lc\esp32_sdk\ohos_esp32_port
```

当前 GitHub 远程仓库：

```text
https://github.com/HEIseYOUmolc/Openharmony-ESP326.0SDK.git
```

本仓库已由用户上传到 GitHub。后续代理如需推送，应基于当前 `origin/main` 工作，不要重新创建仓库。

## 操作边界

代理可以修改：

- `README.md`
- `AGENTS.md`
- `docs/`
- `main/`
- `components/`
- `board_profile/`
- `tools/`
- `.github/workflows/`
- 根目录构建配置文件，如 `CMakeLists.txt`、`Kconfig.projbuild`、`sdkconfig.defaults*`

代理不要随意修改：

- `.git/`
- 用户本地认证文件
- 与本项目无关的外层目录
- 构建产物目录，如 `build/`
- 用户未明确要求的其他仓库或工作区

如果工作区已有用户改动，代理必须先查看 `git status -sb` 和相关 diff，不要覆盖或回退用户改动。

## 编码和文档风格

- 代码和文档优先使用清晰、直接、可维护的表达。
- C/CMake/Kconfig 修改应遵循 ESP-IDF 项目现有风格。
- README 面向中文读者，保留必要英文术语，例如 ESP-IDF、LiteOS-M、Kconfig、Demo。
- 新增文档应使用 Markdown。
- 不要提交本地构建产物、临时文件、日志文件或 IDE 配置。

## 常用检查命令

进入项目：

```bash
cd ~/esp32_sdk/ohos_esp32_port
```

查看状态：

```bash
git status -sb
git remote -v
git branch -vv
```

查看 README 修改：

```bash
git diff -- README.md
```

查看所有文件：

```bash
rg --files
```

## 构建验证

构建前需要先加载 ESP-IDF 6.0.1 环境：

```bash
. $IDF_PATH/export.sh
```

验证 ESP32：

```bash
idf.py set-target esp32
idf.py build
```

验证 ESP32-C6：

```bash
idf.py set-target esp32c6
idf.py build
```

如果本地缺少 ESP-IDF 环境，代理应明确说明未能运行构建，而不是声称构建通过。

## GitHub 上传和推送流程

本仓库已经上传过 GitHub，后续不要再运行 `gh repo create` 创建新仓库。正确流程是提交当前修改并推送到已有远程：

```bash
cd ~/esp32_sdk/ohos_esp32_port
git status -sb
git add <changed-files>
git commit -m "<clear commit message>"
git push
```

如果 upstream 未配置：

```bash
git push -u origin main
```

如果远程地址丢失或错误：

```bash
git remote remove origin 2>/dev/null
git remote add origin https://github.com/HEIseYOUmolc/Openharmony-ESP326.0SDK.git
git push -u origin main
```

常用提交信息示例：

```bash
git commit -m "docs: add Chinese README"
git commit -m "docs: add agent instructions"
git commit -m "fix: update ESP32 port configuration"
```

## GitHub CLI 登录

用户已经在 WSL 中配置过 `gh auth login`。如需检查：

```bash
gh auth status
```

如果需要重新登录：

```bash
gh auth login
```

推荐选择：

```text
GitHub.com
HTTPS
Authenticate Git with GitHub credentials: Yes
```

如果使用 token，至少需要这些 scope：

```text
repo, read:org, workflow
```

## 已知问题和处理方式

### 1. 旧版 Git 不支持 `git init -b main`

如果遇到：

```text
error: unknown switch `b`
```

使用：

```bash
git init
git checkout -b main
```

本仓库已经初始化，不需要再次执行。

### 2. WSL 中没有 `gh`

Ubuntu 默认源可能没有 `gh` 包。应使用 GitHub CLI 官方源安装，或由用户按 GitHub 官方文档安装。

不要安装 `gitsome` 替代 `gh`，它不是 GitHub 官方 CLI。

### 3. `gh repo create` 报 GraphQL EOF

如果遇到：

```text
Post "https://api.github.com/graphql": EOF
```

通常是 GitHub API 网络连接中断。处理方式：

- 不要重复创建仓库。
- 到 GitHub 网页确认仓库是否已经存在。
- 仓库已存在时，直接配置 `origin` 并 `git push`。

### 4. `git push` 报 GnuTLS 错误

如果遇到：

```text
gnutls_handshake() failed: The TLS connection was non-properly terminated
```

可先尝试：

```bash
git config --global http.version HTTP/1.1
git push
```

如果仍失败，可改用 SSH remote。

### 5. Codex 从 Windows 访问 WSL 仓库时报 dubious ownership

如果遇到：

```text
fatal: detected dubious ownership in repository
```

可在 Windows PowerShell 侧配置：

```powershell
git config --global --add safe.directory "//wsl.localhost/Ubuntu-20.04/home/lc/esp32_sdk/ohos_esp32_port"
```

或者尽量让 Git 操作在 WSL 终端中执行。

### 6. Codex 沙箱无法执行 `git add/commit/push`

如果 Codex 当前会话无法写入 `.git/`，代理不能声称已推送。应说明限制，并让用户在 WSL 中执行：

```bash
cd ~/esp32_sdk/ohos_esp32_port
git add <changed-files>
git commit -m "<message>"
git push
```

只有在命令实际成功或 GitHub 插件/API 明确返回成功时，才能报告“已推送”。

## README 维护要求

README 应保持中文版，至少包含：

- 项目定位
- 支持目标
- 目录结构
- 环境要求
- 编译方法
- Demo 选择
- 移植层说明
- 上游基线
- 常见问题入口
- 当前状态

如果修改工程结构、支持目标或构建方法，必须同步更新 README。

## 提交前检查清单

提交前至少确认：

- `git status -sb` 只包含本次任务相关文件。
- README、AGENTS 或 docs 修改能正常阅读。
- 构建相关修改已尽可能运行 `idf.py build` 验证。
- 未提交 `build/`、`sdkconfig`、日志、临时文件。
- 提交信息清晰说明本次变更。

## 当前建议的下一次提交

前面已经完成的本地变更包括：

- 将 `README.md` 改为中文版。
- 新增本 `AGENTS.md`，用于指导后续 AI 代理操作。

建议提交：

```bash
cd ~/esp32_sdk/ohos_esp32_port
git add README.md AGENTS.md
git commit -m "docs: add Chinese README and agent guide"
git push
```
