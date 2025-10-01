#!/bin/bash

# CLI Tests for Kanban-Lite Etapa 2
# Author: Anne Fernandes da Costa Oliveira
# Date: 01/10/2025

set -e  # Exit on any error

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Paths
CLI_EXECUTABLE="./kanban_cli"
TEST_OUTPUT_DIR="/tmp/kanban_test"
TEST_SAVE_FILE="${TEST_OUTPUT_DIR}/test_state.kanban"

# Create test directory
mkdir -p "${TEST_OUTPUT_DIR}"

# Test counter
TESTS_RUN=0
TESTS_PASSED=0

echo -e "${YELLOW}======================================${NC}"
echo -e "${YELLOW}    Kanban-Lite CLI Tests - Etapa 2   ${NC}"
echo -e "${YELLOW}======================================${NC}"
echo

# Function to run test
run_test() {
    local test_name="$1"
    local commands="$2"
    local expected_pattern="$3"
    local should_contain="${4:-true}"  # true or false
    
    TESTS_RUN=$((TESTS_RUN + 1))
    
    echo -n "Test ${TESTS_RUN}: ${test_name}... "
    
    # Run the CLI with commands
    local output
    output=$(echo -e "${commands}\nexit" | ${CLI_EXECUTABLE} 2>&1)
    
    # Check if output contains/doesn't contain expected pattern
    if [ "$should_contain" = "true" ]; then
        if echo "${output}" | grep -q "${expected_pattern}"; then
            echo -e "${GREEN}PASSED${NC}"
            TESTS_PASSED=$((TESTS_PASSED + 1))
            return 0
        else
            echo -e "${RED}FAILED${NC}"
            echo "  Expected output to contain: ${expected_pattern}"
            echo "  Actual output:"
            echo "${output}" | sed 's/^/    /'
            return 1
        fi
    else
        if ! echo "${output}" | grep -q "${expected_pattern}"; then
            echo -e "${GREEN}PASSED${NC}"
            TESTS_PASSED=$((TESTS_PASSED + 1))
            return 0
        else
            echo -e "${RED}FAILED${NC}"
            echo "  Expected output NOT to contain: ${expected_pattern}"
            echo "  Actual output:"
            echo "${output}" | sed 's/^/    /'
            return 1
        fi
    fi
}

# Test 1: Help command
run_test "Help command" \
    "help" \
    "Kanban-Lite CLI - Etapa 2"

# Test 2: Board creation
run_test "Board creation" \
    "board create TestBoard" \
    "Board 'TestBoard' created successfully"

# Test 3: Board listing
run_test "Board listing" \
    "board create TestBoard\nboard list" \
    "TestBoard.*Columns: 0"

# Test 4: Column creation
run_test "Column creation" \
    "board create TestBoard\ncolumn add TestBoard ToDo 5" \
    "Column 'ToDo' added to board 'TestBoard' with WIP limit 5"

# Test 5: Column listing
run_test "Column listing" \
    "board create TestBoard\ncolumn add TestBoard ToDo 5\ncolumn add TestBoard Done\ncolumn list TestBoard" \
    "ToDo.*Cards: 0/5"

# Test 6: Card creation
run_test "Card creation" \
    "board create TestBoard\ncolumn add TestBoard ToDo\ncard add TestBoard ToDo TaskExample" \
    "Card 'TaskExample'.*added to column 'ToDo'"

# Test 7: Card listing
run_test "Card listing" \
    "board create TestBoard\ncolumn add TestBoard ToDo\ncard add TestBoard ToDo TaskExample\ncard list TestBoard" \
    "TaskExample.*ID: card_1"

# Test 8: Card movement
run_test "Card movement" \
    "board create TestBoard\ncolumn add TestBoard ToDo\ncolumn add TestBoard Done\ncard add TestBoard ToDo TaskExample\ncard move card_1 ToDo Done TestBoard" \
    "Card 'card_1' moved from 'ToDo' to 'Done'"

