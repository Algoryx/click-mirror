import setuptools

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setuptools.setup(
    name="agxClick",
    version="0.1.23",
    author="Algoryx Simulation",
    author_email="contact@algoryx.se",
    description="Controller to AGX using pClick messaging",
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
        'pClick>=0.1.8',
        'agxBrick>=0.4.68'
    ],
    python_requires=">=3.6.9",
)
