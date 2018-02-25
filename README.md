# ds
Collection of data structures for library use.

>Linked list: concurrent implementation of a simple linked list. Single lock for the whole list is chosen, since hand-over-hand locking does not provide a great speed-up.

>Doubly linked list: concurrent implementation of a doubly linked list. Similar API to linked list.

>Queue: concurrent implementation of a queue (Michael and Scott). Uses dummy node.

>Stack: concurrent implementation of a stack. Use of pthread condition variables and signals.

#### Notes
* Remember to include "/usr/local/lib" to the LD_LIBRARY_PATH environment variable
'export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH'
