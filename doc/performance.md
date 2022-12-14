
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

/usr/local/bin/python3.9 scripts/click_application.py --model models/RobotLabScenes.yml:MYuMiInLab --decorate --framerate 30 --trace-sizes --profile --profileFile waspwara.profile -- -p
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