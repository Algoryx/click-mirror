'''
Run a brick model with the click protocol and web viewer

   python3 click_application.py --model models/Example5.yml:Example5 --rcs --portRange 5656 5658

On Linux agxViewer must be used:

   agxViewer --python-interpreter-mode click_application.py --model models/Example5.yml:Example5
'''

import sys
from scene import buildScene
try:
    from agxclick import ClickApplication
except Exception as ex:
    print(ex)
    print("This can happen if you are using agxViewer on Mac because of platfrom module not being loadable bug.")
    raise ex
import logging

logging.basicConfig(level="INFO", format='%(asctime)s %(levelname)s %(name)s - %(message)s')
# NOTE: cmdline parsing is done in buildScene, which depends on agxViewer being the first argument
args = sys.argv
args.insert(0, "agxViewer")
clickapp = ClickApplication(args)
clickapp.run(buildScene)
