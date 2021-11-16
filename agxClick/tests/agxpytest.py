import pytest
import sys

args = sys.argv[1:]
print(args)
exit(pytest.main(args))
