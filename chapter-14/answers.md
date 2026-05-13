# Questions
1. First, write a simple program called null.c that creates a pointer to an integer, sets it to NULL, and then tries to dereference it. Compile this into an executable called null. What happens when you run this program?

* When I run the program, it logs "Segmentation Fault".

---

2. Next, compile this program with symbol information included (with the -g flag). Doing so let’s put more information into the executable, enabling the debugger to access more useful information about variable names and the like. Run the program under the debugger by typing gdb ./null and then, once gdb is running, typing run. What does gdb show you?

* It shows me that I had a `bad access` on the line where I dereferenced the pointer.

---

3. Finally, use the valgrind tool on this program. We’ll use memcheck that is a part of valgrind to analyze what happens. Run this by typing in the following: valgrind --leak-check=yes ./null. What happens when you run this? Can you interpret the output from the tool?

* Below is highlight of the logs that supports the idea that we are accessing memory that is not allocated/mapped to our process.

```
Invalid read of size 4 -----> Size of an integer.
 at 0x4001196: main (null.c:8)
Address 0x0 is not stack'd, malloc'd or (recently) free'd -----> Signifying we are accessing a memory not allocated to us.
Process terminating with default action of signal 11 (SIGSEGV)
 Access not within mapped region at address 0x0
   at 0x4001196: main (null.c:8)
```

---

4. Write a simple program that allocates memory using malloc() but forgets to free it before exiting. What happen swhen this program runs? Can you use gdb to find any problems with it? How about valgrind (again with the --leak-check=yes flag)?

* Using `gdb` (in my case used `lldb`), there is no logs points that we have a memory leak. This might not be the tool for catching leaks.

```
opher@Marks-MacBook-Air chapter-14 % lldb ./null
(lldb) target create "./null"
Current executable set to '/Users/popher/OrbStack/ubuntu/home/popher/ostep-code-homework/chapter-14/null' (arm64).
(lldb) run
Process 42380 launched: '/Users/popher/OrbStack/ubuntu/home/popher/ostep-code-homework/chapter-14/null' (arm64)
Process 42380 exited with status = 0 (0x00000000) 
```

* With `valgrind` there is information that at exit we still have a heap usage of 1,000 bytes which signifies that we have a leak i.e. a malloc'd memory that was not free'd.

* `definitley lost` means that there is no reference to the pointer anymore thus we can't free it. That is why in the first code block it says that. We have exited the `main` function thus `main` stack is popped. Once it was popped the pointer was also removed which was our only reference to the malloc'd memory.

* `still reachable` means that at that point we can still free it i.e. there is still a reference to the leaked memory. That is the case in the second code block since we exited before the `main` was finished thus we still have the reference to the malloc'd memory in main.

