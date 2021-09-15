import os
import sys

# Add src as sys.path for now, so we can run pytest from project root
# The alternative would be to pip install --editable, which will be done later.
# or run python -m pytest from src directory which is inconvenient
pyroot = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(pyroot + "/src")
