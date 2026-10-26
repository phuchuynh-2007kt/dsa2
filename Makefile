SHELL := /bin/bash
CXX := g++
CXXFLAGS := -std=c++17 -O2 -g -Wall -Wextra -pedantic -I.
BUILD_DIR := .build
.DEFAULT_GOAL := help
.PHONY: help q1a q1b q2 q3 q4 all demo clean check-q1a check-q1b check-q2 check-q3 check-q4 check-rules

help:
	@echo "BKMove Assignment 2"
	@echo "  make q1a             Q1a BST public tests (9 cases)"
	@echo "  make q1b             Q1b AVL public tests (10 cases)"
	@echo "  make q2              Q2 Heap public tests (10 cases)"
	@echo "  make q3              Q3 Stop Finder public tests (10 cases)"
	@echo "  make q4              Q4 Arrival Board public tests (12 cases)"
	@echo "  make q3 TEST=4       Run only test case 4 of Q3"
	@echo "  make all             Run all 51 public test cases"
	@echo "  make check-rules     Basic source-rule checks"
	@echo "  make demo            Compile example program"
	@echo "  make clean           Remove build files"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Basic feedback checks, not a complete academic-integrity or complexity check.
check-q1a:
	@bash tests/check_source_rules.sh BST.h
check-q1b:
	@bash tests/check_source_rules.sh BST.h AVL.h
check-q2:
	@bash tests/check_source_rules.sh Heap.h
check-q3:
	@bash tests/check_source_rules.sh BST.h AVL.h BKMove.h BKMove.cpp
check-q4:
	@bash tests/check_source_rules.sh BST.h AVL.h Heap.h BKMove.h BKMove.cpp
check-rules:
	@bash tests/check_source_rules.sh BST.h AVL.h Heap.h BKMove.h BKMove.cpp

q1a: check-q1a | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) tests/q1a/test_q1a.cpp -o $(BUILD_DIR)/q1a
	@$(BUILD_DIR)/q1a $(TEST)
q1b: check-q1b | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) tests/q1b/test_q1b.cpp -o $(BUILD_DIR)/q1b
	@$(BUILD_DIR)/q1b $(TEST)
q2: check-q2 | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) tests/q2/test_q2.cpp -o $(BUILD_DIR)/q2
	@$(BUILD_DIR)/q2 $(TEST)
q3: check-q3 | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) BKMove.cpp tests/q3/test_q3.cpp -o $(BUILD_DIR)/q3
	@$(BUILD_DIR)/q3 $(TEST)
q4: check-q4 | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) BKMove.cpp tests/q4/test_q4.cpp -o $(BUILD_DIR)/q4
	@$(BUILD_DIR)/q4 $(TEST)

demo: check-rules | $(BUILD_DIR)
	@$(CXX) $(CXXFLAGS) BKMove.cpp main.cpp -o $(BUILD_DIR)/demo

all: q1a q1b q2 q3 q4
	@echo "All 51 public test cases passed."
clean:
	@rm -rf $(BUILD_DIR)
