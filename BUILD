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
        ":characters",
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
        "@libgam//:spritemap",
        ":item",
        ":rect",
    ],
)

cc_library(
    name = "camera",
    srcs = ["camera.cc"],
    hdrs = ["camera.h"],
    deps = [
        ":planet",
        ":characters",
    ],
)

cc_library(
    name = "rect",
    srcs = ["rect.cc"],
    hdrs = ["rect.h"],
    deps = [
        "@libgam//:graphics",
    ]
)

cc_library(
    name = "characters",
    srcs = [
        "character.cc",
        "player.cc",
    ],
    hdrs = [
        "character.h",
        "player.h",
    ],
    deps = [
        "@libgam//:spritemap",
        ":planet",
        ":rect",
    ],
)

cc_library(
    name = "item",
    srcs = ["item.cc"],
    hdrs = ["item.h"],
    deps = [
        "@libgam//:spritemap",
        ":rect",
    ],
)
