/**
* @file ResourceMonitor.h
* @brief Header file for the ResourceMonitor class.
* @author Arjun Earthperson
* @date 10/20/2023
*/

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
   std::unordered_set<T*> instances; ///< Set to keep track of instances.
   size_t maxInstancesEver;          ///< Record of the maximum instances ever.

   /**
    * @brief Private constructor for the Singleton pattern.
    *
    * Initializes the maximum instances ever to zero.
    */
   ResourceMonitor() : maxInstancesEver(0) {}

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
   void registerInstance(T* instance) {
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
   void unregisterInstance(T* instance) {
       instances.erase(instance);
   }

   /**
    * @brief Method to get the current number of instances.
    *
    * @return The current number of instances of type T.
    */
   [[nodiscard]] size_t getCurrentInstanceCount() const {
       return instances.size();
   }

   /**
    * @brief Method to get the maximum number of instances ever.
    *
    * @return The maximum number of instances of type T that have ever existed at the same time.
    */
   [[nodiscard]] size_t getMaxInstancesEver() const {
       return maxInstancesEver;
   }
};
