/*
Copyright 2016 Gu Zhengxiong <rectigu@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


# ifndef CPP
// register_module_notifier, unregister_module_notifier.
# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/notifier.h> // struct notifier_block.
# endif // CPP

# include "lib/lib.h"


MODULE_LICENSE("GPL");

int
fake_init(void);
void
fake_exit(void);

int
module_notifier(struct notifier_block *nb,
                unsigned long action, void *data);

struct notifier_block nb = {
    .notifier_call = module_notifier,
    .priority = INT_MAX
};


int
init_module(void)
{
    fm_alert("%s\n", "Greetings the World!");

    register_module_notifier(&nb);

    return 0;
}


void
cleanup_module(void)
{
    unregister_module_notifier(&nb);

    fm_alert("%s\n", "Farewell the World!");
    return;
}


int
module_notifier(struct notifier_block *nb,
                unsigned long action, void *data)
{
    struct module *module;
    unsigned long flags;
    DEFINE_SPINLOCK(module_notifier_spinlock);

    module = data;
    fm_alert("Processing the module: %s\n", module->name);

    spin_lock_irqsave(&module_notifier_spinlock, flags);
    switch (module->state) {
    case MODULE_STATE_COMING:
        fm_alert("Replacing init and exit functions: %s.\n",
                 module->name);
        module->init = fake_init;
        module->exit = fake_exit;
        break;
    default:
        break;
    }
    spin_unlock_irqrestore(&module_notifier_spinlock, flags);

    return NOTIFY_DONE;
}


int
fake_init(void)
{
    fm_alert("%s\n", "Fake init.");

    return 0;
}


void
fake_exit(void)
{
    fm_alert("%s\n", "Fake exit.");

    return;
}
