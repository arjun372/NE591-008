/**
* @file ResourceMonitor.h
* @brief Header file for the ResourceMonitor class.
* @author Arjun Earthperson
* @date 10/20/2023
*/

#ifndef NE591_008_RESOURCE_MONITOR_H
#define NE591_008_RESOURCE_MONITOR_H

#include <algorithm>
#include <cstddef>
#include <unordered_set>
#include <unordered_map>

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
   static ResourceMonitor<T>* monitor;
   std::unordered_map<T*, size_t> maxInstanceSizes; ///< Map to keep track of instances.

   /**
    * @brief Private constructor for the Singleton pattern.
    */
   ResourceMonitor() {

   }

   void checkAndUpdateMaxSize(T* instance) {
       if (instance == nullptr) {
           return;
       }
       const size_t newSize = instance->getAllocatedBytes();
       auto it = maxInstanceSizes.find(instance);
       if (it != maxInstanceSizes.end()) {
           // Key exists in the map, check if the current size is greater than the stored value
           size_t currentSize = newSize;
           if (currentSize > it->second) {
               // Update the value in the map
               it->second = currentSize;
           }
       } else {
           // Key does not exist in the map, you might want to handle this case as well
           // For example, you could insert the current size of the instance into the map
           maxInstanceSizes[instance] = newSize;
       }
   }

 public:

   /**
    * @brief Deleted assignment operator to prevent copying.
    */
   ResourceMonitor& operator=(const ResourceMonitor&) = delete;

   /**
    * @brief Deleted copy constructor to prevent copying.
    */
   ResourceMonitor(const ResourceMonitor&) = delete;

   /**
    * @brief Method to register a new instance.
    *
    * Inserts the instance into the set and updates the maximum instances ever if necessary.
    *
    * @param instance Pointer to the instance to register.
    */
   static void registerInstance(T* instance) {
       getInstance().checkAndUpdateMaxSize(instance);
   }

   // TODO:: DOCUMENT
   static ResourceMonitor<T>& getInstance() {
       if (!monitor) {
           monitor = new ResourceMonitor<T>();
       }
       return *monitor;
   }

   static void clear() {
       getInstance().maxInstanceSizes.clear();
   }

   /**
    * @brief Method to unregister an instance.
    *
    * Erases the instance from the set.
    *
    * @param instance Pointer to the instance to unregister.
    */
   static void unregisterInstance(T* instance) {
       getInstance().checkAndUpdateMaxSize(instance);
   }

   // TODO Document.
   [[nodiscard]] static size_t getMaxBytesEver() {
       size_t sum = 0;
       for (const auto& pair : getInstance().maxInstanceSizes) {
           sum += pair.second;
       }
       return sum;
   }

   [[nodiscard]] static size_t getCurrentInstanceCount() {
       return getInstance().maxInstanceSizes.size();
   }

};

template <typename T>
ResourceMonitor<T>* ResourceMonitor<T>::monitor = nullptr;


#endif //NE591_008_RESOURCE_MONITOR_H
