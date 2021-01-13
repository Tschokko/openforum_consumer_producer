load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cc_library(
    name = "consumer_producer",
    srcs = glob(
        [
            "src/**/*.cpp",
            "src/**/*.ipp",
        ],
        exclude = [
            "src/main.cpp",
        ],
    ),
    hdrs = glob([
        "include/**/*.hpp",
        "src/**/*.hpp",
    ]),
    visibility = ["//visibility:public"],
    linkopts = ["-pthread"],
    deps = [
        "@boost//:property_tree"
    ]
)

cc_binary(
    name = "demo",
    srcs = glob(["src/main.cpp"]),
    deps = [
        "//consumer_producer",
    ],
)

cc_test(
    name = "tests",
    srcs = glob(["tests/**/*.cpp"]),
    deps = [
        "//demo",
        "@googletest//:gtest_main",
    ],
)
