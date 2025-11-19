# python version
FROM python:3.10-slim

# set working directory
WORKDIR /app

# install apt dependencies
RUN apt-get update && apt-get install -y \
    tar gzip unzip wget git \
    build-essential cmake gdb pkg-config libffi-dev libssl-dev \
    x11-apps \
    mesa-utils freeglut3-dev mesa-common-dev llvm \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libglew-dev \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev \
    libportaudio2 \
    libpulse0 \
    libasound2 \
    && rm -rf /var/lib/apt/lists/*

# copy code into working directory
COPY requirements.txt .

# install python dependencies
RUN pip install --upgrade pip setuptools wheel
RUN pip install --no-cache-dir -r requirements.txt

# dev environment variables
ENV DISPLAY=":0"
ENV XDG_RUNTIME_DIR="/tmp/runtime-root"

# run app
CMD ["bash"]

# CMD ["python", "kinematics.py"]
# 