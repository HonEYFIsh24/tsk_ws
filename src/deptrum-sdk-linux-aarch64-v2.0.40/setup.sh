#!/bin/bash

# Copyright (c) Deptrum Inc. All Rights Reserved.

# Enable strict mode for better error handling
set -euo pipefail

# Store original directory safely
readonly INITIAL_DIR=$(pwd)
trap 'cd "$INITIAL_DIR"' EXIT

# Get script's absolute directory
SCRIPT_HOME=$(cd "$(dirname "$0")" && pwd)
readonly PROJECT_HOME=$SCRIPT_HOME

# Define essential paths
readonly SCRIPTS_PATH="$PROJECT_HOME/scripts"

# Setup udev rules if available
if [[ -d "$SCRIPTS_PATH" && -f "$SCRIPTS_PATH/install_udev_rules.sh" ]]; then
    echo "INFO: Installing udev rules..."
    "$SCRIPTS_PATH/install_udev_rules.sh"
fi

echo "SUCCESS: Environment setup completed"