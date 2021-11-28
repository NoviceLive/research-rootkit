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


# include <linux/module.h>
# include <linux/kernel.h>

# include "zeroevil/zeroevil.h"


MODULE_LICENSE("GPL");


int
init_module(void)
{
  unsigned long cr0;
  unsigned long cr4;

  fm_alert("%s\n", "Greetings the World!");

  cr0 = read_cr0();
  fm_alert("Old: %d\n", test_bit(X86_CR0_WP_BIT, &cr0));

  disable_wp();
  cr0 = read_cr0();
  fm_alert("New: %d\n", test_bit(X86_CR0_WP_BIT, &cr0));

  enable_wp();
  cr0 = read_cr0();
  fm_alert("Now: %d\n", test_bit(X86_CR0_WP_BIT, &cr0));

  cr4 = __read_cr4();
  fm_alert("old cr4 = %lx", cr4);

  return 0;
}


void
cleanup_module(void)
{
  fm_alert("%s\n", "Farewell the World!");

  return;
}
