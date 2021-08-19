# B-tree

The project was implemented for the needs of *Algorithms and Data Structures* course.

The task was to implement a b-tree data structure with a simple caching policy to measure an enhanced structure where an amount of cache can be used.
Project was implemented according to *Thomas H. Cormen’s book “Introduction to Algorithms”*. When inserting a new key, the algorithm splits every full node on the way to the final node. Also, when deleting a key, it fills every minimal node on the way to the node with the key we search for. 

* **I x** - Construct a tree of order x.
* **A x** - Add the value x to the tree. (No duplicates)
* **? x** - Checks if the value x is in the tree. If it is present prints "x +", otherwise prints "x -".
* **P**   - Prints all values that are in the tree in increasing order.
* **L t** - Loads a tree of order t in the given format. A pair of brackets ( ) signify a node. A value signifies a record (key in the tree).     
          ( ( ( 1 2 ) 3 ( 4 ) 5 ( 6 7 ) ) 8 ( ( 9 ) 10 ( 11 ) ) ) can be interpreted as the following tree:

                                        (         .           8       .    )
                                        (  . 3    .  5   .  )   ( . 10  .  )
                                        ( 1 2 ) ( 4 ) ( 6 7 )   ( 9 ) ( 11 )
* **S**   - Save the tree in the format described above.
* **R x** - Remove the element x from the tree. The element may be present or not.
* **X**   - End the program.
* **C x q1, q2, …** - Calculate an impact of a hypothetical cache. **x** - Size of cache, **q** - keys in query.
