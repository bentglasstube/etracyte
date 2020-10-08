load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "libgam",
    remote = "https://git.sr.ht/~bentglasstube/gam",
    commit = "f29b3cc2fa9e16496b9071a1ef1d0edb59a8477c",
)

git_repository(
    name = "mxebzl",
    remote = "https://github.com/cfrantz/mxebzl.git",
    tag = "20191215_RC01",
)

load("@mxebzl//compiler:repository.bzl", "mxe_compiler")

mxe_compiler(
    deps = [
        "compiler",
        "SDL2",
        "SDL2-extras",
        "xz",
        "pthreads",
    ],
)