```
// Program with just allocating and not freeing.
==5852== Memcheck, a memory error detector
==5852== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==5852== Using Valgrind-3.25.1 and LibVEX; rerun with -h for copyright info
==5852== Command: ./null
==5852== 
==5852== 
==5852== HEAP SUMMARY:
==5852==     in use at exit: 1,000 bytes in 1 blocks
==5852==   total heap usage: 1 allocs, 0 frees, 1,000 bytes allocated
==5852== 
==5852== LEAK SUMMARY:
==5852==    definitely lost: 1,000 bytes in 1 blocks
==5852==    indirectly lost: 0 bytes in 0 blocks
==5852==      possibly lost: 0 bytes in 0 blocks
==5852==    still reachable: 0 bytes in 0 blocks
==5852==         suppressed: 0 bytes in 0 blocks
==5852== Rerun with --leak-check=full to see details of leaked memory
==5852== 
==5852== For lists of detected and suppressed errors, rerun with: -s
==5852== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

```
// Program with allocating then dereferencing a null pointer. 
==5864== Memcheck, a memory error detector
==5864== Copyright (C) 2002-2024, and GNU GPL'd, by Julian Seward et al.
==5864== Using Valgrind-3.25.1 and LibVEX; rerun with -h for copyright info
==5864== Command: ./null
==5864== 
==5864== Invalid read of size 4
==5864==    at 0x4001196: main (null.c:8)
==5864==  Address 0x0 is not stack'd, malloc'd or (recently) free'd
==5864== 
==5864== 
==5864== Process terminating with default action of signal 11 (SIGSEGV)
==5864==  Access not within mapped region at address 0x0
==5864==    at 0x4001196: main (null.c:8)
==5864==  If you believe this happened as a result of a stack
==5864==  overflow in your program's main thread (unlikely but
==5864==  possible), you can try to increase the size of the
==5864==  main thread stack using the --main-stacksize= flag.
==5864==  The main thread stack size used in this run was 8388608.
==5864== 
==5864== HEAP SUMMARY:
==5864==     in use at exit: 1,000 bytes in 1 blocks
==5864==   total heap usage: 1 allocs, 0 frees, 1,000 bytes allocated
==5864== 
==5864== LEAK SUMMARY:
==5864==    definitely lost: 0 bytes in 0 blocks
==5864==    indirectly lost: 0 bytes in 0 blocks
==5864==      possibly lost: 0 bytes in 0 blocks
==5864==    still reachable: 1,000 bytes in 1 blocks
==5864==         suppressed: 0 bytes in 0 blocks
==5864== Reachable blocks (those to which a pointer was found) are not shown.
==5864== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==5864== 
==5864== For lists of detected and suppressed errors, rerun with: -s
==5864== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
Segmentation fault
```
---

5. Write a program that creates an array of integers called data of size 100 using malloc; then, set data[100] to zero. What happens when you run this program? What happens when you run this program using valgrind? Is the program correct?

* `valgrind` says that we leaked 100 bytes which is expected since we did not call free on malloc'd memory. 
* It reports that we have an invalid write since data[100] does not belong to us yet. Valid memory is data[0] to data[99].
  * This is usually called `off-by-one error`.
* Scary thing is this program did not crash. It does exits normally. Good thing we have tools like `valgrind` that can report such stuff.
  * I believe the MMU did not trigger a fault because we technically modified memory that is still ours. I read that sometimes malloc adds a padding, so it might be that data[100] is within the padding.
  * I did `data[100000] = 0` and it caused a segmentation fault.

> Aside
>
> The MMU works at page granularity, not byte granularity.
>
> The MMU doesn't track individual bytes or even individual allocations. It only knows about pages (typically 4KB chunks). It asks one question:  
> "Is this page mapped to your process?"  
> Not:  
> "Is this specific byte within your malloc'd region?"
>
> Valgrind essentially wraps every memory access with its own software-level tracking — doing what the MMU doesn't do: tracking individual byte-level allocations.
> 
> The hardware protects at page level. Valgrind protects at byte level. They operate at completely different granularities.

---

6. Create a program that allocates an array of integers (as above), frees them, and then tries to print the value of one of the elements of the array. Does the program run? What happens when you use valgrind on it?

* Running the program as is does not show any signs of error.
* Running it thru valgrind says otherwise:
  * It gives a warning that we were using the values in the malloc'd memory without first initializing it which I think is a good information to have since it is makes more sense to use it after some initialization, otherwise the values are undefined (though, in this case they all seem to be zero, but I am not sure if that is consistent across all platform/compilers).
  * After we free and then use the array, it shows that we are having invalid read. I believe using after free is not a good idea, in the memory manager libraries point of view you don't own that piece of memory anymore so it can be overwritten by something else or you might write on it whereas other entity owns it already.
    * I believe the reason we did not get a segmentation fault on this is because the OS itself still recognizes this as our processes' memory block. The memory manager library probably did some to not let the OS know yet. It kept it in its memory pool since it expects another malloc call. If a malloc call does come, it just uses the one in its memory pool instead of requesting from the OS as I have read that this operation takes time.

---

7. Now pass a funny value to free (e.g., a pointer in the middle of the array you allocated above). What happens? Do you need tools to find this type of problem?

```C
// code snippet
int main() {
    int *one_hundred_ints = (int *)malloc(sizeof(int) * 100);

    for (int i = 0; i < 100; i++) {
        printf("one_hundred_ints[%d] = %d\n", i, one_hundred_ints[i]);
    }

    int *free_in_middle = (one_hundred_ints + 49);

    free(free_in_middle);

    for (int i = 0; i < 49; i++) {
        printf("one_hundred_ints[%d] = %d\n", i, one_hundred_ints[i]);
    }


    return 0;
}
```

* During compilation I am already having a warning.

```
null.c: In function ‘main’:
null.c:39:5: warning: ‘free’ called on pointer ‘one_hundred_ints’ with nonzero offset 196 [-Wfree-nonheap-object]
   39 |     free(free_in_middle);
      |     ^~~~~~~~~~~~~~~~~~~~
null.c:31:36: note: returned from ‘malloc’
   31 |     int *one_hundred_ints = (int *)malloc(sizeof(int) * 100);
      |                                    ^~~~~~~~~~~~~~~~~~~~~~~~~
