#!/bin/bash

# Variables
IMAGE_NAME="guillaumemichel1026/gcolor-emulator"
TAG="latest"
PLATFORM="linux/amd64"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[1;34m'
NC='\033[0m' # No Color

# Step 1: Build the Docker image
echo -e "${BLUE}Building Docker image for platform ${PLATFORM}...${NC}"

docker build --platform $PLATFORM -t $IMAGE_NAME:$TAG .

# Check if the build succeeded
if [ $? -eq 0 ]; then
    echo -e "${GREEN}Docker build succeeded. Proceeding to push the image...${NC}"

    # Step 2: Push the image to Docker Hub
    docker push $IMAGE_NAME:$TAG

    # Check if the push succeeded
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Docker image successfully pushed to Docker Hub: ${IMAGE_NAME}:${TAG}${NC}"
    else
        echo -e "${RED}Failed to push the Docker image to Docker Hub.${NC}"
        exit 2
    fi
else
    echo -e "${RED}Docker build failed. Aborting the push.${NC}"
    exit 1
fi
