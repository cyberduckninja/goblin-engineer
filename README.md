# goblin-engineer

[![Build Status](https://travis-ci.org/jinntechio/goblin-engineer.svg?branch=master)](https://travis-ci.org/jinntechio/goblin-engineer)

# How To Use

## Installing Via Conan

1. Add the corresponding remote to your conan:

```bash
conan remote add jinntechio: https://api.bintray.com/conan/jinntechio/public-conan
```

2. Add goblin-engineer to `conanfile.txt` of your project:
```
[requires]
goblin-engineer/1.0.0a1@jinntechio/testing
```

# Getting started
The library has a "service" abstraction which allows you to bind handlers which would call asynchronously on completing HTTP request