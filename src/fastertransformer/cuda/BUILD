load("//:def.bzl", "rpm_library", "torch_deps", "copts", "cuda_copts", "rocm_copts")

cc_library(
    name = "cublas",
    srcs = [
        "cublas/cublasAlgoMap.cc",
        # "cublas/cublasFP8MMWrapper.cc",
        "cublas/cublasINT8MMWrapper.cc",
        "cublas/cublasMMWrapper.cc",
    ],
    hdrs = [
        "cublas/cublasAlgoMap.h",
        # "cublas/cublasFP8MMWrapper.h",
        "cublas/cublasINT8MMWrapper.h",
        "cublas/cublasMMWrapper.h",
        "cublas/cublas.h",
    ],
    deps = [
        ":cuda_utils",
        "//src/fastertransformer/core:allocator",
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
    ],
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cufmha",
    srcs = [
        "cufmha/cufmha.cc",
    ],
    hdrs = [
        "cufmha/cufmha.h",
    ],
    deps = [
        ":cuda_utils",
        "//src/fastertransformer/core:types",
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
        "@flash_attention//:flash_attention2_header",
        "//3rdparty/contextFusedMultiHeadAttention:trt_fmha_header",
        "//3rdparty/trt_fused_multihead_attention:trt_fused_multihead_attention_header",
        ":memory_utils",
        "//src/fastertransformer/kernels:kernels",
        "//3rdparty/contextFusedMultiHeadAttention:trt_fmha_impl",
        "//3rdparty/trt_fused_multihead_attention:trt_fused_multihead_attention_impl",
    ],
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cuggemm",
    srcs = [
        "cuggemm/cuggemm.cc",
    ],
    hdrs = [
        "cuggemm/cuggemm.h",
    ],
    deps = [
        ":cuda_utils",
        "//src/fastertransformer/core:types",
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
        "//src/fastertransformer/cutlass:cutlass_interface",
        # TODO(lidongjin) cutlass_interface dependece gpt_init_params_hdr is not good.
        "//:gpt_init_params_hdr",
    ],
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "custom_ar",
    srcs = [
        "custom_ar/custom_ar_comm.cc",
    ],
    hdrs = [
        "custom_ar/custom_ar_comm.h"
    ],
    deps = [
        ":nccl",
    ] + select({
        "//:using_cuda": [  ":cuda_utils",
                            ":memory_utils",
                            "//src/fastertransformer/core:types",
                            "//src/fastertransformer/kernels:kernels_cu",
                            "@local_config_cuda//cuda:cuda",
                            "@local_config_cuda//cuda:cudart",],
        "//:using_rocm": [  "//src/fastertransformer/rocm:memory_utils",
                            "//src/fastertransformer/rocm:rocm_utils",
                            "//src/fastertransformer/kernels:gpt_kernels_rocm",]}),

                            
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "nccl",
    srcs = [
        "nccl/nccl_utils_torch.cc",
        "nccl/nccl_utils.cc",
    ],
    hdrs = [
        "nccl/nccl_utils_torch.h",
        "nccl/nccl_utils.h",
    ],
    deps = torch_deps() + select({
        "//:using_cuda": [":cuda_utils",
                          "@local_config_cuda//cuda:cuda",
                          "@local_config_cuda//cuda:cudart",],
        "//:using_rocm": ["//src/fastertransformer/rocm:rocm_utils"],
        "//conditions:default": [],
    }),
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "nvtx",
    srcs = [
        "nvtx/nvtx_utils.cc",
        "nvtx/kernel_profiler.cc"
    ],
    hdrs = [
        "nvtx/nvtx_utils.h",
        "nvtx/kernel_profiler.h"
    ],
    deps = [
        "//maga_transformer/cpp:metrics",
    ] + select({
        "//:using_cuda": [":cuda_utils",
                          "@local_config_cuda//cuda:cuda",
                          "@local_config_cuda//cuda:cudart",],
        "//:using_rocm": ["//src/fastertransformer/rocm:rocm_utils"],
        "//conditions:default": [],
    }),
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "memory_utils",
    srcs = [
        "memory_utils.cu",
    ],
    hdrs = [
        "memory_utils.h",

    ],
    deps = [
        "//:gpt_init_params_hdr",
        "//src/fastertransformer/cutlass:cutlass_interface",
        ":cuda_utils",
        "//src/fastertransformer/utils:utils",
        "//src/fastertransformer/core:Tensor",
        "//src/fastertransformer/core:allocator",
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
    ],
    copts = cuda_copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cuda_utils_cu",
    srcs = [
        "cuda_fp8_utils.cu"
    ],
    hdrs = [
        "cuda_fp8_utils.h",
        "cuda_type_utils.cuh",
        "cuda_bf16_fallbacks.cuh",
    ],
    deps = [
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
    ],
    copts = cuda_copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cuda_fmha_utils",
    hdrs = [
        "cuda_fmha_utils.h",
    ],
    deps = [
        ":cuda_utils",
        "//src/fastertransformer/utils:utils",
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
    ],
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cuda_utils",
    srcs = [
        "cuda_utils.cc",
    ],
    hdrs = [
        "cuda_utils.h",
        "Dispatch.h",
        "trt_utils.h"
    ],
    deps = [
        ":cuda_utils_cu",
        "//src/fastertransformer/utils:utils",
        "//src/fastertransformer/core:types",
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
    ],
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cuda",
    srcs = ["gemm.cc"],
    hdrs = [
        "custom_ar_comm.h",
        "gemm.h",
        "Dispatch.h"],
    deps = [
        ":nccl",
        ":nvtx",
        ":cublas",
        ":cufmha",
        "cuggemm",
        ":cuda_utils",
        ":cuda_fmha_utils",
        ":custom_ar",
        ":memory_utils",
        "//src/fastertransformer/kernels:kernels",
        "//src/fastertransformer/utils:utils",
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
    ],
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "allocator_cuda",
    srcs = [
        "allocator_cuda.cc"
    ],
    hdrs = [
        "allocator_cuda.h"
    ],
    copts = cuda_copts(),
    deps = [
        "//src/fastertransformer/core:allocator",
        ":cuda_utils",
    ],
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "allocator_torch",
    srcs = [
        "allocator_torch.cc"
    ],
    hdrs = [
        "allocator_torch.h"
    ],
    copts = copts(),
    deps = torch_deps() + [
        ":allocator_cuda",
    ],
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "tensor_utils",
    srcs = [
        "tensor_utils.cc"
    ],
    hdrs = [
        "tensor_utils.h"
    ],
    copts = copts(),
    deps = torch_deps() + [
        "//src/fastertransformer/core:Tensor",
    ],
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "expert_attention_util",
    srcs = [
        "ExpertAttentionUtil.cc"
    ],
    hdrs = [
        "ExpertAttentionUtil.h"
    ],
    copts = copts(),
    deps = torch_deps() + [
        ":cuda_utils",
        "//src/fastertransformer/core:allocator",
        "@local_config_cuda//cuda:cuda",
        "@local_config_cuda//cuda:cudart",
    ],
    include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_library(
    name = "cuda_utils_rocm",
    srcs = [
    ],
    hdrs = [
        "cuda_utils.h",
        "Dispatch.h",
        "memory_utils.h",
        "cuda_fp8_utils.h",
        "cuda_type_utils.cuh",
        "cuda_bf16_fallbacks.cuh",
    ],
    deps = [
        "//src/fastertransformer/rocm:rocm_types_hdr",
        "//src/fastertransformer/core:types",
        "//src/fastertransformer/rocm:rocm_utils",
        "//src/fastertransformer/core:tensor_hdr",
    ],
    copts = copts(),
    include_prefix = "src",
    visibility = ["//visibility:public"],
)
