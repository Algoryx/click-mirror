
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

/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --trace-sizes  --timeStep 0.004032 --framerate 30 --stopAfter 4  --realTime 0 --disableClickSync --profile --profileFile waspwara.profile
```

This

- client.cpp sends ControlMessages payload of 160 bytes
- client.py sends ControlMessages payload of 155 bytes
- recv SensorMessage payload of 215 bytes

10000 times.

Results
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
Sending 10000 messages
Receive took 0.318723 secs in total 3.18723e-05 per roundtrip
Idled betweeen send-recv for 1.49206 secs in total 0.000149206 per roundtrip
bin/democlient --range 10000  0.36s user 0.64s system 52% cpu 1.907 total

/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --framerate 30 --trace-sizes -- -p
time bin/democlient --range 1000
Sending 1000 messages
Receive took 0.059227 secs in total 5.9227e-05 per roundtrip
Idled betweeen send-recv for 16.5428 secs in total 0.0165428 per roundtrip
bin/democlient --range 1000  1.36s user 3.08s system 26% cpu 16.648 total
```

--> Roundrip utan simulering: 0.15 millisec --> 6702 Hz
--> Roundtrip med waspvarasimulering: 16.54 millisec --> 60Hz
--> En factor 110 alltså.
--> Själva receiven tog dubbelt så lång tid mot waspwara. Det är samma datamängd men något mer komplex struktur från waspvara, så det kan vara så att serialiseringen tar längre tid.

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

Test                            | Wall clock OSX | Client recv idle
--------------------------------|----------------|-----------------
248 Hz DisableClickSync         | 1.9 sek        | -
248 Hz democlient without sleep | 2.8 sek        | 2.7
500 Hz DisableClickSync         | 2.6 sek        | -
500 Hz democlient without sleep | 4.7 sek        | 4.6

### OSX

On OSX click seem to have a 47% increase in time, but client is idling.

### Windows

Windows is waaay slower than OSX on this. Does not understand why yet.

Sandra:
Om jag kör samma som Markus körde, 250 Hz, 1000 steg, (4 sekunder simuleringstid) så får jag:
Med --disableClickSync: 8.2 sek med getDataoch setData, 4 sekunder utan.
Utan --disableClickSync: 12.2 sek med getDataoch setData, 8,4 sekunder utan.
