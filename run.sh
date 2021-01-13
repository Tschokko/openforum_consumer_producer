#!/bin/sh
bazel run --cxxopt='-std=c++17' --test_output=all //consumer_producer:demo