# Test 9: Activity history
run_test "Activity history" \
    "board create TestBoard\ncolumn add TestBoard ToDo\nhistory" \
    "Column 'ToDo' added to board 'TestBoard'"

# Test 10: WIP limit enforcement
run_test "WIP limit enforcement" \
    "board create TestBoard\ncolumn add TestBoard ToDo 1\ncard add TestBoard ToDo Task1\ncard add TestBoard ToDo Task2\ncard list TestBoard" \
    "Task1.*ID: card_1"

# Test 11: Board removal
run_test "Board removal" \
    "board create TestBoard\nboard remove TestBoard\nboard list" \
    "No boards found"

# Test 12: Column removal
run_test "Column removal" \
    "board create TestBoard\ncolumn add TestBoard ToDo\ncolumn remove TestBoard ToDo\ncolumn list TestBoard" \
    "No columns found"

# Test 13: Card removal
run_test "Card removal" \
    "board create TestBoard\ncolumn add TestBoard ToDo\ncard add TestBoard ToDo TaskExample\ncard remove TestBoard ToDo card_1\ncard list TestBoard" \
    "no cards"

# Test 14: Error handling - nonexistent board
run_test "Error handling - nonexistent board" \
    "column add NonexistentBoard ToDo" \
    "Error: Board 'NonexistentBoard' not found"

# Test 15: Error handling - invalid command
run_test "Error handling - invalid command" \
    "invalid_command" \
    "Error: Unknown command 'invalid_command'"

# Test 16: Save/Load functionality
run_test "Save functionality" \
    "board create TestBoard\ncolumn add TestBoard ToDo\nsave ${TEST_SAVE_FILE}" \
    "State saved to '${TEST_SAVE_FILE}'"

# Test 17: File save verification
if [ -f "${TEST_SAVE_FILE}" ]; then
    TESTS_RUN=$((TESTS_RUN + 1))
    echo -n "Test $((TESTS_RUN)): Save file creation... "
    if grep -q "BOARD_NAME=TestBoard" "${TEST_SAVE_FILE}"; then
        echo -e "${GREEN}PASSED${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}FAILED${NC}"
        echo "  Save file does not contain expected board data"
    fi
else
    TESTS_RUN=$((TESTS_RUN + 1))
    echo "Test $((TESTS_RUN)): Save file creation... ${RED}FAILED${NC}"
    echo "  Save file was not created"
fi

# Test 18: Load functionality
run_test "Load functionality" \
    "load ${TEST_SAVE_FILE}\nboard list" \
    "State loaded from '${TEST_SAVE_FILE}'"

# Test 19: Multi-board support
run_test "Multi-board support" \
    "board create Project1\nboard create Project2\nboard list" \
    "Project1"

# Test 20: Complex workflow
run_test "Complex workflow" \
    "board create Workflow\ncolumn add Workflow Backlog\ncolumn add Workflow InProgress 2\ncolumn add Workflow Done\ncard add Workflow Backlog Feature1\ncard add Workflow Backlog Feature2\ncard move card_1 Backlog InProgress Workflow\ncard list Workflow\nhistory" \
    "Feature1.*card_1"

# Cleanup
rm -rf "${TEST_OUTPUT_DIR}"

# Summary
echo
echo -e "${YELLOW}======================================${NC}"
echo -e "${YELLOW}           Test Summary               ${NC}"
echo -e "${YELLOW}======================================${NC}"
echo "Tests run: ${TESTS_RUN}"
echo -e "Tests passed: ${GREEN}${TESTS_PASSED}${NC}"
echo -e "Tests failed: ${RED}$((TESTS_RUN - TESTS_PASSED))${NC}"

if [ ${TESTS_PASSED} -eq ${TESTS_RUN} ]; then
    echo -e "\n${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "\n${RED}Some tests failed! ❌${NC}"
    exit 1
fi