#include <vector>
#include <tuple>
std::tuple<location_t,std::tuple<std::vector<uint64_t>,uint64_t,last_used_loc_t>> get_next_free_location(location_stack_t location_stack);
std::vector<uint64_t>& evict_location(std::vector<uint64_t>& location_stack);
auto cache_is_full(location_stack_ptr_t location_stack_ptr);
std::tuple<std::vector<uint64_t>,std::tuple<std::vector<uint64_t>,address_to_cache_loc_t,std::vector<uint64_t>,std::tuple<location_stack_storage_t,location_stack_ptr_t,last_used_loc_t>>> write_to_mem_on_evict(std::vector<uint64_t>& memory,cache_t cache,address_t address,data_t data);
std::tuple<std::vector<uint64_t>&,std::tuple<std::vector<uint64_t>,std::vector<uint64_t>,std::vector<uint64_t>,std::tuple<location_stack_storage_t,location_stack_ptr_t,uint64_t>>> read_from_mem_on_miss(std::vector<uint64_t>& memory,cache_t cache,address_t address);
std::tuple<memory_t,std::tuple<std::vector<uint64_t>,std::vector<uint64_t>,cache_loc_to_address_t,std::tuple<location_stack_storage_t,location_stack_ptr_t,uint64_t>>> update_cache_entry(cache_t cache,address_t address,data_t data);
uint64_t fetch_cache_entry(cache_t cache,address_t address);
std::tuple<memory_t,std::tuple<cache_storage_t,std::vector<uint64_t>,cache_loc_to_address_t,std::tuple<location_stack_storage_t,location_stack_ptr_t,last_used_loc_t>>> write_data_to_cache(memory_t memory,cache_t cache,address_t address,data_t data);
std::tuple<uint64_t,memory_t,std::tuple<cache_storage_t,address_to_cache_loc_t,cache_loc_to_address_t,uint64_t>> read_data_from_cache(memory_t memory,address_t address,cache_t cache);
