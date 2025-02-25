load("@//:def.bzl", "cuda_copts", "torch_deps", "copts")
package(default_visibility = ["//visibility:public"])

config_setting(
    name = "using_arm",
    values = {"define": "using_arm=true"},
)

cc_library(
    name = "flash_attention2_header",
    hdrs = [
        "@//3rdparty/flash_attention:flash_api.h",
        "csrc/flash_attn/src/flash.h",
    ],
    deps = [
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
    ] + torch_deps(),
    copts = cuda_copts(),
    visibility = ["//visibility:public"],
)

cc_library(
    name = "flash_attention2_impl",
    srcs = glob([
        "csrc/flash_attn/src/flash_fwd_hdim128_bf16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_hdim128_fp16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_hdim96_bf16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_hdim96_fp16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_hdim64_bf16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_hdim64_fp16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_split_hdim128_bf16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_split_hdim128_fp16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_split_hdim96_bf16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_split_hdim96_fp16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_split_hdim64_bf16_sm80.cu",
        "csrc/flash_attn/src/flash_fwd_split_hdim64_fp16_sm80.cu",
        "csrc/flash_attn/src/*.cpp"
    ]),
    hdrs = glob([
        "csrc/flash_attn/src/*.h",
        "csrc/flash_attn/src/*.cuh",
    ], exclude=["csrc/flash_attn/src/flash.h"]),
    deps = [
        "@cutlass//:cutlass",
        ":flash_attention2_header",
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
    ] + torch_deps(),
    copts = cuda_copts(),
    visibility = ["//visibility:public"],
    alwayslink = True,
)
