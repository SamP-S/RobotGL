# python version
FROM python:3.10-slim

# set working directory
WORKDIR /app

# install apt dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake gdb pkg-config \
    mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev llvm \
    libglew-dev libsdl2-dev \ 
    && rm -rf /var/lib/apt/lists/*

# copy code into working directory
COPY . .

# install python dependencies
RUN pip install --upgrade pip \
 && pip install -r ./requirements.txt

# dev environment variables
ENV DISPLAY="host.docker.internal:0"

# run app
CMD ["python", "robot.py"]