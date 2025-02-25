#include "src/fastertransformer/devices/arm_impl/ArmDevice.h"
#include "src/fastertransformer/devices/DeviceFactory.h"
#include "src/fastertransformer/core/allocator.h"
#include "src/fastertransformer/core/cpu_allocator.h"
#include <cstring>

namespace fastertransformer {

/* Apply mask to input.
    Different heads share the same mask. */
template<typename T>
void context_mask(BufferPtr input, const Buffer& mask) {
    for (size_t dim0 = 0; dim0 < input->shape()[0]; dim0++) {
        for (size_t dim1 = 0; dim1 < input->shape()[1]; dim1++) {
            for (size_t dim2 = 0; dim2 < input->shape()[2]; dim2++) {
                for (size_t dim3 = 0; dim3 < input->shape()[3]; dim3++) {
                    auto v = input->dataWithOffset(
                        ((dim0 * input->shape()[1] + dim1) * input->shape()[2] + dim2) * input->shape()[3] + dim3);
                    auto m = mask.dataWithOffset((dim0 * input->shape()[2] + dim2) * input->shape()[3] + dim3);
                    *(T*)v += (1.0f - *(T*)m) * -10000.0f;
                }
            }
        }
    }
}

BufferPtr ArmCpuDevice::softmax(const SoftmaxParams& params) {
    if (params.input == nullptr) {
        throw std::runtime_error("softmax input can not be nullptr");
    }
    auto        type  = params.input->type();
    const auto& input = params.input;
    auto output = allocateBuffer({params.output_t == DataType::TYPE_INVALID ? params.input->type() : params.output_t,
                                  params.input->shape(),
                                  AllocationType::HOST});

    size_t type_size = params.input->typeSize();
    if ((type_size != 4) && (type_size != 2)) {
        throw std::runtime_error("Softmax input type is not supported");
    }

    if (params.mask.has_value()) {
        /* Apply mask. */
        auto mask_type = params.mask.value().get().type();
        if (mask_type != type) {
            throw std::runtime_error("Inconsistent softmax input type and mask type is not supported");
        }
        if (type == DataType::TYPE_FP32) {
            context_mask<float>(params.input, params.mask.value().get());
        } else if (type == DataType::TYPE_FP16) {
            context_mask<__fp16>(params.input, params.mask.value().get());
        } else {
            throw std::runtime_error("Softmax data type is not supported");
        }
    }

    arm_compute::DataType   acl_data_type = getAclDataType(type);
    arm_compute::TensorInfo data_info     = arm_compute::TensorInfo(
        arm_compute::TensorShape(input->shape()[3], input->shape()[2], input->shape()[1], input->shape()[0]),
        1,
        acl_data_type);

    arm_compute::NESoftmaxLayer softmax;
    arm_compute::Tensor         src_tensor;
    arm_compute::Tensor         dst_tensor;

    src_tensor.allocator()->init(data_info);
    dst_tensor.allocator()->init(data_info);
    src_tensor.allocator()->import_memory(input->data());
    dst_tensor.allocator()->import_memory(output->data());
    float beta = params.scale;

    softmax.configure(&src_tensor, &dst_tensor, beta);
    softmax.run();

    src_tensor.allocator()->free();
    dst_tensor.allocator()->free();

    return std::move(output);
}

}  // namespace fastertransformer
