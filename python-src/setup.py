import setuptools

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setuptools.setup(
    name="pClick",
    version="0.1",
    author="Algoryx",
    author_email="contact@algoryx.se",
    description="Controller to AGX messaging",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://git.algoryx.se:algoryx/external/click",
    project_urls={
        "Bug Tracker": "https://git.algoryx.se/algoryx/external/click/-/issues"
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: Apache License Version 2.0",
        "Operating System :: OS Independent",
    ],
    package_dir={"": "src"},
    packages=setuptools.find_packages(where="src"),
    install_requires=[
        'protobuf==3.17.1',
        'pyzmq==22.3.0'
    ],
    python_requires=">=3.8",
)
