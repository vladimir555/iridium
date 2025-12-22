#!/bin/bash
# install-git-hooks.sh
# Simply copies the pre-commit hook and makes it executable

REPO_ROOT=$(git rev-parse --show-toplevel)
HOOKS_DIR="$REPO_ROOT/.git/hooks"

# Verify that this is a git repository
if [ ! -d "$HOOKS_DIR" ]; then
    echo "Error: .git/hooks not found. Make sure you are running this inside a git repository."
    exit 1
fi

# Copy the pre-commit hook
cp "$REPO_ROOT/shell/git-hooks/pre-commit" "$HOOKS_DIR/pre-commit"
chmod +x "$HOOKS_DIR/pre-commit"

echo "Pre-commit hook has been successfully installed!"
