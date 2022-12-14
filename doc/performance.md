
# Performance

```bash
python3 -m pClick.demo.server
# client.cpp
time bin/democlient > apa
# Get block average:
grep "Would have blocked" apa |sort -n|awk '{print $4}'|awk '{s+=$1} END {print s/10000.0}'
# client.py
python3 -m pClick.demo.client --controlmessage  --controltype Angle --range   0.56s user 0.40s system 42% cpu 2.237 total

## profile

/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --trace-sizes  --timeStep 0.004032 --framerate 30 --stopAfter 4  --realTime 0 --disableClickSync --profile --profileFile waspwara-nosync.profile

time bin/democlient --timings --range 993
/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --trace-sizes  --timeStep 0.004032 --framerate 30 --stopAfter 4  --realTime 0 --profile --profileFile waspwara-sync.profile
```

This

- client.cpp sends ControlMessages payload of 160 bytes
- client.py sends ControlMessages payload of 155 bytes
- recv SensorMessage payload of 215 bytes

10000 times.

Results OSX
what                                | client would block average ms | wall clock time [s] | ms per roundtrip | Improvement
------------------------------------|-------------------------------|---------------------|------------------|------------
client.cpp -> server.py TCP 100ms   | 0.178                         | 2.5                 | 0.25             |
client.cpp -> server.py TCP 10ms    | 0.089                         | 2.2                 | 0.22             |
client.cpp -> server.py TCP nosleep | 0.263                         | 2.0                 | 0.20             |
client.cpp -> server.py TCP block   |                               | 2.0                 | 0.20             |
client.cpp -> server.py IPC 100ms   | 0.152                         | 2.1                 | 0.21             |
client.cpp -> server.py IPC nosleep | 0.243                         | 1.7                 | 0.17             |
client.cpp -> server.py IPC block   |                               | 1.6                 | 0.16             |
client.py -> server.py  TCP block   |                               | 2.3                 | 0.23             |
client.py -> server.py  IPC block   |                               | 1.8                 | 0.23             |

Results Win 210/890
what                                | client would block average ms | wall clock time [s] | ms per roundtrip | Improvement
------------------------------------|-------------------------------|---------------------|------------------|------------
client.cpp -> server.py TCP nosleep | -                             | 3.4                 | 0.34             |


## Conclusions

- The sleep takes a significant amount of time - don't use it.
- Blocking comm vs nosleep non-blocking has no significant performance difference.
- IPC is significantly faster
- Removing prints makes a significant performance difference ~7%

# Bigger scenes

When emulating WASPVARA scene, sending 210 bytes receiving 890 is around 2% slower than 160/215

# Performance measure log

```bash
python3 -m pClick.demo.server
time bin/democlient --range 10000
Sending 1000 messages
Receive took 0.033541 secs in total 3.3541e-05 per roundtrip
Idled betweeen send-recv for 0.153801 secs in total 0.000153801 per roundtrip
Total time for 1000 messages: 0.196399 secs
bin/democlient --timings --range 1000  0.04s user 0.07s system 52% cpu 0.205 total

/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --framerate 30 --trace-sizes --realTime 0
time bin/democlient --range 1000
Sending 1000 messages
Receive took 0.043621 secs in total 4.3621e-05 per roundtrip
Idled betweeen send-recv for 2.69866 secs in total 0.00269866 per roundtrip
Total time for 1000 messages: 2.77226 secs
```

--> Roundrip utan simulering: 0.19 millisec --> 5091 Hz
--> Roundtrip med waspvarasimulering: 2.77 millisec --> 361
--> En factor 14 alltså.

## Wasp wara

### 248 Hz realtime

```bash
/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --trace-sizes  --timeStep 0.004032 --framerate 30 --stopAfter 4  --realTime 0 --disableClickSync
Rendered 120 frames and 993 simulation steps, received 0 control messages
simulated time: 4.00377596821636 Wall clock time: 4.2987220287323
Wallclock sim freq: 231.0 Hz Wallclock framerate: 27.9

bin/democlient --timings --range 993
/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --trace-sizes  --timeStep 0.004032 --framerate 30 --stopAfter 4 --realTime 0
Rendered 118 frames and 993 simulation steps, received 993 control messages
simulated time: 4.00377596821636 Wall clock time: 4.089172124862671
Wallclock sim freq: 242.8 Hz Wallclock framerate: 28.9


```

