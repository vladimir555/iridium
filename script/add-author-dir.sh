#!/bin/bash
# add-author-header.sh - Adds copyright header to C/C++ files
# Usage: ./add-author-header.sh <file-or-directory> [<file-or-directory> ...]

set -euo pipefail

# Output colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

# Counters for statistics
PROCESSED=0
SKIPPED=0
ERRORS=0

# ============================================================================
# Functions
# ============================================================================

print_usage() {
    echo -e "${CYAN}Usage:${NC} $0 <file-or-directory> [<file-or-directory> ...]"
    echo ""
    echo "Examples:"
    echo "  $0 source/main.cpp"
    echo "  $0 source/"
    echo "  $0 source/ include/"
    echo "  $0 ."
}

# Check if path should be excluded
should_exclude() {
    local PATH="$1"
    case "$PATH" in
        */build/*|*/cmake-build*/*|*/external/*|*/third_party/*|*/vendor/*|*/.git/*|*/.svn/*|*/.vs/*|*/out/*|*/dist/*)
            return 0  # true - exclude
            ;;
        *)
            return 1  # false - include
            ;;
    esac
}

# Process a single file
process_file() {
    local FILE="$1"

    # Check if file exists
    if [ ! -f "$FILE" ]; then
        echo -e "${RED}File not found: $FILE${NC}"
        ERRORS=$((ERRORS + 1))
        return 1
    fi

    # Check file extension (only C/C++ files)
    local EXT="${FILE##*.}"
    case "$EXT" in
        h|hpp|hh|c|cc|cpp|cxx)
            ;;
        *)
            return 0
            ;;
    esac

    # Check if copyright already exists (within first 10 lines)
    if head -n 10 "$FILE" | grep -qiE "(copyright|©|SPDX-License-Identifier)"; then
        SKIPPED=$((SKIPPED + 1))
        return 0
    fi

    # Header with dynamic year
    local YEAR
    YEAR=$(date +%Y)
    local HEADER="// Copyright © 2019-$YEAR Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

"

    # Create temp file in the same directory (preserves permissions)
    local TEMP_FILE
    TEMP_FILE=$(mktemp "${FILE}.XXXXXX")

    # Ensure cleanup on failure
    cleanup() {
        rm -f "$TEMP_FILE" 2>/dev/null || true
    }
    trap cleanup EXIT

    # Find first meaningful line (#include, #pragma, class, namespace, etc.)
    local FIRST_CODE_LINE
    FIRST_CODE_LINE=$(grep -nE "^[[:space:]]*(#include|#pragma|class|struct|namespace|enum|int|void|template)" "$FILE" 2>/dev/null | head -n1 | cut -d: -f1 || echo "")

    if [ -z "$FIRST_CODE_LINE" ]; then
        # Empty file or no code — just write the header
        printf '%s' "$HEADER" > "$TEMP_FILE"
        cat "$FILE" >> "$TEMP_FILE"
    else
        # Insert header before the first code line
        # NOTE: head -n 0 is not supported on BSD (macOS), so we check first
        if [ "$FIRST_CODE_LINE" -gt 1 ]; then
            head -n $((FIRST_CODE_LINE - 1)) "$FILE" > "$TEMP_FILE"
        else
            : > "$TEMP_FILE"  # Create empty temp file (BSD-compatible)
        fi

        printf '%s' "$HEADER" >> "$TEMP_FILE"
        tail -n +"$FIRST_CODE_LINE" "$FILE" >> "$TEMP_FILE"
    fi

    # Preserve original file permissions (cross-platform: GNU and BSD stat)
    if chmod --reference="$FILE" "$TEMP_FILE" 2>/dev/null; then
        :  # GNU chmod succeeded
    else
        # BSD fallback (macOS)
        chmod "$(stat -f '%Lp' "$FILE")" "$TEMP_FILE" 2>/dev/null || true
    fi

    # Atomic replacement
    mv "$TEMP_FILE" "$FILE"
    trap - EXIT  # Disable cleanup after successful mv

    echo -e "${GREEN}+ $FILE${NC}"
    PROCESSED=$((PROCESSED + 1))
}

# Collect all C/C++ files from a directory (recursively)
collect_from_dir() {
    local DIR="$1"
    find "$DIR" -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.hh" \
                           -o -name "*.c" -o -name "*.cc" -o -name "*.cpp" -o -name "*.cxx" \) 2>/dev/null | \
    while IFS= read -r FILE; do
        if ! should_exclude "$FILE"; then
            echo "$FILE"
        fi
    done
}

# ============================================================================
# Main
# ============================================================================

# Check arguments
if [ "$#" -lt 1 ]; then
    print_usage
    exit 1
fi

echo -e "${CYAN}Collecting files...${NC}"

# Collect all files into a temp list (POSIX-compatible, no mapfile)
FILE_LIST=$(mktemp)
trap 'rm -f "$FILE_LIST"' EXIT

for TARGET in "$@"; do
    if [ -f "$TARGET" ]; then
        echo "$TARGET" >> "$FILE_LIST"
    elif [ -d "$TARGET" ]; then
        collect_from_dir "$TARGET" >> "$FILE_LIST"
    else
        echo -e "${RED}Not found (skipping): $TARGET${NC}"
        ERRORS=$((ERRORS + 1))
    fi
done

# Remove duplicates and sort
sort -u "$FILE_LIST" -o "$FILE_LIST"

TOTAL=$(wc -l < "$FILE_LIST" | tr -d ' ')
if [ "$TOTAL" -eq 0 ]; then
    echo -e "${YELLOW}No C/C++ files found.${NC}"
    rm -f "$FILE_LIST"
    trap - EXIT
    exit 0
fi

echo -e "${CYAN}Found $TOTAL file(s). Processing...${NC}"
echo ""

# Process each file
while IFS= read -r FILE; do
    process_file "$FILE" || true
done < "$FILE_LIST"

# Cleanup
rm -f "$FILE_LIST"
trap - EXIT

# Print statistics
echo ""
echo -e "${CYAN}=== Summary ===${NC}"
echo -e "${GREEN}Processed: $PROCESSED${NC}"
echo -e "${YELLOW}Skipped:   $SKIPPED${NC}"
if [ "$ERRORS" -gt 0 ]; then
    echo -e "${RED}Errors:    $ERRORS${NC}"
fi
