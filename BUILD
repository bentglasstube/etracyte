package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "etracyte",
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
        "game_state.h",
        "planet_screen.h",
        "ship_screen.h",
        "title_screen.h",
    ],
    deps = [
        "@libgam//:backdrop",
        "@libgam//:screen",
        "@libgam//:spritemap",
        "@libgam//:text",
        "@libgam//:util",
        ":appearing_text",
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
        "@libgam//:util",
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
        "@libgam//:text",
        "@libgam//:util",
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

cc_library(
    name = "appearing_text",
    srcs = ["appearing_text.cc"],
    hdrs = ["appearing_text.h"],
    deps = [
        "@libgam//:audio",
        "@libgam//:text",
    ],
)
