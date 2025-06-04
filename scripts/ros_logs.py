#!/usr/bin/env python3
import os
import re
from datetime import datetime

ROOT_LOG_DIR = "/root/Auto-Filmer/log"
ROSOUT_FILENAME = "rosout.log"
OUTPUT_PREFIX = "aflog"

def clean_and_reorder_line(line):
    line = line.strip()
    if not line:
        return None

    # 先匹配时间戳 + 余下部分
    m = re.match(r"^(\d+\.\d+)\s+(.*)", line)
    if not m:
        return None
    raw_ts, rest = m.groups()

    # 转换时间戳为标准时间格式
    try:
        float_ts = float(raw_ts)
        dt_str = datetime.fromtimestamp(float_ts).strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
    except Exception:
        return None

    # 去除 [topics: ...]
    rest = re.sub(r"\s*\[topics: [^\]]+\]", "", rest)

    # 正则拆解 rest 部分
    # 结构假设：<LEVEL> <MODULE> [<FILE:LINE(FUNC)>] <MESSAGE>
    # 例如：INFO /manager [/path/to/file.cpp:202(func)] no map!

    pattern = re.compile(
        r"^(DEBUG|INFO|WARN|ERROR|FATAL)\s+"     # 日志级别
        r"(\S+)\s+"                              # 模块名
        r"(\[[^\]]+\])\s*"                       # 源码路径
        r"(.*)$"                                # 消息体
    )

    m2 = pattern.match(rest)
    if not m2:
        # 如果不匹配上面格式，直接返回原始行（带时间戳）
        return f"[{dt_str}] {raw_ts} {rest}"

    level, module, source, msg = m2.groups()

    # 按要求调整顺序： 时间戳 + level + 消息 + module + source
    # return f" {module} {source} \n [{dt_str}] {level}  {msg}"
    return f"[{dt_str}] {level}  {msg}"
def clean_and_reorder_line2(line):
    line = line.strip()
    if not line:
        return None

    # 先匹配时间戳 + 余下部分
    m = re.match(r"^(\d+\.\d+)\s+(.*)", line)
    if not m:
        return None
    raw_ts, rest = m.groups()

    # 转换时间戳为标准时间格式
    try:
        float_ts = float(raw_ts)
        dt_str = datetime.fromtimestamp(float_ts).strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
    except Exception:
        return None

    # 去除 [topics: ...]
    rest = re.sub(r"\s*\[topics: [^\]]+\]", "", rest)

    # 正则拆解 rest 部分
    # 结构假设：<LEVEL> <MODULE> [<FILE:LINE(FUNC)>] <MESSAGE>
    # 例如：INFO /manager [/path/to/file.cpp:202(func)] no map!

    pattern = re.compile(
        r"^(DEBUG|INFO|WARN|ERROR|FATAL)\s+"     # 日志级别
        r"(\S+)\s+"                              # 模块名
        r"(\[[^\]]+\])\s*"                       # 源码路径
        r"(.*)$"                                # 消息体
    )

    m2 = pattern.match(rest)
    if not m2:
        # 如果不匹配上面格式，直接返回原始行（带时间戳）
        return f"[{dt_str}] {raw_ts} {rest}"

    level, module, source, msg = m2.groups()

    # 按要求调整顺序： 时间戳 + level + 消息 + module + source
    # return f" {module} {source} \n [{dt_str}] {level}  {msg}"
    return f"[{dt_str}] {level}  {msg} -> {source} "

def get_timestamp_from_file(filepath):
    t = os.path.getmtime(filepath)
    return datetime.fromtimestamp(t).strftime("%Y%m%d-%H%M%S")

def find_rosout_files(root_dir):
    found_files = []
    for subdir, dirs, files in os.walk(root_dir):
        if ROSOUT_FILENAME in files:
            found_files.append(os.path.join(subdir, ROSOUT_FILENAME))
    return found_files

def main():
    rosout_files = find_rosout_files(ROOT_LOG_DIR)
    if not rosout_files:
        print("未找到任何 rosout.log 文件。")
        return

    for filepath in rosout_files:
        timestamp = get_timestamp_from_file(filepath)
        output_filename = f"{OUTPUT_PREFIX}-{timestamp}.log"
        output_path = os.path.join(ROOT_LOG_DIR, output_filename)
        output_path2 = os.path.join(ROOT_LOG_DIR, 'v'+output_filename)

        with open(filepath, "r") as infile, open(output_path, "w") as outfile:
            for line in infile:
                cleaned = clean_and_reorder_line(line)
                if cleaned:
                    outfile.write(cleaned + "\n")
        with open(filepath, "r") as infile, open(output_path2, "w") as outfile:
            for line in infile:
                cleaned = clean_and_reorder_line2(line)
                if cleaned:
                    outfile.write(cleaned + "\n")

        print(f"✅ new log: {output_path}")

if __name__ == "__main__":
    main()
