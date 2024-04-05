# Initialise the cache
def init_cache():
# Cache of size CACHE_SZ, cache line = 64 bytes = 16 words
    cache_storage=[[0]*16]*(CACHE_SZ/16)
    location_stack_storage=range(0,CACHE_SZ/16)
    location_stack_ptr=(CACHE_SZ/16)-1
    last_used_loc = location_stack[location_stack_ptr]
    location_stack = (location_stack_storage,location_stack_ptr,last_used_loc)
    address_to_cache_loc={}
    cache_loc_to_address={}
    cache_lookup=(address_to_cache_loc,cache_loc_to_address)
    cache = (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack)
    return cache
    
# Some helper functions    
def get_next_free_location(location_stack):
    (location_stack_storage,location_stack_ptr,last_used_loc) = location_stack
    loc = location_stack_storage[location_stack_ptr] 
    location_stack_ptr-=1
    location_stack = (location_stack_storage,location_stack_ptr,last_used_loc)
    return  (location,location_stack)

def  evict_location(location_stack):
    (location_stack_storage,location_stack_ptr,last_used_loc) = location_stack
    location_stack_ptr+=1
    location_stack[location_stack_ptr] = last_used_loc
    location_stack = (location_stack_storage,location_stack_ptr,last_used_loc)
    return location_stack
    
def cache_is_full(location_stack_ptr):
    if location_stack_ptr==0:
        return True
    else:
        return False    

# If the cache was full, evict first
def write_to_mem_on_evict(memory,cache,address,data):
    (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack) = cache
    (location_stack_storage,location_stack_ptr,last_used_loc) = location_stack    
    if cache_is_full(location_stack_ptr):
        location_stack = evict_location(location_stack)
        evicted_address = cache_loc_to_address[last_used_loc]
        cache_line = cache_storage[last_used_loc]
        for i in range(0,16):
            cdata = cache_line[i]
            memory[(evicted_address<<4) + i]=cdata
    location_stack = (location_stack_storage,location_stack_ptr,last_used_loc)
    cache = (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack)
    return (memory,cache)

# and
# If the item was not yet in the cache (write miss):
def read_from_mem_on_miss(memory,cache,address):
    (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack) = cache
    (location_stack_storage,location_stack_ptr,last_used_loc) = location_stack    
    cache_line = []
# Get a free location. I am assuming that an eviction has been done before if necessary            
    (loc,location_stack) = get_next_free_location(location_stack)
# Get the DRAM content and write it to the cache storage    
    for i in range(0,16):
        cache_line.append( memory[((address>>4)<<4)+i])
    cache_storage[loc] = cache_line
# Update the lookup table and the last used location    
    address_to_cache_loc[address>>4]=loc
    cache_loc_to_address[loc] = address>>4
    last_used_loc=loc
    location_stack = (location_stack_storage,location_stack_ptr,last_used_loc)
    cache = (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack)
    return (memory, cache)

def update_cache_entry(cache,address,data):
    (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack) = cache
    (location_stack_storage,location_stack_ptr,last_used_loc) = location_stack    
    loc =  address_to_cache_loc[address>>4]
    cache_line = cache_storage[loc]
    cache_line[address & 0xF] = data
    cache_storage[loc] = cache_line
# Update the lookup table and the last used location    
    last_used_loc=loc
    location_stack = (location_stack_storage,location_stack_ptr,last_used_loc)
    cache = (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack)
    return (memory, cache)

    
def fetch_cache_entry(cache,address):    
    (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack) = cache
    (location_stack_storage,location_stack_ptr,last_used_loc) = location_stack    
    loc =  address_to_cache_loc[address>>4]
    cache_line = cache_storage[loc]
    data = cache_line[address & 0xF]
    return data 

def write_data_to_cache(memory,cache, address, data):
    (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack) = cache
    (location_stack_storage,location_stack_ptr,last_used_loc) = location_stack    
# If the cache was full, evict first
    if cache_is_full(location_stack_ptr):
        (memory, cache) = write_to_mem_on_evict(memory,cache,address,data)

# If the item was not yet in the cache (write miss):
    cache_line = []
    if address not in address_to_cache_loc:          
        (memory, cache) = read_from_mem_on_miss(memory,cache,address)
    else:
        # Find the cache location and cache line entry and update
        (memory,cache) = update_cache_entry(cache,address,data)
        loc =  address_to_cache_loc[address>>4]
    return (memory, cache)
    
def read_data_from_cache(memory, address, cache):
    (cache_storage, address_to_cache_loc,cache_loc_to_address, location_stack) = cache
    location_stack = evict_location(location_stack)
    
# If the data is not yet in the cache (read miss), fetch it from the DRAM
    if address not in address_to_cache_loc:                
    # If the cache was full, evict first
        if cache_is_full(location_stack_ptr):
            (memory, cache) = write_to_mem_on_evict(memory,cache,address,data)
        # Now read a cache line from memory
        (memory, cache) = read_from_mem_on_miss(memory,cache,address)
# Get the data from the cache
    data = fetch_cache_entry(cache,address)
    return (data,memory,cache)   


