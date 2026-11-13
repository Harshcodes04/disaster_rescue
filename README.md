# Disaster Rescue Coordination System

Data Structures PBL in C. Standard library only, no external libraries.
Terminal pe chalta hai, menu-driven, saath mein live ASCII city map.

Ek earthquake ke baad ka control room — rescue requests aati hain, sabse critical
victim ko pehle ambulance bheji jaati hai, aur Dijkstra sabse fast route nikalta hai.

## Quick start

```
run
```

Bas. `run.bat` compile karke program chala deta hai.

Manually karna ho to:

```
gcc main.c dispatch.c heap.c queue.c hash.c graph.c render.c -o rescue
rescue.exe
```

Linux / Mac pe `rescue.exe` ki jagah `./rescue`.

## Test drivers

Har module ka apna standalone test hai:

```
gcc -Wall test_queue.c queue.c -o test_queue
gcc -Wall test_heap.c heap.c -o test_heap
gcc -Wall test_hash.c hash.c -o test_hash
gcc -Wall test_graph.c graph.c heap.c -o test_graph
gcc -Wall test_render.c render.c graph.c heap.c -o test_render
```

Phir `test_queue.exe`, `test_heap.exe` wagairah chala lo.

Agar `make` installed hai to `make` aur `make tests` bhi kaam karta hai.
Windows pe aksar nahi hota — upar wali gcc lines wahi kaam karti hain.

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

## Demo ka order

1. `9` - 6 demo victims load karo
2. `5` - pending heap dekho, index 0 pe critical hai (chahe wo pehle type na hua ho)
3. `2` paanch baar - har dispatch pe route `#` se jal uthta hai, critical pehle jaate hain
4. `6` - 0 free, saari 5 bahar, front/rear index hil chuke hain
5. `2` - mana kar dega, aur bolega request heap se hatayi NAHI gayi
6. `5` - proof ki request abhi bhi padi hai
7. `3` phir `2` - ambulance wapas, rear index wrap hoke 0 pe
8. `2` - ab aakhri victim ko wahi ambulance milti hai
9. `4` -> `4002` - ek victim ka pura record
10. `8` - poora table bucket ke saath (4001 aur 4012 dono bucket 8 mein)

## City map

15 junctions, 22 two-way roads. Rescue centre 0, 9 aur 11 pe.
Road pe likha number travel minutes hai, screen pe lambai nahi.

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

`[R]` rescue centre, `[V]` victim, `#` highlighted route.

## Files

| File | Kya hai |
|---|---|
| `common.h` | shared constants, Request aur Victim structs, status enum |
| `heap.h/.c` | max-heap (requests) + min-heap (Dijkstra ke liye) |
| `queue.h/.c` | circular queue of free ambulances |
| `hash.h/.c` | victim table, chaining se collisions |
| `graph.h/.c` | adjacency list, Dijkstra, nearest rescue centre |
| `render.h/.c` | ASCII city map canvas |
| `dispatch.c` | menu actions aur pura dispatch cycle |
| `main.c` | menu loop |

## Data structures

| Kaam | Structure | Time |
|---|---|---|
| register request | max-heap push | O(log n) |
| sabse critical nikalna | max-heap pop | O(log n) |
| ambulance lena / wapas karna | circular queue | O(1) |
| victim lookup | hash table, chaining | O(1) average |
| fastest route | Dijkstra + min-heap | O((V+E) log V) |
| map redraw | 2D char canvas | O(ROWS*COLS + E) |

Ek pura dispatch cycle O((V+E) log V) ka hai, kyunki routing sabse bhaari step hai.

## Design decisions

**Circular queue** full vs empty ke liye `count` variable use karti hai, ek slot
sacrifice karne wala tareeka nahi. Isliye saare MAX_AMB slots usable hain.

**Heap** ka order `better()` function decide karta hai - zyada severity pehle,
same severity pe purana timestamp pehle. Isliye koi request starve nahi hoti.

**TABLE_SIZE 11** prime hai, taaki IDs buckets mein achhe se phail jaayein.

**Adjacency list** use ki hai matrix nahi, kyunki city sparse hai - har junction
sirf 3-4 road se judta hai.

**Lazy deletion** Dijkstra mein - decrease-key implement nahi kiya. Behtar distance
dobara push kar dete hain aur purani copy pop hone pe `done[]` check se skip ho jaati hai.

**Screen coordinates weights nahi hain.** `node_pos[]` sirf `render.c` mein hai,
`graph.c` use kabhi nahi padhta. Isliye choti dikhne wali road ka weight 9 ho sakta hai.

**Ambulance call pe rehti hai** jab tak option 3 se wapas na laayi jaaye. Agar wo
turant free ho jaati to "no free ambulance" wala case kabhi chalta hi nahi.

**Koi free ambulance na ho to peek, pop nahi.** `hp_top()` use hota hai taaki sabse
critical request heap pe bani rahe aur kho na jaaye.

## Abhi nahi kiya

Blocked roads aur re-routing, rescue centre ki capacity limit, file mein logging
aur reload, aur route pe ambulance ka animation.
