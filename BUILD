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
        "planet_screen.cc",
        "ship_screen.cc",
        "title_screen.cc",
    ],
    hdrs = [
        "planet_screen.h",
        "ship_screen.h",
        "title_screen.h",
    ],
    deps = [
        "@libgam//:backdrop",
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
        "enemy.cc",
        "player.cc",
    ],
    hdrs = [
        "character.h",
        "enemy.h",
        "player.h",
    ],
    deps = [
        "@libgam//:audio",
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