gcc null.o -o null
```

* Running the code throws an error already.

```
free(): invalid pointer
Aborted
```

* With the above, I guess it provides information that something is wrong. It lacks some details such as where it was, so I imagine in a bigger program it would not be that easy to find.

```
// lldb output
rocess 49564 stopped
* thread #1, queue = 'com.apple.main-thread', stop reason = EXC_BREAKPOINT (code=1, subcode=0x1947ef188)
    frame #0: 0x00000001947ef188 libsystem_malloc.dylib`mfm_free.cold.2 + 36
libsystem_malloc.dylib`mfm_free.cold.2:
->  0x1947ef188 <+36>: brk    #0x1

libsystem_malloc.dylib`mfm_free.cold.3:
    0x1947ef18c <+0>:  mov    x8, x0
    0x1947ef190 <+4>:  stp    x20, x21, [sp, #-0x10]!
    0x1947ef194 <+8>:  adrp   x20, 5
Target 0: (null) stopped.
```

```
// valgrind output
==6506== Invalid free() / delete / delete[] / realloc()
==6506==    at 0x48558BF: free (vg_replace_malloc.c:989)
==6506==    by 0x4001225: main (null.c:39)
==6506==  Address 0x4ab4104 is 196 bytes inside a block of size 400 alloc'd
==6506==    at 0x4852858: malloc (vg_replace_malloc.c:446)
==6506==    by 0x40011C5: main (null.c:31)
==6506== 
```

* Looking at the output of lldb and valgrind, it seems like valgrind tells the story better. It is saying that the cause of error is that we are trying to free a block of memory but using an address that is not the start of that block (From this, I assume that if you free a block it should be the an address at the start of the block? Follow up question, given that we give an address to free, how does it know how many blocks to free? Does the memeory allocation library have a map of how many blocks are assigned to an address, and it that way it knows the "range" of a particular address, which is also the reason why it figured out we are calling free on a memory block that was allocated with 400 bytes?)

```
Research:
malloc maintains a hidden metadata header just before the pointer it returns to you:
what malloc actually allocates:
┌──────────────┬─────────────────────────┐
│   metadata   │   your usable memory    │
│  (size, etc) │                         │
└──────────────┴─────────────────────────┘
                ↑
                pointer returned to you
When you call free(ptr), it steps backward from your pointer to read the metadata and finds out exactly how many bytes to free.

This also explains why freeing a middle pointer is dangerous:
┌──────────┬─────────────────────────────────┐
│ metadata │   your 400 bytes                │
└──────────┴─────────────────────────────────┘
            ↑                    ↑
         valid ptr          free_in_middle
                            steps back here → reads garbage as "metadata"
When you pass free_in_middle, it steps backward from that address expecting metadata — but finds garbage instead. That's why it aborts.
```

---

8. Try out some of the other interfaces to memory allocation. For example, create a simple vector-like data structure and related routines that use realloc() to manage the vector. Use an array to store the vectors elements; when a user adds an entry to the vector, use realloc() to allocate more space for it. How well does such a vector perform? How does it compare to a linked list? Use valgrind to help you find bugs.

* I had some bugs with using `realloc`, when I am passing the `size`, I am just passing the number of elements instead of number of elements times the size of the data type (in this case, `int`).

* I had a bug where when I am appending, I don't add the new value lol.

* Forgot to call free at end of program.
  * Created a `free_vector` function. Technically the user can just do `free(vector->memory-pointer)` but this struct can evolve in the future. I do not want the burden to be on the user on how to free this struct. He can just use `free_vector` instead.
    * I can see a future problem tho. Nothing stops a user from doing `free_vector` and then doing `free(vector->memory-pointer)` or the other way around.

* When popping, I realloc'd with size of zero. I will not have found that bug if not for valgrind.
  * Side note: I found something interesting. When we realloc'd to zero and tried to print the first element `printf("I can still access the last lol. %d\n", v.memory_pointer[0]);`, the behavior is different in mac and linux. In linux it threw a segmentation fault, while in mac it ran without signs of failure.
  * I have a decision that has quite some tradeoff. When we pop everything in the vector. It would seem like the vector does not own any memory/values anymore, but that is not the case. In the last pop, we return the last value in the vector, and set the size to zero, but we do not free the memory. This is done so that when "append" is called again, we have a valid memory to append to.

---

9. Spend more time and read about using gdb and valgrind. Knowing your tools is critical; spend the time and learn how to become an expert debugger in the UNIX and C environment.

yes. yes. yes.

---