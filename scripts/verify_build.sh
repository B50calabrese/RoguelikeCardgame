#!/bin/bash
set -e

# Configuration
BUILD_DIR="build"
SCREENSHOT_DIR="screenshots"
XVFB_SCREEN="1024x768x24"
DEMO_RUNTIME=5  # Time to run each demo in seconds

mkdir -p $SCREENSHOT_DIR

# 1. Run Tests
echo "Running C++ Tests..."
cd $BUILD_DIR
ctest --output-on-failure
cd ..

# 2. Run Demos and Capture Screenshots
echo "Capturing Screenshots for Demos..."

# Start Xvfb once for all captures to avoid authorization/startup issues
Xvfb :99 -screen 0 $XVFB_SCREEN &
XVFB_PID=$!
export DISPLAY=:99

# Give Xvfb time to start
sleep 2

# Cleanup function to ensure Xvfb is killed on exit
cleanup() {
    kill $XVFB_PID || true
    wait $XVFB_PID 2>/dev/null || true
}
trap cleanup EXIT

# Function to run a demo and capture a screenshot
capture_screenshot() {
    local demo_path=$1
    local demo_name=$2

    echo "Processing $demo_name..."

    # Run demo in background
    $demo_path > /dev/null 2>&1 &
    local demo_pid=$!

    # Wait for the demo to initialize and render
    sleep $DEMO_RUNTIME

    # Capture screenshot using import (ImageMagick)
    # Target the root window of the current display (:99)
    import -window root "$SCREENSHOT_DIR/${demo_name}.png"

    # Kill the demo
    kill $demo_pid || true
    wait $demo_pid 2>/dev/null || true

    echo "Screenshot saved to $SCREENSHOT_DIR/${demo_name}.png"
}

# Check if executables exist before trying to run them
run_if_exists() {
    if [ -f "$1" ]; then
        capture_screenshot "$1" "$2"
    else
        echo "Warning: Executable $1 not found. Skipping."
    fi
}

# Main Game
run_if_exists "./build/RoguelikeCardgame" "MainGame"

# Game Demos
run_if_exists "./build/demos/cardviewerdemo" "CardViewerDemo"
run_if_exists "./build/demos/handrendererdemo" "HandRendererDemo"
run_if_exists "./build/demos/uibuttondemo" "UIButtonDemo"

echo "Verification Complete. Screenshots available in $SCREENSHOT_DIR/"
