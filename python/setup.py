from setuptools import setup

setup(
    name='smap',
    version='0.0.1',
    packages=['sysmap'],
    py_modules=['smap'],
    install_requires=[
        'python-arango',
        'click',
        'graphviz',
    ],
    entry_points='''
        [console_scripts]
        smap=smap:cli
    ''',
)
