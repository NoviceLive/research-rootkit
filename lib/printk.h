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


# ifndef _GU_ZHENGXIONG_PRINTK_H
# define _GU_ZHENGXIONG_PRINTK_H


# ifndef CPP
# include <linux/printk.h>
# include <linux/module.h>
# endif // CPP


// INFO: ``fn`` is short for ``__func__``.
# define fn_printk(level, fmt, ...)                     \
    printk(level "%s: " fmt, __func__, ##__VA_ARGS__)

// INFO: ``fm`` is short for ``__func__`` and ``module``.
# define fm_printk(level, fmt, ...)                     \
    printk(level "%s.%s: " fmt,                        \
           THIS_MODULE->name, __func__, ##__VA_ARGS__)


// INFO: I only use ``pr_alert`` at present.
// TODO: When wanting more, e.g. ``pr_info``, I will add them.
# define fn_alert(fmt, ...) \
    fn_printk(KERN_ALERT, fmt, ##__VA_ARGS__)

# define fm_alert(fmt, ...) \
    fm_printk(KERN_ALERT, fmt, ##__VA_ARGS__)


# endif // printk.h
