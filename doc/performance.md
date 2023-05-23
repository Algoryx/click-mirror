
# Performance

```bash
python3.9 -m pClick.demo.server --trace-sizes
# client.cpp
```bash
time bin/democlient --range 10000 --timings
Sending 10000 messages
Receive took 0.336687 secs in total 3.36687e-05 per roundtrip
Idled betweeen send-recv for 1.51219 secs in total 0.000151219 per roundtrip
Total time for 10000 messages: 1.94437 secs
bin/democlient --range 10000 --timings
0.37s user 0.67s system 53% cpu 1.953 total
```

## client.py

```bash
time python3.9 -m pClick.demo.client --controlmessage "panda_tool:1,1,1,1,1,1,1;panda_2:1,1,1,1,1,1,1" --controltype Angle --range 9999
0.58s user 0.41s system 42% cpu 2.325 total
```

### profile

```bash
/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --trace-sizes  --timeStep 0.004032 --framerate 30 --stopAfter 4  --realTime 0 --disableClickSync --profile --profileFile nosync.profile

time bin/democlient --timings --range 993
/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --trace-sizes  --timeStep 0.004032 --framerate 30 --stopAfter 4  --realTime 0 --profile --profileFile sync.profile
```

Payloads:

- client.cpp sends ControlMessages payload of 210 bytes, recv 890 bytes
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
