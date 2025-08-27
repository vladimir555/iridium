#!/bin/bash

VERSION_FILE="version.txt"
MAJOR=0

# Detect the main branch (main or master)
if git show-ref --quiet refs/heads/main; then
    MAIN_BRANCH="main"
else
    MAIN_BRANCH="master"
fi

# Count the number of merges from develop -> main/master (minor)
MINOR=$(git log $MAIN_BRANCH --merges --oneline --first-parent | grep -i "develop" | awk 'END{print NR}')

# Find the latest merge of develop -> main/master
LAST_MERGE=$(git log $MAIN_BRANCH --merges --oneline --first-parent | grep -i "develop" | head -n1 | awk '{print $1}')

# patch — number of commits after the last merge
if [ -n "$LAST_MERGE" ]; then
    PATCH=$(git rev-list ${LAST_MERGE}..HEAD --count)
else
    PATCH=$(git rev-list HEAD --count)
fi

VERSION="${MAJOR}.${MINOR}.${PATCH}"

echo "$VERSION" > "$VERSION_FILE"
git add "$VERSION_FILE"
