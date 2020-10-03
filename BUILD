package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "ld47",
    data = ["//content"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
    ],
    srcs = ["main.cc"],
    deps = [
        "@libgam//:game",
        ":screens",
    ],
)

cc_library(
    name = "screens",
    srcs = [
        "title_screen.cc",
        "planet_screen.cc",
    ],
    hdrs = [
        "title_screen.h",
        "planet_screen.h",
    ],
    deps = [
        "@libgam//:screen",
        "@libgam//:spritemap",
        "@libgam//:text",
        ":camera",
        ":planet",
    ],
)

cc_library(
    name = "planet",
    srcs = ["planet.cc"],
    hdrs = [
        "planet.h",
        "stb_perlin.h",
    ],
    deps = [
        "@libgam//:graphics",
        "@libgam//:spritemap",
    ],
)

cc_library(
    name = "camera",
    srcs = ["camera.cc"],
    hdrs = ["camera.h"],
    deps = [
        ":planet",
    ],
)

