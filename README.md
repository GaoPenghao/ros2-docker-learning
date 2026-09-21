# ROS2 Jazzy Docker 学习环境
基于`osrf/ros:jazzy-desktop-noble`官方镜像构建的 ROS2 Jazzy 学习环境。

容器与宿主机共用网络、共用用户名和 UID/GID 、挂载整个`/home/${USEERNAME}`目录，GUI 程序（RViz rqt 等）可直接显示到宿主机桌面。

## 目录结构
```text
ros2-docker-learning/
├── docker/
│ ├── Dockerfile
│ ├── docker-compose.yml
│ └── .env
├── ros2_ws/ # ROS 2 工作空间，代码都放这里
│ └── src/ # 所有 ROS 2 包的源码
└── README.md
```

## 前置条件

- 宿主机：Ubuntu 24.04
- 已安装 Docker Engine 与 Docker Compose 插件
- 宿主机用户 `zy`（UID/GID 均为 1000），如果不同请修改 `docker/.env`

首次使用前，在宿主机上执行一次（允许容器访问 X11，用于显示 GUI）：
```bash
xhost +local:docker
```
宿主机重启后需要重新执行这条命令。后续待验证是否需要，目前看起来不需要执行这个

## 首次构建并启动
```bash
# 构建
cd ~/ros2-docker-learning/docker
docker compose up -d --build

# 进入容器
docker compose exec ros2 bash
```

## 构建失败时重新构建
常见失败原因：网络抖动导致 `apt-get` 拉包失败，或 `Dockerfile` 写错。

先清理失败的中间层缓存，再重试：
```bash
cd ~/ros2-docker-learning/docker

# 停掉并删除旧容器
docker compose down

# 清理构建缓存（不会删除已成功下载的基础镜像层）
docker builder prune -f

# 重新构建
docker compose up -d --build
```
如果怀疑基础镜像层损坏，可以强制不使用缓存：
```bash
docker compose build --no-cache
docker compose up -d
```

## 常用操作命令
所有命令在宿主机 `~/ros2-docker-learning/docker` 目录下执行

| 操作 | 命令 |
|------|------|
| 启动容器（后台） | `docker compose up -d` |
| 停止并删除容器 | `docker compose down` |
| 停止容器（不删除） | `docker compose stop` |
| 启动已停止的容器 | `docker compose start` |
| 进入容器（开新 shell） | `docker compose exec ros2 bash` |
| 查看容器状态 | `docker compose ps` |
| 查看容器日志 | `docker compose logs -f ros2` |
| 在容器内执行单条命令 | `docker compose exec ros2 <cmd>` |
|

## 修改 Dockerfile 或 docker-compose.yml 后如何重建
只要这两个文件有改动，就需要重建容器：
```bash
cd ~/ros2-docker-learning/docker

# 1. 停掉旧容器
docker compose down

# 2. 重新构建镜像并启动新容器
docker compose up -d --build
```
如果只改了 `docker-compose.yml` 里挂载、环境变量之类的配置，可以不重新构建镜像，直接重建容器：
```bash
docker compose up -d --force-recreate
```
注意：容器重建后，容器内部非挂载路径下的所有改动都会丢失。所以：
- 代码放 `ros2_ws/`，宿主机可见，不会丢。
- 用户配置放 `/home/zy/`，宿主机可见，不会丢。
- 其他临时改动（例如手动改 `/etc/hosts`）应当写进 `docker-compose.yml` 或 `Dockerfile`

## 彻底清除容器和镜像
当你想从零开始，或者磁盘空间不足时：
```bash
cd ~/ros2-docker-learning/docker

# 1. 停止并删除容器、网络
docker compose down

# 2. 删除本项目的镜像
docker rmi ros2-jazzy-learning:latest

# 3. 清理构建缓存（可选，释放较多磁盘）
docker builder prune -f

# 4. 清理未使用的镜像
docker system prune -a
```