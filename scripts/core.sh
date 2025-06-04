#!/bin/bash

echo "=== Testing Core Dump Configuration ==="
echo

# 检查当前配置
echo "Current ulimit -c: $(ulimit -c)"
echo "Core pattern: $(cat /proc/sys/kernel/core_pattern)"
echo "Available disk space:"
df -h /tmp /root/Auto-Filmer

echo
echo "=== Creating test crash program ==="

# 创建会崩溃的测试程序
cat > /tmp/crash_test.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("About to crash...\n");
    
    // 故意造成段错误
    int *p = NULL;
    *p = 42;
    
    return 0;
}
EOF

# 编译测试程序
gcc -g -o /tmp/crash_test /tmp/crash_test.c

echo "Test program created: /tmp/crash_test"
echo

# 设置core dump限制
ulimit -c unlimited
echo "Set ulimit -c unlimited"

echo "=== Running crash test ==="
cd /tmp
./crash_test

echo
echo "=== Checking for core files ==="
ls -la /tmp/core* 2>/dev/null || echo "No core files found in /tmp"
ls -la core* 2>/dev/null || echo "No core files found in current directory"

# 如果使用systemd-coredump，检查coredumpctl
echo
echo "=== Checking systemd coredumps ==="
coredumpctl list --no-pager 2>/dev/null || echo "coredumpctl not available or no dumps"

echo
echo "=== Core dump test completed ==="