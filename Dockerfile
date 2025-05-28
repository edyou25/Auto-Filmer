
# Use the official NVIDIA CUDA image as the base image

FROM nvidia/cuda:12.8.0-devel-ubuntu20.04
# cuda:12.8.0 is for my RTX 5070 GPU
# Find your container tag here: https://hub.docker.com/r/nvidia/cuda/tags

# Set environment variables for CUDA
ENV CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda
ENV PATH $CUDA_TOOLKIT_ROOT_DIR/bin:$PATH
ENV LD_LIBRARY_PATH $CUDA_TOOLKIT_ROOT_DIR/lib64:$LD_LIBRARY_PATH
ENV DEBIAN_FRONTEND=noninteractive
# Jump the interactive prompt during package installation
ENV TZ=Asia/Hong_Kong

RUN apt-get update && apt-get install -y tzdata
# Install necessary packages and tools
RUN apt-get update && apt-get install -y \
    lsb-release \
    gnupg2 \
    curl \
    wget \
    git \
    build-essential \
    cmake \
    python3-pip \
    libopencv-dev \
    libeigen3-dev \
    libboost-all-dev

# Setup ROS Noetic sources
RUN sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
RUN apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654

# Install ROS Noetic
RUN apt-get update && apt-get install -y \
    ros-noetic-desktop-full \
    ros-noetic-roscpp \
    ros-noetic-sensor-msgs \
    ros-noetic-cv-bridge \
    ros-noetic-image-transport \
    python3-rosdep

# # Initialize rosdep
RUN rosdep init && rosdep update
RUN /bin/bash -c "source /opt/ros/noetic/setup.bash && catkin_make"
RUN /bin/bash -c "source /opt/ros/noetic/setup.bash 
RUN echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc

# # Deploy Auto-Filmer
WORKDIR /root
RUN git clone https://github.com/edyou25/Auto-Filmer.git
WORKDIR /root/Auto-Filmer

# Some runtime dependencies
RUN ln -s /usr/bin/python3 /usr/bin/python
RUN apt-get install python3-pil python3-pil.imagetk

# Set the entrypoint to bash
ENTRYPOINT ["/bin/bash"]