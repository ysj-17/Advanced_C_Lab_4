**Decode 3 of 9 with multithreading to move through different checkout lanes**

This lab will use multithreading to move through different lanes(queues).
The queues contain a cart with items which are processed.
Each lane has a lock on its process but when it is finished will move to another threaded lane.