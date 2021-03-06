/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * BP3MGARD.h
 *
 *  Created on: Nov 16, 2018
 *      Author: William F Godoy godoywf@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_FORMAT_BP3_OPERATION_BP3MGARD_H_
#define ADIOS2_TOOLKIT_FORMAT_BP3_OPERATION_BP3MGARD_H_

#include "adios2/toolkit/format/bp3/operation/BP3Operation.h"

namespace adios2
{
namespace format
{

class BP3MGARD : public BP3Operation
{
public:
    BP3MGARD() = default;

    ~BP3MGARD() = default;

#define declare_type(T)                                                        \
    void SetData(const core::Variable<T> &variable,                            \
                 const typename core::Variable<T>::Info &blockInfo,            \
                 const typename core::Variable<T>::Operation &operation,       \
                 BufferSTL &bufferSTL) const noexcept final;                   \
                                                                               \
    void SetMetadata(const core::Variable<T> &variable,                        \
                     const typename core::Variable<T>::Info &blockInfo,        \
                     const typename core::Variable<T>::Operation &operation,   \
                     std::vector<char> &buffer) const noexcept final;          \
                                                                               \
    void UpdateMetadata(                                                       \
        const core::Variable<T> &variable,                                     \
        const typename core::Variable<T>::Info &blockInfo,                     \
        const typename core::Variable<T>::Operation &operation,                \
        std::vector<char> &buffer) const noexcept final;

    ADIOS2_FOREACH_MGARD_TYPE_1ARG(declare_type)
#undef declare_type

    void GetMetadata(const std::vector<char> &buffer, Params &info) const
        noexcept final;

    void GetData(const char *input,
                 const helper::BlockOperationInfo &blockOperationInfo,
                 char *dataOutput) const final;

private:
    template <class T>
    void SetDataCommon(const core::Variable<T> &variable,
                       const typename core::Variable<T>::Info &blockInfo,
                       const typename core::Variable<T>::Operation &operation,
                       BufferSTL &bufferSTL) const noexcept;

    template <class T>
    void GetDataCommon(const char *input,
                       const helper::BlockOperationInfo &blockOperationInfo,
                       T *dataOutput) const;

    template <class T>
    void
    SetMetadataCommon(const core::Variable<T> &variable,
                      const typename core::Variable<T>::Info &blockInfo,
                      const typename core::Variable<T>::Operation &operation,
                      std::vector<char> &buffer) const noexcept;

    template <class T>
    void
    UpdateMetadataCommon(const core::Variable<T> &variable,
                         const typename core::Variable<T>::Info &blockInfo,
                         const typename core::Variable<T>::Operation &operation,
                         std::vector<char> &buffer) const noexcept;
};

} // end namespace format
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_FORMAT_BP3_OPERATION_BP3MGARD_H_ */
