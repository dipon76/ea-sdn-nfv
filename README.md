EA-SDN/NFV - An energy-optimized interference-aware SDN/NFV architecture for IoT architecture.
===

Intro
---
This repo hosts the source code of EA-SDN/NFV. This is a work of extention of the μSDN architecture. The μSDN repository can be found at 
https://github.com/mbaddeley/usdn. 

About
---
EA-SDN/NFV has been developed as a part of the academic research where we implemented a heuristic energy-optimized interference-aware and its corresponding node architecture. The architecture is based upon the μSDN. so, we provided the important parts of our codes here which can be easily plugged or exchanged into the μSDN code base. 

Getting Started
---

**IMPORTANT** 
First, you have to go [here] https://github.com/mbaddeley/usdn and follow the instruction to the point to make the μSDN code base work properly. 

Then download the three folders from this repository. 

Then you need to replace the the corresponding folders of μSDN code base with the newly downloaded folder. 

The folder 'usdn/app/atom' of μSDN code base is to be replaced by the 'atom' folder of this repository.

The folder 'usdn/core/net' of μSDN code base is to be replaced by the 'net' folder of this repository.

The folder 'usdn/example/sdn' of μSDN code base is to be replaced by the 'sdn' folder of this repository. 

Then you have to move into the 'usdn/example/sdn' and compile using 


To get you going you can find some Cooja examples in:

 **usdn/examples/sdn/..**

There is a handy compile script in there that can be used to compile both the controller and node:

```
./compile.sh MULTIFLOW=1 NUM_APPS=1 FLOWIDS=1 TXNODES=8 RXNODES=10 DELAY=0 BRMIN=5 BRMAX=5 NSUFREQ=600 FTLIFETIME=300 FTREFRESH=1 FORCENSU=1 LOG_LEVEL_SDN=LOG_LEVEL_DBG LOG_LEVEL_ATOM=LOG_LEVEL_DBG CAPACITY 2 HIGHE 20
```

uSDN Make Args:
- NSUFREQ - Frequency of node state updates to the controller (seconds)
- FTLIFETIME - Lifetime of flowtable entries (seconds)
- FTREFRESH - Refresh flowtable entries on a match (0/1)
- FORCENSU - Immediately send a NSU to the controller on join (0/1)
- LOG_LEVEL_SDN - Set the uSDN log level (0 - 5)
- LOG_LEVEL_ATOM - Set the Atom controller log level (0 - 5)

Multiflow Make Args:
- MULTIFLOW - Turn on multiflow (0/1)
- NUM_APPS - Number of flows (N)
- FLOWIDS - Id for each flow ([0,1,2...])
- TXNODES - Transmission nodes ([18,12...] 0 is ALL)
- RXNODES - Receiving nodes ([1,2...])
- DELAY   - Delay each transmission (seconds)
- BRMIN   - Minimum bitrate (seconds)
- BRMAX   - Maximum bitrate (seconds)


Where is everything?
---
- Core: */core/net/sdn/*
- Stack: */core/net/sdn/usdn/*
- Atom: */apps/atom/*
- Multiflow: */apps/multiflow/*

EA-SDN/NFV implementation details
--- 
- initialization for controller */examples/sdn/controller/sdn-controller.c* 
- initialization for nodes */examples/sdn/node/sdn-udp-client.c*
- configuration files */core/net/sdn* 
- heuristic files */apps/atom* 

Simulation
---
- The Cooja simulator is integrated in the Code base. go to '/usdn/tools/cooja' and run the command 'ant run'
- the topology file provided in the */examples/sdn* can be used or any new file can be created. 
