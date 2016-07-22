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
    unsigned long **real_sct;
    unsigned long **lstar_sct;
    phys_addr_t real_phys;
    phys_addr_t lstar_phys;

    fm_alert("%s\n", "Greetings the World!");

    real_sct = get_sct_via_sys_close();
    lstar_sct = get_lstar_sct();

    if (real_sct == NULL || lstar_sct == NULL) {
        return 1;
    }

    fm_alert("%s\n", "==> According to sys_close:");
    fm_alert("virt: %p\n", real_sct);
    real_phys = virt_to_phys(real_sct);
    fm_alert("phys: %llx\n", real_phys);

    fm_alert("%s\n", "==> According to entry_SYSCALL_64:");
    fm_alert("virt: %p\n", lstar_sct);
    lstar_phys = virt_to_phys(lstar_sct);
    fm_alert("phys: %llx\n", lstar_phys);

    if (real_phys == lstar_phys) {
        fm_alert("%s\n", "==> Matched.");
    } else {
        fm_alert("%s\n", "==> Patching to that from sys_close...");
        set_lstar_sct((u32)(unsigned long)phys_to_virt_kern(real_phys));
    }

    return 0;
}


void
cleanup_module(void)
{
    fm_alert("%s\n", "Farewell the World!");

    return;
}
