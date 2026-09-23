# 基于官方 Ros2 Jazzy Desktop 镜像（Ubuntu 24.04 Noble 基础）
FROM osrf/ros:jazzy-desktop-noble

# 避免 apt 安装时交互式提示
ENV DEBIAN_FRONTEND=noninteractive

# 安装常用开发工具（colcon 构建工具、rosdep 常用调试工具等）
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-colcon-common-extensions \
    python3-rosdep \
    ros-dev-tools \
    ros-jazzy-rmw-cyclonedds-cpp \
    ros-jazzy-xacro \
    ros-jazzy-turtle-tf2-py \
    ros-jazzy-tf2-tools \
    ros-jazzy-joint-state-publisher-gui \
    vim \
    git \
    curl \
    tmux \
    xclip \
    sudo \
    ncurses-term \
    && rm -rf /var/lib/apt/lists/*

# 构建参数：与宿主机一致的用户名和 UID/GID
ARG USERNAME=zy
ARG USER_UID=1000
ARG USER_GID=1000

# 创建用户和组，并赋予 sudo 免密
RUN if getent passwd $USER_UID >/dev/null; then \
        existing_user=$(getent passwd $USER_UID | cut -d: -f1); \
        userdel -r "$existing_user" 2>/dev/null || true; \
    fi \
    && if getent group $USER_GID >/dev/null; then \
        existing_group=$(getent group $USER_GID | cut -d: -f1); \
        groupdel "$existing_group" 2>/dev/null || true; \
    fi \
    && groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /bin/bash \
    && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# 环境变量
ENV USER=$USERNAME
ENV HOME=/home/$USERNAME
ENV ROS_WS=$HOME/ros2-docker-learning/ros2_ws
WORKDIR $ROS_WS

# 切换到普通用户
USER $USERNAME

CMD ["bash"]
