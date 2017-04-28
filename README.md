# CRawlerPlusPlus

<img align="center" src="doc/logo.png">

CrawlerPlusPlus is the Modern C++ solution for web crawling. 

## Introduction

CrawlerPlusPlus uses a multi-threaded, async I/O model powered by Boost ASIO and libcurl network backend to achieve blazingly fast, non-blocking HTTP connection. Together with Python-like user interface, CrawlerPlusPlus makes writing spider in C++ both easy and efficient.

## Environment & Dependency

Environment

- Ubuntu 16.04 with g++6.2 and cmake3.8

Library dependency: 

- boost
- pthread
- crp (https://github.com/whoshuu/cpr.git)
- 

## Installation
```bash
$ git clone https://github.com/huanyan-hny/CRawlerPlusPlus.git

# on Ubuntu 16.04, assuming Environment & Dependency above satisfied

$ cd CRawlerPlusPlus
$ mkdir build && cd build 
$ cmake .. & make
```

## Tutorial

See CRawlerPlusPlus tutorial [here](doc/Tutorial.md)

## Documentation

To build the documention through doxygen, do:

```bash
$ doxygen Doxyfile
$ firefox html/index.html
```
## Structure

/demo/: Demostration of the Crawler on IMDB and Grubhub

/doc/: Documentations

/include/CRawlerPlusPlus/: Header files of the Library

/src/: Implementation files of the Library

/test/: Unit tests