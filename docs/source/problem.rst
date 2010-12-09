Common Stream Process Problems
==============================

Parsing
-------

Parsing is probably one of the most difficult problems to solve with regards
to streams of bytes. Parsing is an ancient problem in the computing world
but still it seems many parsing libraries get it wrong. A good stream parser
keeps its own state, does not use malloc/free, and uses callbacks to inform
the user when and where tokens are found.

Buffering
---------

Parsing being the tricky task that it is usually requires buffering. If only
partial tokens are found buffering needs to happen in order to accumulate enough
bytes to represent the next useful thing a parser can handle. Buffering should
be fast and act very much like a randomly accessible file. It should
provide a continuous memory address space if possible and if not make it easy
enough to act like one.

Structure
---------

After a stream has been parsed it typically needs to be placed in to some
form of a useful structure. Whether that be a simple C struct, hash table, tree,
or list depends all on the nature of the stream. HTTP for example fits nicely
in all of the above structures for different reasons!

Parallelism
-----------

Streams are often coming in and out in droves and parallelism is really
important once your program is expected to handle more than few streams at a
time.

Reliable
--------

A reliable stream handler fails gracefully. Depending on the situation that
may be as simple as giving up altogether and starting with a clean slate or
attempting to recover from some errors.

Octonaut Solutions
==================

Octonaut does not even begin to attempt to solve all of the above problems.

It attempts to solve the problem of Buffering, Structure,
and give the ability to handle Reliability, Parallelism, and Parsing in any way
you like.

It may sound then like it octonaut does not do much. Thats a fair enough
statement and if you feel the same way then octonaut is hopefully doing
not much very very well.

Take for example a typical http server. Starting simple and socket is listened
upon for connections, each connection representing a http request.

A good http server can handle tens of thousands of requests a second.

Things that can help make a http server fast and reliable

* Sane buffering with limits.
* Incremental and parallel http parsing.
* Incremental and parallel response returns.
* Root free design after accept()

So for those criteria a http server may have a fixed size buffer per client
for requests, where the server listening is in one process and each request
is given to a child process pool for handling. Both the child workers and parent
server use event driven programs to handle requests to avoid context switching
allowing for low overhead per connection. Memory pools of data structures are
kept to avoid using malloc/free and therefore avoiding memory fragmentation
and overhead of malloc/free.

The ideal webserver looks very much like apache's event mpm or nginx.




