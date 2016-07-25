English `中文 <README-zh_CN.rst>`_

LibZeroEvil & the Research Rootkit project
==========================================

This is ``LibZeroEvil & the Research Rootkit project``,
in which there are step-by-step, experiment-based courses
that help to get you started and keep your hands dirty
with offensive or defensive development in the Linux kernel,
and also guide you with demonstrative examples
through the underlying core library, ``LibZeroEvil``,
which **attempts** to be
a real-world consumable programming framework
for any evil or good kernel-land invaders or defenders.

That being said, it's just the beginning
and ``LibZeroEvil`` is still in its infancy,
serving educational purposes mainly.

Warning
-------

It's never recommended to perform kernel module experiments
on a physical machine, unless the owner will never complain about
frequent rebooting or forced halting and possible data or work loss.

You have been warned.

Guidelines on Creating New Issues or Contributing
-------------------------------------------------

If a course doesn't compile,
or do compile but doesn't work as expected,
e.g. crashing or hanging your system,
feel free to `create a new issue`_.
But before that, consider the following.

.. _create a new issue: https://github.com/NoviceLive/research-rootkit/issues

1. Search existing issues to ensure that it won't be duplicated.

2. Attach detailed information of your system, e.g., ``uname --all``,
   and what your compiler throws on you,
   i.e., the error information,
   so that others can successfully reproduce your issue
   and manage to help you out.

   If you can't figure out which error is the most significant one,
   paste all of them verbatim inside Markdown triple quotes.

3. Remember that kernel compatibility issues
   are the most common ones, since the author paid virtually no
   attention to that. However,
   it's my pleasure to learn about and fix them.

4. Some courses are x64 only for the time being.

Information on Kernel Compatibility
-----------------------------------

Tested Kernel
+++++++++++++

My major development environment.

- Kali

  ``Linux anon 4.6.0-kali1-amd64 #1 SMP Debian 4.6.3-1kali1 (2016-07-12) x86_64 GNU/Linux``.

Compilable Kernel
+++++++++++++++++

That is, ``./tests/makeall.sh --quiet`` reports no error,
but I haven't tested the functionality.

- Arch

  ``Linux anon 4.6.4-1-ARCH #1 SMP PREEMPT Mon Jul 11 19:12:32 CEST 2016 x86_64 GNU/Linux``.

- Ubuntu 14.04

  ``Linux anon 4.2.0-42-generic #49~14.04.1-Ubuntu SMP Wed Jun 29 20:22:11 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux``

Known Unresolved Compatibility Issues
+++++++++++++++++++++++++++++++++++++

- ``struct dir_context`` doesn't exist on kernel version 3.10 and earlier.

  For example, CentOS 7,
  ``Linux localhost.localdomain 3.10.0-327.22.2.el7.x86_64 #1 SMP Thu Jun 23 17:05:11 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux``.

Notice on Directory Structure
-----------------------------

The directory structure of this repository might change drastically
without any notifications.

Available Courses
-----------------

More courses might be designed and added here later.

Course 1: Modifying / Hooking the ``sys_call_table``
++++++++++++++++++++++++++++++++++++++++++++++++++++

- Experiment 1: ``hello``

  Hello World! kernel module.

- Experiment 2: ``sys_call_table``

  Get ``sys_call_table`` 's address by brute-force memory searching
  starting from ``PAGE_OFFSET``.

- Experiment 3: ``write_protection``

  Disable or enable ``Write Protection`` via the ``CR0`` register.

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

  Hidden files: ``032416_525.mp4``.

Course 2: Implementing fundamental functionalities of rootkits
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- Experiment 1: ``root``

  Providing a root backdoor.

- Experiment 2: ``komon``

  Preventing modules from initializing and functioning
  by substituting their init and exit functions
  when ``MODULE_STATE_COMING`` is notified to module notifiers.

- Experiment 3: ``fshid``

  Hiding files by hooking ``filldir``.

- Experiment 4: ``pshid``

  Hiding processes by hiding entries under ``/proc``.

- Experiment 5: ``pthid``

  Hiding ports by filtering contents
  in ``/proc/net/tcp`` and the like
  by hooking the ``show`` function of their ``seq_file`` interfaces.

