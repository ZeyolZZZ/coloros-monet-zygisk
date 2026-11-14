#!/system/bin/sh
# 进系统后清除"启动进行中"标记；若未完成则下次开机自动卸载
MARK=/data/adb/.monet_zygisk_boot_inprogress
i=0
while [ "$(getprop sys.boot_completed)" != "1" ] && [ $i -lt 180 ]; do
  sleep 2; i=$((i+1))
done
if [ "$(getprop sys.boot_completed)" = "1" ]; then
  rm -f "$MARK"
  echo "$(date) 启动完成" >> /data/adb/.monet_zygisk_boot.log
fi
