#!/system/bin/sh
# 救砖逻辑：启动失败一次即自动卸载本模块
MODDIR=${0%/*}
MARK=/data/adb/.monet_zygisk_boot_inprogress
LOG=/data/adb/.monet_zygisk_boot.log

if [ -f "$MARK" ]; then
  # 上一次启动在 boot_completed 之前就中断了 → 判定失败 → 自杀
  echo "$(date) 上次启动未完成，自动移除本模块" >> "$LOG"
  touch "$MODDIR/remove" 2>/dev/null
  rm -f "$MARK"
  # 尽量把现场恢复（如果之前动过别的东西，在这里撤销）
  exit 0
fi

echo "$(date) 启动开始" >> "$LOG"
touch "$MARK"
