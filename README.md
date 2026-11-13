# Disaster Rescue Coordination System

A Data Structures PBL written in C. Standard library only, no external libraries.
It runs in a terminal as a menu-driven program with a live ASCII city map.

The program is a control room after an earthquake. Rescue requests come in, the most
critical victim is always attended first, a free ambulance is assigned from a fixed
fleet, and Dijkstra finds the fastest route from the nearest rescue centre.

## Build and run

```
gcc main.c dispatch.c heap.c queue.c hash.c graph.c render.c -o rescue
rescue.exe
```

On Linux or Mac use `./rescue` instead of `rescue.exe`.

If you have `make` installed, `make` does the same thing. It is usually not
installed on Windows, so the gcc line above is the reliable way.

## Test drivers

Every module has its own standalone test, so each one can be checked without
the rest of the system.

```
gcc -Wall test_queue.c queue.c -o test_queue
gcc -Wall test_heap.c heap.c -o test_heap
gcc -Wall test_hash.c hash.c -o test_hash
gcc -Wall test_graph.c graph.c heap.c -o test_graph
gcc -Wall test_render.c render.c graph.c heap.c -o test_render
```

Then run `test_queue.exe`, `test_heap.exe` and so on. `make tests` builds all five.

What each one proves:

| Test | What it checks |
|---|---|
| `test_queue` | fills and empties the queue, then pushes past the wrap point to show the indices cycle |
| `test_heap` | pushes 10 mixed-severity requests and pops them in severity order, FIFO within equal severity |
| `test_hash` | inserts 22 victims including deliberate collisions and finds every one |
| `test_graph` | shortest paths from junction 0, and nearest-centre lookups |
| `test_render` | draws the city with a hard-coded path, so it works without Dijkstra |

## Menu

```
 1. register a new rescue request
 2. dispatch next ambulance
 3. an ambulance came back (rescue finished)
 4. victim status query
 5. show pending requests
 6. show free ambulances
 7. show city map
 8. show all victims
 9. load demo victims
 0. exit
```

## Demo walkthrough

1. `9` loads six demo victims
2. `5` shows the pending heap. Index 0 is the critical one even though it was not entered first
3. `2` five times. Each dispatch lights the route up with `#` and the critical ones leave first
4. `6` shows 0 free, all five out, and the front/rear indices have moved
5. `2` again. It refuses and reports that the request was **not** taken off the heap
6. `5` again proves the request is still waiting
7. `3` then `2` brings an ambulance back. The rear index wraps around to 0
8. `2` dispatches the last victim with the returned ambulance
9. `4` then `4002` shows one victim's full record
10. `8` shows the whole table with bucket numbers. 4001 and 4012 share bucket 8

## The city

15 junctions and 22 two-way roads. Rescue centres are at junctions 0, 9 and 11.
The number printed on a road is travel minutes, not its length on screen.

```
   [R]------4------(1)------6------(2)------5------(3)-----7-----(4)
    |               |               |               |             |
    9               2               5               6             8
    |               |               |               |             |
   (5)------3------(6)------8------(7)------2------(8)-----6-----[R]
    |               |               |               |             |
    4               7               3               4             5
    |               |               |               |             |
  (10)------5------[R]------4-----(12)------9-----(13)-----3----(14)
```

`[R]` is a rescue centre, `[V]` is the victim, `#` marks the highlighted route.

## Files

| File | Contents |
|---|---|
| `common.h` | shared constants, the Request and Victim structs, status enum |
| `heap.h` / `heap.c` | max-heap for requests, plus the min-heap Dijkstra uses |
| `queue.h` / `queue.c` | circular queue of free ambulances |
| `hash.h` / `hash.c` | victim table with chaining |
| `graph.h` / `graph.c` | adjacency list, Dijkstra, nearest rescue centre |
| `render.h` / `render.c` | the ASCII city map canvas |
| `dispatch.c` | menu actions and the full dispatch cycle |
| `main.c` | menu loop |

## One dispatch cycle

| Step | Operation | Structure | Time |
|---|---|---|---|
| 1 | `hp_pop` picks the most critical request | max-heap | O(log n) |
| 2 | `q_dequeue` takes a free ambulance | circular queue | O(1) |
| 3 | `nearest_center` finds the closest centre | graph + Dijkstra | O((V+E) log V) |
| 4 | `get_path` rebuilds the route | parent array | O(V) |
| 5 | `map_draw` highlights it on screen | 2D canvas | O(ROWS·COLS + E) |
| 6 | `ht_update_status` updates the victim record | hash table | O(1) average |
| 7 | `q_enqueue` returns the ambulance | circular queue | O(1) |

The whole cycle is O((V+E) log V) because the routing step dominates.

## Design decisions

**Circular queue, full versus empty.** A `count` variable is used rather than
sacrificing a slot, so all MAX_AMB slots are usable.

**Heap ordering.** A single `better()` function decides everything: higher severity
first, and on a tie the earlier timestamp wins. That is why no request starves.

**TABLE_SIZE is 11, a prime**, so IDs sharing common factors still spread across
buckets instead of clustering into a few chains.

**Adjacency list, not a matrix.** Each junction touches only three or four roads,
so a 15×15 matrix would be mostly zeros. The list is O(V+E).

**Lazy deletion in Dijkstra.** `decrease-key` is never implemented. A better distance
is simply pushed again, and when the stale copy pops, `done[]` is already set so it
is skipped.

**Screen coordinates are not weights.** `node_pos[]` lives only in `render.c` and
`graph.c` never reads it. A road that looks short on screen can still cost 9 minutes.

**An ambulance stays out until it reports back.** Dispatch marks the victim
DISPATCHED and removes the ambulance from the ring; option 3 brings it back and marks
the victim RESCUED. Re-enqueuing immediately would make the no-free-ambulance branch
unreachable.

**No free ambulance means peek, not pop.** `hp_top()` is used so the most critical
request stays on the heap instead of being thrown away.

## Not implemented

Blocked roads with re-routing, rescue centre capacity limits, logging rescues to a
file and reloading on startup, and animating the ambulance along the route.
