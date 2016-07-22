// Copyright 2016 Gu Zhengxiong <rectigu@gmail.com>
//
// This file is part of LibZeroEvil.
//
// LibZeroEvil is free software:
// you can redistribute it and/or modify it
// under the terms of the GNU General Public License
// as published by the Free Software Foundation,
// either version 3 of the License,
// or (at your option) any later version.
//
// LibZeroEvil is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LibZeroEvil.
// If not, see <http://www.gnu.org/licenses/>.


# ifndef CPP
# include <linux/module.h>
# include <linux/kernel.h>
# endif // CPP

# include "zeroevil/zeroevil.h"


MODULE_LICENSE("GPL");


int
init_module(void)
{
    unsigned long **sct1;
    unsigned long **sct2;

    fm_alert("%s\n", "Greetings the World!");

    sct1 = get_sct_via_sys_close();
    sct2 = get_lstar_sct();

    if (sct1 == NULL || sct2 == NULL) {
        return 1;
    }

    fm_alert("%s\n", "==> According to sys_close:");
    fm_alert("virt: %p\n", sct1);
    fm_alert("phys: %llx\n", virt_to_phys(sct1));
    fm_alert("%s\n", "==> According to entry_SYSCALL_64:");
    fm_alert("virt: %p\n", sct2);
    fm_alert("phys: %llx\n", virt_to_phys(sct2));

    fm_alert("%s\n", "==> Do they match?");

    return 0;
}


void
cleanup_module(void)
{
    fm_alert("%s\n", "Farewell the World!");

    return;
}