### 500 Hz

```bash
/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --trace-sizes  --timeStep 0.002 --framerate 30 --stopAfter 4 --realTime 0 --disableClickSync
Rendered 98 frames and 2000 simulation steps, received 0 control messages
Controller could not keep up with simulation! 2000 simulation steps taken without control message
simulated time: 4.000000189989805 Wall clock time: 4.081348180770874
Wallclock sim freq: 490.0 Hz Wallclock framerate: 24.0

# Synced
/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --trace-sizes  --timeStep 0.002 --framerate 30 --stopAfter 4 --realTime 0

Rendered 136 frames and 2000 simulation steps, received 2000 control messages
simulated time: 4.000000189989805 Wall clock time: 4.672284841537476
Wallclock sim freq: 428.1 Hz Wallclock framerate: 29.1
```

## Conclusion

### OSX

No short term gains, but moving to C++ should increase the Brick-AGX data transfer performance, which probably come from poor implementation of pythondotnet on OSX.

2020 MacBook Pro

Test                                        | Wall clock OSX | Client recv idle
--------------------------------------------|----------------|-----------------
248 Hz DisableClickSync                     | 1.9 sek        | -
248 Hz democlient without sleep             | 2.8 sek        | 2.7
248 Hz DisableClickSync no graphics         | 1.5 sek        | -
248 Hz democlient without sleep no graphics | 2.7 sek        | 2.6
500 Hz DisableClickSync                     | 2.6 sek        | -
500 Hz democlient without sleep             | 4.7 sek        | 4.6

On OSX enabling sync means a 47% increase in time, but client is idling, i.e. waiting for simulation.
Profiling shows 55% of increase is internal data transfer (Brick-AGX, not click). 20% is Click (protobuf, zmq).

### Windows

Comparing ABB Windows performance with ours, there is a big difference (328%) between 8.22 vs 2.5 sek. 224% for no sync.
It is also worse than both OSX and Linux laptops we tested with.
We should look into why their performance is so much worse than ours.

1. Test without graphics to see if graphic settings might be the cause
2. If not, what is the big hardware difference?

In the long run, we could look into using C++ protobuf from Python, or implement Click serverside completely in C++ when rebrick is mature.

#### ABB Desktop(?)

Test                            | Wall clock OSX
--------------------------------|---------------
248 Hz DisableClickSync         | 5.6 sek
248 Hz democlient without sleep | 8.22 sek

Enabling sync means 47% increase in wallclock time.

#### 2022 Win Desktop

Test                                        | Wall clock Windows | Client recv idle
--------------------------------------------|--------------------|-----------------
248 Hz DisableClickSync                     | 2.5 sek            | -
248 Hz democlient without sleep             | 2.5 sek            | 2.?
248 Hz DisableClickSync no graphics         | 1.1 sek            | -
248 Hz democlient without sleep no graphics | 1.5 sek            | 1.?

The difference is only visible without graphics:
Enabling sync means 64% increase in wallclock time.
Profiling shows ~70% of increase is from click, mainly protobuf in python(~52%). 9% of increase is Brick-AGX.
8% is from within AGX and the rest from the python application.

#### 2020 Linux Laptop

Test                                        | Wall clock Windows | Client recv idle
--------------------------------------------|--------------------|-----------------
248 Hz DisableClickSync                     | 5.2 sek            | -
248 Hz democlient without sleep             | 7.59 sek           | 7.56
248 Hz DisableClickSync no graphics         | 2.0 sek            | -
248 Hz democlient without sleep no graphics | 3.50 sek           | 3.48

On Linux enabling sync means a 63% increase in time, but client is idling, i.e. waiting for simulation.
Profiling shows 52% of increase is internal data transfer (Brick-AGX, not click). 11% is Click (protobuf, zmq).


#### 2017 Win Desktop

Test                                        | Wall clock Windows
--------------------------------------------|-------------------
248 Hz DisableClickSync no graphics         | 1.9 sek
248 Hz democlient without sleep no graphics | sek

#### 2021 Win Desktop

Test                                        | Wall clock Windows
--------------------------------------------|-------------------
248 Hz DisableClickSync                     | 4.1 sek
248 Hz DisableClickSync no graphics         | 1.46 sek
248 Hz democlient without sleep no graphics | 3.2
