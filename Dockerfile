FROM ros:noetic

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    python3-pip \
    python3-catkin-tools \
    ros-noetic-catkin \
    ros-noetic-desktop-full \
    ros-noetic-tf \
    ros-noetic-pcl-ros \
    ros-noetic-cv-bridge \
    ros-noetic-image-transport \
    ros-noetic-navigation \
    ros-noetic-move-base \
    ros-noetic-amcl \
    ros-noetic-map-server \
    ros-noetic-gmapping \
    ros-noetic-dwa-local-planner \
    libeigen3-dev \
    libopencv-dev \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install numpy opencv-python rospkg catkin_pkg

ENV CATKIN_WS=/root/auto_filmer_ws
RUN mkdir -p $CATKIN_WS/src

WORKDIR $CATKIN_WS/src
RUN git clone git@github.com:edyou25/Auto-Filmer.git

WORKDIR $CATKIN_WS
RUN /bin/bash -c "source /opt/ros/noetic/setup.bash && catkin_make"

# 设置环境
RUN echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc
RUN echo "source $CATKIN_WS/devel/setup.bash" >> ~/.bashrc

# 设置默认工作目录
WORKDIR $CATKIN_WS

CMD ["/bin/bash"]
