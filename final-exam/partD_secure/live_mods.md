## Part D Curveball
**Instruction:** Add per-buyer purchase cap and reject orders above it.
**Live Value:** cap = 8
**Commands Ran:** 
`nano ~/bin/buy_beacon` (Added check: `if [ "$2" -gt "$CAP" ]`)
`~/bin/buy_beacon "Greedy_Bot" 10` (Proved rejection)
`swarm` (Proved consistent locker state)
