## Part A Curveball
**Instruction:** Spawn 3 extra workers after originals join.
**Live Value:** 3 extra workers.
**Commands Ran:** 
`nano thread_demo.c`
`gcc -pthread thread_demo.c -o thread_demo`
`./thread_demo &`
`ps -eLf | grep thread_demo` (while extra workers were alive)
`ps -eLf | grep thread_demo` (after completion to show they disappeared)
