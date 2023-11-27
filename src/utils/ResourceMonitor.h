/**
* @file ResourceMonitor.h
* @brief Header file for the ResourceMonitor class.
* @author Arjun Earthperson
* @date 10/20/2023
*/

#ifndef NE591_008_RESOURCE_MONITOR_H
#define NE591_008_RESOURCE_MONITOR_H

#include <unordered_set>
#include <algorithm>
#include <cstddef>

/**
* @brief Singleton class that monitors resource instances.
*
* This class is used to keep track of instances of type T and record the maximum number of instances
* that have ever been created at any point in time.
*
* @tparam T The type of resource to monitor.
*/
template <typename T>
class ResourceMonitor {
 private:
   static std::unordered_set<T*> instances; ///< Set to keep track of instances.
   static size_t maxInstancesEver;          ///< Record of the maximum instances ever.

   /**
    * @brief Private constructor for the Singleton pattern.
    */
   ResourceMonitor() = default;

 public:
   /**
    * @brief Deleted copy constructor to prevent copying.
    */
   ResourceMonitor(const ResourceMonitor&) = delete;

   /**
    * @brief Deleted assignment operator to prevent copying.
    */
   ResourceMonitor& operator=(const ResourceMonitor&) = delete;

   /**
    * @brief Method to access the singleton instance of the ResourceMonitor.
    *
    * @return Reference to the singleton instance of the ResourceMonitor.
    */
   static ResourceMonitor& getInstance() {
       static ResourceMonitor instance; // Guaranteed to be destroyed and instantiated on first use.
       return instance;
   }

   /**
    * @brief Method to register a new instance.
    *
    * Inserts the instance into the set and updates the maximum instances ever if necessary.
    *
    * @param instance Pointer to the instance to register.
    */
   static void registerInstance(T* instance) {
       instances.insert(instance);
       maxInstancesEver = std::max(maxInstancesEver, instances.size());
   }

   /**
    * @brief Method to unregister an instance.
    *
    * Erases the instance from the set.
    *
    * @param instance Pointer to the instance to unregister.
    */
   static void unregisterInstance(T* instance) {
       instances.erase(instance);
   }

   /**
    * @brief Method to get the current number of instances.
    *
    * @return The current number of instances of type T.
    */
   [[nodiscard]] static size_t getCurrentInstanceCount() {
       return instances.size();
   }

   /**
    * @brief Method to get the maximum number of instances ever.
    *
    * @return The maximum number of instances of type T that have ever existed at the same time.
    */
   [[nodiscard]] static size_t getMaxInstancesEver() {
       return maxInstancesEver;
   }

   /**
     * @brief Calculates the total memory allocated by all instances.
     *
     * This static method sums up the memory allocated by each instance of type T.
     * Each instance must implement a method getAllocatedBytes that returns
     * the amount of memory it has allocated.
     *
     * @param allocated If true, calculates the actual memory allocated; otherwise, estimates the memory.
     * @return The total memory allocated by all instances of type T.
    */
   static size_t getTotalBytes(bool allocated = false) {
       size_t totalBytes = 0;
       for (const T* instance : instances) {
           totalBytes += instance->getAllocatedBytes(allocated);
       }
       return totalBytes;
   }

   /**
     * @brief Estimates the maximum memory that could be allocated by all instances.
     *
     * This static method finds the instance with the highest allocated bytes and multiplies it
     * by the maximum number of instances ever to estimate the maximum memory usage.
     *
     * @param allocated If true, calculates the actual memory allocated; otherwise, estimates the memory used.
     * @return The estimated maximum memory that could be allocated by all instances of type T.
    */
   static size_t estimateMaximumBytes(bool allocated = false) {
       size_t maxBytesPerInstance = 0;
       for (const T* instance : instances) {
           size_t instanceBytes = instance->getAllocatedBytes(allocated);
           maxBytesPerInstance = std::max(maxBytesPerInstance, instanceBytes);
       }
       return maxBytesPerInstance * getMaxInstancesEver();
   }

   /**
     * @brief Finds an instance in the set of tracked instances.
     *
     * This method searches for the given instance in the set and returns an iterator to it
     * if found, or to the end of the set otherwise.
     *
     * @param instance The instance to search for.
     * @return An iterator to the instance if found, or to the end of the set otherwise.
    */
   static typename std::unordered_set<T*>::const_iterator find(const T* instance) {
       return instances.find(const_cast<T*>(instance));
   }

   /**
     * @brief Provides access to the begin iterator of the set of instances.
     *
     * @return A const_iterator to the beginning of the set of instances.
    */
   static typename std::unordered_set<T*>::const_iterator begin() {
       return instances.begin();
   }

   /**
     * @brief Provides access to the end iterator of the set of instances.
     *
     * @return A const_iterator to the end of the set of instances.
    */
   static typename std::unordered_set<T*>::const_iterator end() {
       return instances.end();
   }
};

template <typename T>
std::unordered_set<T*> ResourceMonitor<T>::instances;

template <typename T>
size_t ResourceMonitor<T>::maxInstancesEver = 0;

#endif //NE591_008_RESOURCE_MONITOR_H
