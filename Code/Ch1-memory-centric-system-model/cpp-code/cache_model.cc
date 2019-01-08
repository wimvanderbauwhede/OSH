#include <tuple>
// Initialise the cache
def init_cache() {
// Cache of size CACHE_SZ,cache line=64 bytes=16 words
    cache_storage=[[0]*16]*(CACHE_SZ/16);
    location_stack_storage=range(0,CACHE_SZ/16);
    location_stack_ptr=(CACHE_SZ/16)-1;
    last_used_loc=location_stack[location_stack_ptr];
    location_stack= std::make_tuple(location_stack_storage,location_stack_ptr,last_used_loc);
    address_to_cache_loc={};
    cache_loc_to_address={};
    cache_lookup= std::make_tuple(address_to_cache_loc,cache_loc_to_address);
    cache= std::make_tuple(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack);
    return cache;

// Some helper functions
}//4 > 0

std::tuple<location_t,std::tuple<std::vector<uint64_t>,uint64_t,last_used_loc_t>> get_next_free_location(location_stack_t location_stack){
    last_used_loc_t last_used_loc;
    uint64_t loc;
    uint64_t location_stack_ptr;
    std::vector<uint64_t> location_stack_storage;
    uint64_t None=0;
    std::tie(location_stack_storage,location_stack_ptr,last_used_loc)=location_stack;
    loc=location_stack_storage[location_stack_ptr];
    location_stack_ptr-=1;
    location_stack= std::make_tuple(location_stack_storage,location_stack_ptr,last_used_loc);
    return std::make_tuple(location,location_stack);

}//4 > 0

std::vector<uint64_t>& evict_location(std::vector<uint64_t>& location_stack){
    last_used_loc_t last_used_loc;
    uint64_t location_stack_ptr;
    location_stack_storage_t location_stack_storage;
    uint64_t None=0;
    std::tie(location_stack_storage,location_stack_ptr,last_used_loc)=location_stack;
    location_stack_ptr+=1;
    location_stack[location_stack_ptr]=last_used_loc;
    location_stack= std::make_tuple(location_stack_storage,location_stack_ptr,last_used_loc);
    return location_stack;

}//4 > 0

auto cache_is_full(location_stack_ptr_t location_stack_ptr){
    uint64_t None=0;
    if location_stack_ptr==0;
        return True;
    else;
        return False;

// we also need
}//8 > 0

std::tuple<std::vector<uint64_t>,std::tuple<std::vector<uint64_t>,address_to_cache_loc_t,std::vector<uint64_t>,std::tuple<location_stack_storage_t,location_stack_ptr_t,last_used_loc_t>>> write_to_mem_on_evict(std::vector<uint64_t>& memory,cache_t cache,address_t address,data_t data){
    address_to_cache_loc_t address_to_cache_loc;
    std::vector<uint64_t> cache_line;
    std::vector<uint64_t> cache_loc_to_address;
    std::vector<uint64_t> cache_storage;
    uint64_t cdata;
    uint64_t evicted_address;
    last_used_loc_t last_used_loc;
    std::tuple<location_stack_storage_t,location_stack_ptr_t,last_used_loc_t> location_stack;
    location_stack_ptr_t location_stack_ptr;
    location_stack_storage_t location_stack_storage;
    uint64_t None=0;
    std::tie(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack)=cache;
    std::tie(location_stack_storage,location_stack_ptr,last_used_loc)=location_stack;
// If the cache was full,evict first
    if (cache_is_full(location_stack_ptr) ) {
        location_stack=evict_location(location_stack);
        evicted_address=cache_loc_to_address[last_used_loc];
        cache_line=cache_storage[last_used_loc];
        for (auto i : range(0,16) ) {
            cdata=cache_line[i];
            memory[(evicted_address<<4)+ i]=cdata;
        }//12 > 4
    }//12 > 4
    location_stack= std::make_tuple(location_stack_storage,location_stack_ptr,last_used_loc);
    cache= std::make_tuple(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack);
    return std::make_tuple(memory,cache);

// and
// If the item was not yet in the cache (write miss):
}//4 > 0

