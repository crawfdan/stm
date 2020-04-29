# We use docker for the build environment

# install docker
https://docs.docker.com/get-docker/

# Open a command prompt in Windows

# build our image
# these follow the form docker build -t <image>:<tag> -f <DOCKERFILE> <location>
docker build -t ubuntu-gcc:latest -f Dockerfile .

# gain access to our container and build
docker run -it --rm ubuntu-gcc:latest /bin/sh -c "make"