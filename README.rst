English `中文 <README-zh_CN.rst>`_

the Research Rootkit project
============================


Notice
------

The directory structure of this repository might change drastically
without any notifications.


Library
-------

There are some components
which are intended to be reused over and over again.

But note that they can always be improved or even corrected.

::

   unsigned long **
   get_sys_call_table(void);

   void
   disable_write_protection(void);
   void
   enable_write_protection(void);

   char *
   join_strings(const char *const *strings, const char *delim,
                char *buff, size_t count);
   void
   print_memory(void *addr, size_t count, const char *prompt);


Available Courses
-----------------

Course 1: Modifying / Hooking sys_call_table
++++++++++++++++++++++++++++++++++++++++++++

- Experiment 1: hello

  Hello World! kernel module.

- Experiment 2: sys_call_table

  Get sys_call_table's address by brute-force memory searching
  starting from PAGE_OFFSET.

- Experiment 3: write_protection

  Disable or enable Write Protection via CR0 register.

- Roundup Experiment 1: fsmon

  A **primitive** file monitor based on system call hooking.

  Hooked functions: ``open``, ``unlink``, ``unlinkat``.

- Roundup Experiment 2: psmon

  A **primitive** process monitor via system call hooking.

  Hooked functions: ``execve``.

  - **Notice**

    I have seen reports that the method used in this experiment
    would not work normally
    due to inconsistent ABI of ``stub_execve``,
    which requires further investigation.

- Roundup Experiment 3: fshid

  A **primitive** file-hiding demonstration
  using system call hooking.

  Hooked functions: ``getdents``, ``getdents64``.


Projects Of Interests
---------------------

- `mncoppola/suterusu`_

  An LKM rootkit targeting Linux 2.6/3.x on x86(_64), and ARM.


References & Further Readings
-----------------------------

- `Suterusu Rootkit: Inline Kernel Function Hooking on x86 and ARM <https://poppopret.org/2013/01/07/suterusu-rootkit-inline-kernel-function-hooking-on-x86-and-arm/>`_


.. _mncoppola/suterusu: https://github.com/mncoppola/suterusu
