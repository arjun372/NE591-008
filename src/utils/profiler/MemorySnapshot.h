#ifndef NE591_008_MEMORYSNAPSHOT_H
#define NE591_008_MEMORYSNAPSHOT_H

#include "json.hpp"

#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"
#include "physics/diffusion/DiffusionMatrix.h"
//#include "ResourceMonitor.h"
#include "json.hpp"

template <typename T>
class ResourceMonitor;

struct Snapshot {
    size_t _max_instances = 0;
    size_t _max_bytes = 0;
    size_t _instances = 0;
    size_t _bytes = 0;
};

/**
 * @struct MemorySnapshot
 * @brief A structure to hold the summary statistics of memory allocations for tracked variables
 * @tparam T The type of the elements in the vector or matrix.
 */
struct MemorySnapshot {

    const Snapshot _matrix;      ///< Snapshot for matrix allocations.
    const Snapshot _vector;      ///< Snapshot for vector allocations.
    const Snapshot _lazy_matrix; ///< Snapshot for lazy matrix allocations.

    /**
     * @brief Constructs a MemorySnapshot object, optionally initializing the snapshots based on current allocations.
     * @param allocated If true, initializes the snapshots based on current allocations.
     */
    explicit MemorySnapshot(bool allocated = false)
        : _matrix(MemorySnapshot::click<MyBLAS::Matrix<MyBLAS::NumericType>>(allocated)),
          _vector(MemorySnapshot::click<MyBLAS::Vector<MyBLAS::NumericType>>(allocated)),
          _lazy_matrix(MemorySnapshot::click<MyBLAS::LazyMatrix<MyBLAS::NumericType>>(allocated)) {
        std::cout<<*this;
    }

    /**
     * @brief Creates a Snapshot for a given resource type.
     * @tparam ResourceType The type of the resource to create a snapshot for.
     * @param allocated If true, considers only currently allocated resources.
     * @return A Snapshot containing the number of instances and bytes used by the resource type.
     */
    template <typename ResourceType>
    static Snapshot click(bool allocated = false) {
        return {
            ._max_instances = ResourceMonitor<ResourceType>::getMaxInstancesEver(),
            ._max_bytes = ResourceMonitor<ResourceType>::getTotalBytes(allocated),
            ._instances = ResourceMonitor<ResourceType>::getCurrentInstanceCount(),
            ._bytes = ResourceMonitor<ResourceType>::getTotalBytes(allocated),
        };
    }

    /**
     * @brief Gets the total number of bytes used by all tracked resources.
     * @return The total number of bytes.
     */
    [[nodiscard]] size_t getTotalBytes() const {
        return _matrix._bytes + _vector._bytes + _lazy_matrix._bytes;
    }

    /**
     * @brief Overloaded stream insertion operator to print the summary statistics.
     * @param os The output stream.
     * @param summary The summary statistics to print.
     * @return The output stream.
     */
//    friend std::ostream &operator<<(std::ostream &os, const MemorySnapshot &stats) {
//        os << " :::: TOTAL BYTES: "<<stats.getTotalBytes();
//        return os;
//    }

    friend std::ostream &operator<<(std::ostream &os, const MemorySnapshot &snapshot) {
        const auto width = 15; // Width for each field
        const auto precision = 2; // Decimal precision for printing numbers

        // Set formatting for the output stream
        os << std::left << std::setprecision(precision) << std::setw(width) << std::setfill(' ');

        // Print the header
        os << "MemorySnapshot:" << std::endl;
        os << std::setw(width) << "Resource" << std::setw(width) << "Instances" << std::setw(width) << "Max Instances" << std::setw(width) << "Bytes" << std::setw(width) << "Max Bytes" << std::endl;
        os << std::string(3 * width, '-') << std::endl; // Separator line

        // Print the details for each resource type
        os << std::setw(width) << "Matrix" << std::setw(width) << snapshot._matrix._instances << std::setw(width) << snapshot._matrix._max_instances << std::setw(width) << snapshot._matrix._bytes << snapshot._matrix._max_bytes << std::setw(width) << std::endl;
        os << std::setw(width) << "Vector" << std::setw(width) << snapshot._vector._instances << std::setw(width) << snapshot._vector._max_instances << std::setw(width) << snapshot._vector._bytes << snapshot._vector._max_bytes << std::setw(width) << std::endl;
        os << std::setw(width) << "LazyMatrix" << std::setw(width) << snapshot._lazy_matrix._instances << std::setw(width) << snapshot._lazy_matrix._max_instances << std::setw(width) << snapshot._lazy_matrix._bytes << snapshot._lazy_matrix._max_bytes << std::setw(width) << std::endl;

        // Reset stream formatting to defaults
        os << std::right << std::setfill(' ') << std::setprecision(6);

        return os;
    }

    /**
     * @brief Serializes the memory snapshot to a JSON object.
     * @param jsonMap The JSON object to serialize to.
     */
    void toJSON(nlohmann::json &jsonMap) const {
        jsonMap["bytes"] = getTotalBytes();
    }
};
#endif // NE591_008_MEMORYSNAPSHOT_H
