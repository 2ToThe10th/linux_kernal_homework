# Keyboard click counter

Count number of keyboard click by user for every minute and write it to kernel logs

**Usage**

To install module:
```bash
make
insmod keyboard_click_counter.ko
```

To remove module:
```bash
rmmod keyboard_click_counter.ko
```

To watch statistics:
```bash
dmesg # better to use with tail
```
