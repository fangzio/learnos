#! /bin/bash

APP_DIR=$(cd $(dirname "${BASH_SOURCE[0]}") && pwd)
EXEC_DIR="exec"

if [ ! -d "exec/sig" ];then
    mkdir -p "exec/sig"
fi

src_arr=("sig")

for tdir in $src_arr
do
    # 遍历c文件
    for file in $(ls $APP_DIR/$tdir)
    do
        if [ "${file##*.}"x = "c"x ];then
        /usr/bin/gcc "$APP_DIR/$tdir/$file" -o "$APP_DIR/$EXEC_DIR/$tdir/${file%.*}"
        fi
    done
done 