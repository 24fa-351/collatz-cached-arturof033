: '
sh collatzTimeTest.sh VALUES SMALLEST LARGEST CACHE_SIZE METHOD

VALUES = amount of random values to test
SMALLEST = lower range limit of random values to choose from
LARGEST = upper range limit of random value to choose from
CACHE_SIZE = size of cache for LRU or LFU
METHOD = choosing between "LRU", "LFU", or "none"

Passes outputs to collatz function, prints results of collatz function in file 'collatzCacheResult.csv'
    and prints time spend, mean time spent per collatz, and collatz bounds in file 'collatzCacheTIme.csv'.
'


# starts counter for total time
start_ms=$(ruby -e 'puts (Time.now.to_f * 1000).to_i')

# takes in command line arguments for collatz function
values=$1
smallest=$2
largest=$3
cache_size=$4
method=$5

gcc collatz.c collatzBase.c cacheLRU.c cacheLFU.c -o collatz

# outputs list of collatz iterations and time spent on the program
time_output=$( { time ./collatz "$values" "$smallest" "$largest" "$cache_size" "$method" 2>&1; } 2>&1)

# extracts collatz list from time_output
#collatz_list=$(./collatz "$values" "$smallest" "$largest" "$cache_size" "$method" )
collatz_list=$(echo "$time_output" | grep '' | awk '{l[NR] = $0} END {for (i=1; i<=NR-7; i++) print l[i]}')

#prints collatz list in two columns and places in file 'collatzResult.csv'
printf "random_value, collatz_counter\n" > collatzCacheResult.csv
printf "$collatz_list\n" >> collatzCacheResult.csv

# extract time from time_output 
    # extracts minutes and seconds -> converts to seconds
real_time=$(echo "$time_output" | grep 'real' | awk -F '[m|s]' '{print ($1 * 60) + $2}') 
user_time=$(echo "$time_output" | grep -w 'user' | awk -F '[r|m|s]' '{print ($3 * 60) + $4}')    
sys_time=$(echo "$time_output" | grep -w 'sys' | awk -F '[s|m|s]' '{print ($3 * 60) + $4}')

# extracts hit and miss variables
hit=$(echo "$time_output" | grep 'hit:' | awk '{print $2}') 
miss=$(echo "$time_output" | grep 'miss:' | awk '{print $2}')
hit_ratio=$(echo "$time_output" | grep 'ratio:' | awk '{print $3}')

# print time complexity  
printf "method, smallest, largest, iterations, cache_size, user_time, sys_time, real_time, hit, miss, hit_ratio, total_time, iteration_mean\n" > collatzCacheTime.csv
printf "$method, $smallest, $largest, $values, $cache_size, $real_time, $user_time, $sys_time, $hit, $miss, $hit_ratio, " > collatzCacheTime.csv

# calculates the total run time of the program
end_ms=$(ruby -e 'puts (Time.now.to_f * 1000).to_i')
elapsed_ms=$((end_ms - start_ms))
total_time=$(echo "scale=5;($elapsed_ms/1000)" | bc)
iteration_mean=$(echo "scale=5;($total_time/$values)" |bc)

# prints total time spend on the tasks
printf "$total_time, $iteration_mean\n" >> collatzCacheTime.csv
