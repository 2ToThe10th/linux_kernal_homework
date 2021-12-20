//
// Created by 2ToThe10th on 20.12.2021.
//

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/notifier.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");

atomic_t number_of_keyboard_click = ATOMIC_INIT(0);
static struct timer_list keyboard_click_counter_timer;

#ifdef notifier_block
struct notifier_block {
  int (*notifier_call)(struct notifier_block *, unsigned long, void *);
  struct notifier_block *next;
  int priority;
};
#endif

int keyboard_click_counter_notify(struct notifier_block *nblock, unsigned long code, void *_param) {
  struct keyboard_notifier_param *param = _param;
  if (code == KBD_KEYCODE && param->down) {
    atomic_inc(&number_of_keyboard_click);
  }
  return NOTIFY_OK;
}

static struct notifier_block nb = {
    .notifier_call = keyboard_click_counter_notify
};

void keyboard_click_counter_timer_callback(struct timer_list* data) {
  int saved_number_of_keyboard_click = atomic_xchg(&number_of_keyboard_click, 0);
  printk(KERN_INFO "User clicked %d times for last minute\n", saved_number_of_keyboard_click);

  mod_timer(&keyboard_click_counter_timer, jiffies + msecs_to_jiffies(60 * 1000));
}

static int keyboard_click_counter_init(void) {
  printk(KERN_INFO "Loading keyboard_click_counter module...\n");
  register_keyboard_notifier(&nb);
  timer_setup(&keyboard_click_counter_timer, keyboard_click_counter_timer_callback, 0);
  mod_timer(&keyboard_click_counter_timer, jiffies + msecs_to_jiffies(60 * 1000));

  return 0;
}

static void keyboard_click_counter_release(void) {
  unregister_keyboard_notifier(&nb);
  del_timer(&keyboard_click_counter_timer);
  printk(KERN_INFO "Goodbye Mr. keyboard_click_counter\n");
}

module_init(keyboard_click_counter_init);
module_exit(keyboard_click_counter_release);