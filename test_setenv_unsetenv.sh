#!/bin/bash

echo "=== Testing setenv and unsetenv ==="
echo ""

echo "1. Test setenv with one parameter:"
echo "setenv HBTN" | ./hsh
echo ""

echo "2. Test setenv without parameters:"
echo "setenv" | ./hsh
echo ""

echo "3. Test setenv with existing variable:"
echo -e "env | grep PATH\nsetenv PATH new_path_value\nenv | grep PATH" | ./hsh
echo ""

echo "4. Test setenv with new variable:"
echo -e "env | grep TEST_VAR\nsetenv TEST_VAR test_value\nenv | grep TEST_VAR" | ./hsh
echo ""

echo "5. Test unsetenv with existing variable:"
echo -e "setenv TEMP_VAR temp_value\nenv | grep TEMP_VAR\nunsetenv TEMP_VAR\nenv | grep TEMP_VAR" | ./hsh
echo ""

echo "6. Test unsetenv with non-existing variable:"
echo "unsetenv NON_EXISTENT" | ./hsh
echo ""

echo "7. Test unsetenv without parameters:"
echo "unsetenv" | ./hsh
echo ""

echo "=== All tests completed ==="
