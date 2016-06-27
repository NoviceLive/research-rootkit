English `中文 <README-zh_CN.rst>`_

the Research Rootkit project
============================


Notice
------

The directory structure of this repository might change drastically
without any notifications.


Available Courses
-----------------

Course 1: Modifying / Hooking the sys_call_table
++++++++++++++++++++++++++++++++++++++++++++++++

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

  Hidden files: ``032416_525.mp4``.

  - **Notice**

    There is no hooking logic in ``getdents64``
    since I have not triggered it yet.


Course 2: Implementing fundamental functionalities of rootkits
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- Experiment 1: root

  Providing a root backdoor.

- Experiment 2: komon

  Preventing modules from initializing and functioning
  by substituting their init and exit functions
  when ``MODULE_STATE_COMING`` is notified to module notifiers.

- Experiment 3: fshid

  Hiding files by hooking ``filldir``.

- Experiment 4: pshid

  Hiding processes by hiding entries under ``/proc``.

- Experiment 5: pthid

  Hiding ports by filtering contents
  in ``/proc/net/tcp`` and the like
  by hooking the ``show`` function of their ``seq_file`` interfaces.

- Experiment 6: kohid

  Hiding modules by hiding entries in ``/sys/module``
  and filtering contents of ``/proc/modules``
  by hooking its ``show`` function.

  This experiment combines the techniques demonstrated
  in ``Experiment 4: pshid`` and ``Experiment 5: pthid``.


Projects Of Interests
---------------------

- `mncoppola/suterusu`_

  An LKM rootkit targeting Linux 2.6/3.x on x86(_64), and ARM.

- `ivyl/rootkit`_

  Sample Rootkit for Linux

- `cloudsec/brootkit`_

  Lightweight rootkit implemented by bash shell scripts v0.10.


References & Further Readings
-----------------------------

- `Suterusu Rootkit: Inline Kernel Function Hooking on x86 and ARM <https://poppopret.org/2013/01/07/suterusu-rootkit-inline-kernel-function-hooking-on-x86-and-arm/>`_


.. _mncoppola/suterusu: https://github.com/mncoppola/suterusu
.. _ivyl/rootkit: https://github.com/ivyl/rootkit
.. _cloudsec/brootkit: https://github.com/cloudsec/brootkit
