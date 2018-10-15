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
noinj_init(void)
{
    fm_alert("noinj: %s\n", "Greetings the World!");

    return 0;
}


void
noinj_exit(void)
{
    fm_alert("noinj: %s\n", "Farewell the World!");

    return;
}


module_init(noinj_init);
module_exit(noinj_exit);


int
fake_init(void)
{
    noinj_init();

    fm_alert("==> NOINJ: %s\n", "GR33TINGS THE W0RLD!");

    return 0;
}


int
fake_exit(void)
{
    noinj_exit();

    fm_alert("==> NOINJ: %s\n", "FAR3W311 THE W0RLD!");

    return 0;
}