std::tuple<std::vector<uint64_t>&,std::tuple<std::vector<uint64_t>,std::vector<uint64_t>,std::vector<uint64_t>,std::tuple<location_stack_storage_t,location_stack_ptr_t,uint64_t>>> read_from_mem_on_miss(std::vector<uint64_t>& memory,cache_t cache,address_t address){
    std::vector<uint64_t> address_to_cache_loc;
    std::vector<uint64_t> cache_line;
    std::vector<uint64_t> cache_loc_to_address;
    std::vector<uint64_t> cache_storage;
    uint64_t last_used_loc;
    loc_t loc;
    std::tuple<location_stack_storage_t,location_stack_ptr_t,uint64_t> location_stack;
    location_stack_ptr_t location_stack_ptr;
    location_stack_storage_t location_stack_storage;
    uint64_t None=0;
    std::tie(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack)=cache;
    std::tie(location_stack_storage,location_stack_ptr,last_used_loc)=location_stack;
    cache_line=[];
// Get a free location. I am assuming that an eviction has been done before if necessary
    std::tie(loc,location_stack)=get_next_free_location(location_stack);
// Get the DRAM content and write it to the cache storage
    for (auto i : range(0,16) ) {
        cache_line.append(memory[((address>>4)<<4)+i];
    }//8 > 4
    cache_storage[loc]=cache_line;
// Update the lookup table and the last used location
    address_to_cache_loc[address>>4]=loc;
    cache_loc_to_address[loc]=address>>4;
    last_used_loc=loc;
    location_stack= std::make_tuple(location_stack_storage,location_stack_ptr,last_used_loc);
    cache= std::make_tuple(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack);
    return std::make_tuple(memory,cache);

}//4 > 0

std::tuple<memory_t,std::tuple<std::vector<uint64_t>,std::vector<uint64_t>,cache_loc_to_address_t,std::tuple<location_stack_storage_t,location_stack_ptr_t,uint64_t>>> update_cache_entry(cache_t cache,address_t address,data_t data){
    std::vector<uint64_t> address_to_cache_loc;
    std::vector<uint64_t> cache_line;
    cache_loc_to_address_t cache_loc_to_address;
    std::vector<uint64_t> cache_storage;
    uint64_t last_used_loc;
    uint64_t loc;
    std::tuple<location_stack_storage_t,location_stack_ptr_t,uint64_t> location_stack;
    location_stack_ptr_t location_stack_ptr;
    location_stack_storage_t location_stack_storage;
    uint64_t None=0;
    std::tie(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack)=cache;
    std::tie(location_stack_storage,location_stack_ptr,last_used_loc)=location_stack;
    loc=address_to_cache_loc[address>>4];
    cache_line=cache_storage[loc];
    cache_line[address & 0xF]=data;
    cache_storage[loc]=cache_line;
// Update the lookup table and the last used location
    last_used_loc=loc;
    location_stack= std::make_tuple(location_stack_storage,location_stack_ptr,last_used_loc);
    cache= std::make_tuple(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack);
    return std::make_tuple(memory,cache);


}//4 > 0

uint64_t fetch_cache_entry(cache_t cache,address_t address){
    std::vector<uint64_t> address_to_cache_loc;
    std::vector<uint64_t> cache_line;
    cache_loc_to_address_t cache_loc_to_address;
    std::vector<uint64_t> cache_storage;
    uint64_t data;
    last_used_loc_t last_used_loc;
    uint64_t loc;
    std::tuple<location_stack_storage_t,location_stack_ptr_t,last_used_loc_t> location_stack;
    location_stack_ptr_t location_stack_ptr;
    location_stack_storage_t location_stack_storage;
    uint64_t None=0;
    std::tie(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack)=cache;
    std::tie(location_stack_storage,location_stack_ptr,last_used_loc)=location_stack;
    loc=address_to_cache_loc[address>>4];
    cache_line=cache_storage[loc];
    data=cache_line[address & 0xF];
    return data;

}//4 > 0

std::tuple<memory_t,std::tuple<cache_storage_t,std::vector<uint64_t>,cache_loc_to_address_t,std::tuple<location_stack_storage_t,location_stack_ptr_t,last_used_loc_t>>> write_data_to_cache(memory_t memory,cache_t cache,address_t address,data_t data){
    std::vector<uint64_t> address_to_cache_loc;
    std::vector<uint64_t> cache_line;
    cache_loc_to_address_t cache_loc_to_address;
    cache_storage_t cache_storage;
    last_used_loc_t last_used_loc;
    uint64_t loc;
    std::tuple<location_stack_storage_t,location_stack_ptr_t,last_used_loc_t> location_stack;
    location_stack_ptr_t location_stack_ptr;
    location_stack_storage_t location_stack_storage;
    uint64_t None=0;
    std::tie(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack)=cache;
    std::tie(location_stack_storage,location_stack_ptr,last_used_loc)=location_stack;
// If the cache was full,evict first
    if (cache_is_full(location_stack_ptr) ) {
        std::tie(memory,cache)=write_to_mem_on_evict(memory,cache,address,data);

// If the item was not yet in the cache (write miss):
    }//8 > 4
    cache_line=[];
    if (address not in address_to_cache_loc ) {
        std::tie(memory,cache)=read_from_mem_on_miss(memory,cache,address);
     } else {
// Find the cache location and cache line entry and update
        std::tie(memory,cache)=update_cache_entry(cache,address,data);
        loc=address_to_cache_loc[address>>4];
    }//8 > 4
    return std::make_tuple(memory,cache);

}//4 > 0

std::tuple<uint64_t,memory_t,std::tuple<cache_storage_t,address_to_cache_loc_t,cache_loc_to_address_t,uint64_t>> read_data_from_cache(memory_t memory,address_t address,cache_t cache){
    address_to_cache_loc_t address_to_cache_loc;
    cache_loc_to_address_t cache_loc_to_address;
    cache_storage_t cache_storage;
    uint64_t data;
    uint64_t location_stack;
    uint64_t None=0;
    std::tie(cache_storage,address_to_cache_loc,cache_loc_to_address,location_stack)=cache;
    location_stack=evict_location(location_stack);

// If the data is not yet in the cache (read miss),fetch it from the DRAM
    if (address not in address_to_cache_loc ) {
// If the cache was full,evict first
        if (cache_is_full(location_stack_ptr) ) {
            std::tie(memory,cache)=write_to_mem_on_evict(memory,cache,address,data);
// Now read a cache line from memory
        }//12 > 8
        std::tie(memory,cache)=read_from_mem_on_miss(memory,cache,address);
// Get the data from the cache
    }//8 > 4
    data=fetch_cache_entry(cache,address);
    return std::make_tuple(data,memory,cache);


}
