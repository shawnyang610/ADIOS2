/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * helloBPZfpWriter_nompi.cpp sequential non-mpi version of helloBPZfpWrapper
 * using  Zfp https://computation.llnl.gov/projects/floating-point-compression
 *
 *  Created on: Jul 26, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include <cstdint>   //std::int32_t
#include <ios>       //std::ios_base::failure
#include <iostream>  //std::cout
#include <numeric>   //std::iota
#include <stdexcept> //std::invalid_argument std::exception
#include <vector>

#include <adios2.h>

int main(int argc, char *argv[])
{
    /** Application variable uints from 0 to 100 */
    std::vector<float> myFloats(100);
    std::iota(myFloats.begin(), myFloats.end(), 0.f);
    const std::size_t Nx = myFloats.size();
    const std::size_t inputBytes = Nx * sizeof(float);

    try
    {
        /** ADIOS class factory of IO class objects, DebugON is recommended */
        adios2::ADIOS adios(adios2::DebugON);

        // Get a Transform of type BZip2
        adios2::Operator adiosZfp =
            adios.DefineOperator("ZfpVariableCompressor", "zfp");

        /*** IO class object: settings and factory of Settings: Variables,
         * Parameters, Transports, and Execution: Engines */
        adios2::IO bpIO = adios.DeclareIO("BPVariable_Zfp");

        /** global array : name, { shape (total) }, { start (local) }, { count
         * (local) }, all are constant dimensions */
        adios2::Variable<float> bpFloats = bpIO.DefineVariable<float>(
            "bpUInts", {}, {}, {Nx}, adios2::ConstantDims);

        // Adding transform metadata to variable:
        // Options:
        // Rate double
        // Tolerance
        // Precision
        const unsigned int zfpID =
            bpFloats.AddOperator(adiosZfp, {{"Rate", "8"}});

        // Treat Transforms as wrappers to underlying library (zfp):
        const std::size_t estimatedSize =
            adiosZfp.BufferMaxSize(myFloats.data(), bpFloats.Count(),
                                   bpFloats.OperatorsInfo()[zfpID].Parameters);

        // Compress
        std::vector<char> compressedBuffer(estimatedSize);

        size_t compressedSize = adiosZfp.Compress(
            myFloats.data(), bpFloats.Count(), bpFloats.Sizeof(),
            bpFloats.Type(), compressedBuffer.data(),
            bpFloats.OperatorsInfo()[zfpID].Parameters);

        compressedBuffer.resize(compressedSize);

        std::cout << "Compression summary:\n";
        std::cout << "Input data size: " << inputBytes << " bytes\n";
        std::cout << "Zfp estimated output size: " << estimatedSize
                  << " bytes\n";
        std::cout << "Zfp final output size: " << compressedSize
                  << " bytes\n\n";

        // Decompress, allocate original data size
        std::vector<float> decompressedBuffer(Nx);
        size_t decompressedSize = adiosZfp.Decompress(
            compressedBuffer.data(), compressedBuffer.size(),
            decompressedBuffer.data(), bpFloats.Count(), bpFloats.Type(),
            bpFloats.OperatorsInfo()[zfpID].Parameters);

        std::cout << "Decompression summary:\n";
        std::cout << "Decompressed size: " << decompressedSize << " bytes\n ";
        std::cout << "Data:\n";

        for (const auto number : decompressedBuffer)
        {
            if (static_cast<int>(number) % 25 == 0 && number != 0)
            {
                std::cout << "\n";
            }
            std::cout << number << " ";
        }
        std::cout << "\n";
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Invalid argument exception, STOPPING PROGRAM\n";
        std::cout << e.what() << "\n";
    }
    catch (std::ios_base::failure &e)
    {
        std::cout << "IO System base failure exception, STOPPING PROGRAM\n";
        std::cout << e.what() << "\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Exception, STOPPING PROGRAM\n";
        std::cout << e.what() << "\n";
    }

    return 0;
}