- Experiment 6: ``kohid``

  Hiding modules by hiding entries in ``/sys/module``
  and filtering contents of ``/proc/modules``
  by hooking its ``show`` function.

  This experiment combines the techniques demonstrated
  in ``Experiment 4: pshid`` and ``Experiment 5: pthid``.

Course 3: Infecting critical kernel modules for persistence and more
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- Experiment 1: ``elf``

  Providing elementary materials on ELF parsing and modifying.

  This experiment implemented an essential tool, i.e. ``setsym``,
  for following experiments,
  and also two trivial tools,
  ``lssec`` resembling ``readelf -S``
  and ``lssym`` resembling ``readelf -s`` / ``objdump -t``.

  **They are coded for 64-bit ELF only,
  but it shouldn't be difficult to adapt.**

- Experiment 2: ``noinj``

  Hijacking / Hooking the init and exit function of the module with
  functions in the same module by modifying the symbol table.

- Experiment 3: ``codeinj``

  Injecting the adapted ``fshid`` (See Experiment 3 of Course 2)
  into a demonstrative simple module
  (i.e. without ``static __init`` or ``static __exit``),
  and hooking / hijacking its init and exit functions
  by modifying the symbol table.

- Roundup Experiment: ``real``

  Injecting the adapted ``fshid`` (See Experiment 3 of Course 2)
  into a real-world kernel module
  (i.e. with ``static __init`` or ``static __exit``)
  by linking,
  and hooking / hijacking its init and exit functions
  by modifying the symbol table.

Course 4: Modifying / Patching the ``entry_SYSCALL_64``
+++++++++++++++++++++++++++++++++++++++++++++++++++++++

**This is x64 only. However, it's not difficult to adapt.**

- Experiment 1: get

  Getting the ``sys_call_table`` 's address
  in the machine code of ``entry_SYSCALL_64``
  by searching the identifying bytes ``ff 14 c5``.

- Experiment 2: set

  Patching the ``sys_call_table`` 's address
  in the machine code of ``entry_SYSCALL_64``
  with a faked but innocuous, i.e. unmodified, one.

- Experiment 3: rec

  Recovering the ``sys_call_table`` 's address
  in the machine code of ``entry_SYSCALL_64`` to that
  obtained via ``sys_close`` -based memory searching.

- Roundup Experiment: ifmon

  Monitoring network flow (especially ``GET`` & ``POST``)
  by hooking ``sys_sendto`` using the method demonstrated
  in the above three experiments on ``entry_SYSCALL_64``.

Course 5: Inline Hooking
++++++++++++++++++++++++

- Experiment 1: ``jmp``

  Patching the starting bytes of target functions with
  control flow redirection instructions,
  e.g. ``PUSH RET``, ``JMP`` or ``INT``,
  which transfer control to our function,
  where our tasks are performed,
  including restoring those bytes and
  invoking the victim function if necessary.

Projects Of Interests
---------------------

- `mncoppola/suterusu`_

  An LKM rootkit targeting Linux 2.6/3.x on x86(_64), and ARM.

- `maK-/maK_it-Linux-Rootkit`_

  This is a linux rootkit using many of the techniques described on http://r00tkit.me.

- `ivyl/rootkit`_

  Sample Rootkit for Linux.

- `cloudsec/brootkit`_

  Lightweight rootkit implemented by bash shell scripts v0.10.

References & Further Readings
-----------------------------

- `Suterusu Rootkit: Inline Kernel Function Hooking on x86 and ARM <https://poppopret.org/2013/01/07/suterusu-rootkit-inline-kernel-function-hooking-on-x86-and-arm/>`_
- `Infecting loadable kernel modules: kernel versions 2.6.x/3.0.x <http://phrack.org/issues/68/11.html#article>`_

.. _mncoppola/suterusu: https://github.com/mncoppola/suterusu
.. _maK-/maK_it-Linux-Rootkit: https://github.com/maK-/maK_it-Linux-Rootkit
.. _ivyl/rootkit: https://github.com/ivyl/rootkit
.. _cloudsec/brootkit: https://github.com/cloudsec/brootkit
