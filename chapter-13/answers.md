# Questions
1. The first Linux tool you should check is the very simple tool `free`. First, type `man free` and read its entire manual page; it's short, don't worry!

* Displays amount of free and used memory in the system.
  * Information from /proc/meminfo.
* Definitions
  * total -> MemTotal + SwapTotal
  * used -> total - available
  * free (unused memory) -> Memfree + SwapFree
  * shared -> Shmem
    * Used by tmpfs
  * bufffers -> Buffers
    * Used by kernel buffers
  * cache -> Cached + SReclaimable
    * Used by page cache and slabs
  * buff/cache -> bufffers + cache
  * available
    * Estimation of how much memory is availale for starting new applications, without swapping. Unlike the data provided by the cache or free fields, this field takes into account page cache and also that not all reclaimable memory slabs will be reclaimed due to items being in use.

* Questions
  * Why is `used` not just `(total - free)`?
    * In context of `free` command, `used` represents memory currently allocated to processes, whereas `free` is memory that is absolutely empty **zeros.
    * If you subtract `free` from `total`, you get a number that includes `buffers` and `cache`. The `used` column specifically excludes those because buffers and cache are considered reclaimable. The formula used by tool is: `used = total - free - buffers - cache`
  * Why do we have `availble`, isn't `free` enough? Also, vice versa, if `available` is already there, why need `free`? Why have two definitions meaning the same thing?
    * The `free` value is almost always very low on a healthy Linux system. This is because Linux follows the philosophy: "Free RAM is wasted RAM". If RAM isn't being used by an app, the kernel uses it to cache files from the disk to speed things up.
      * `free`: Memory that is doing absolutely nothing.
      * `available`: The "real" number you care about. It is an estimate of how much memory can be given to a new process immediately by dropping caches or reclaiming slabs, without forcing the system to use the slow Swap space on your disk.
    * Analogy: `free` is like having an empty shelf in your closet. `available` is like having a shelf full of old newspapers that you are willing to throw away the second you buy a new book. The shelf isn't empty, but the space is available. 
  * Does `shared` belong to `free` or `used`?
    * Refer to table below.
  * Does `buffers` belong to `free` or `used`?
    * Refer to table below.
  * Does `cache` belong to `free` or `used`?
    * Refer to table below.
  * Does `buff/cache` belong to `free` or `used`?
    * Refer to table below.

---

| Field | Category | Why? |
| -- | -- | -- |
| Shared | Used | This Reperesents memory used by `tmpfs` (like `/dev/shm`) and shared memory segments. It cannot be easily reclaimed to start a new app |
| Buffers | Free-ish | These are temporary headers for raw disk blocks. They are technically "in use" by the kernel, but can be discarded quickly if an application needs the space. |
| Cache | Free-ish | This is the Page Cache (files read from disk). This is technically "in use", but since the data exists on the disk anyway, the kernel can delete it from RAM instantly to make room for an app. |
| Buff/Cache | Free-ish | This is simply the sum of the two above. For a system administrator, this is "Available" space. |

---

2. Now, run `free`, perhaps using some of the arguments that might be useful (e.g. `-m`, to display memory totals in megabytes). How much memory is in your system? How much is free? Do these numbers match your intuition?

* I have created a VM on my machine and allocated ~8GB memory.
  * It shows that I have total memory of 7994 megabytes which is matching my intuition.

---

3. Next, create a little program that uses a certain amount of memory, called `memory-user.c`. This program should take on command-line argument: the number of megabytes of memory it will use. When run, it should allocate an array, and constantly stream through the array, touching each entry. The program should do this indefinitely, or, perhaps, for a certain amount of time also specified at the command line.

* See `memory-user.c`.

---

4. Now, while running your memory-user program, also (in a different terminal window, but on the same machine) run the free
tool. How do the memory usage totals change when your program
is running? How about when you kill the memory-user program?
Do the numbers match your expectations? Try this for different
amounts of memory usage. What happens when you use really
large amounts of memory?

* I learned that just doing 'malloc' does not actually use the RAM immediately. The OS has this behavior that if you allocate memory, but not use it yet in code, it does not actually use memory yet so it won't reflect memory usage when you use `free`.
  * Only when you actually work with the memory you allocated with malloc does the OS really allocate the memory in RAM. In code, this is done be writing to it.
* Expectations when running the program are met i.e. when I run the program `used` gets increased with approximate allocated size, and when program is exited, it goes back to original `usage`.
* When I used large amount of memory, at first, I saw it utilizing almost all memory, then I saw it utilizing the swap space, then after a while, the process was killed.
  * I also say that when I allocated a very big (1000000000000000 bytes) what actually happened is that in the code, it was not able to finish a loop, it crashed before being able to write to memory.

---

5. Let’s try one more tool, known as pmap. Spend some time, and read
the pmap manual page in detail.

* Report memory map of a process.

---

6. To use pmap, you have to know the process ID of the process you’re
interested in. Thus, first run ps auxw to see a list of all processes;
then, pick an interesting one, such as a browser. You can also use
your memory-user program in this case (indeed, you can even
have that program call getpid() and print out its PID for your
convenience).

* Implemented in code!

---

7. Now run pmap on some of these processes, using various flags (like
-X) to reveal many details about the process. What do you see?
How many different entities make up a modern address space, as
opposed to our simple conception of code/stack/heap?

* I saw the code segment (text, rodata, data, BSS).
  * text which are the instructions has a mode of read and execute.
  * rodata has a mode of read only.
  * data and BSS has a mode of read and write.

* Aside from stack and heap, I also found the mapped shared libraries.
* There are [anon] too but not too sure what it is yet, maybe I will learn it in the future.

---

8. Finally, let’s run pmap on your memory-user program, with different amounts of used memory. What do you see here? Does the
output from pmap match your expectations?

* A certain [anon] region grows proportionally as I increase/decrease the size.
  * malloc -> mmap -> shows as [anon]

---