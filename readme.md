# Advent Of Code
Collection of my solutions for the [Advent of Code](https://adventofcode.com) series.

## My Path
I discovered the **Advent of Code** in December 2021 and decided to give the puzzles a try.  I used c++ and found it  refreshing to work on small projects with no dependencies outside of the standard library.  I was able to progress through the first two weeks of 2021 rather efficiently and had a love/hate relationship with the challenges within the last week.  I was able to **complete 100% of 2021**, although I had to come back at a later time for Day 23 part 2.  I also decided to explore 2015, the first year, and was able to **complete 100% of 2015** also.  I have found this exercise very enjoyable and I look forward to exploring more years to come.  

## Current Progress
* 2022 =  6 stars
* 2021 = 50 stars (complete)
* 2016 = 20 stars 
* 2015 = 50 stars (complete)

## Goals
* **have fun**
* **explore c++ outside of embedded environment**
* **learn new techniques**

I enjoyed meeting these goals as I found the puzzles engaging, challenging, and satisfying to complete. I ended up  learning new techniques for competitive programming which has led to further research including the book ["Guide to Competitive Programming"](https://www.amazon.com/Guide-Competitive-Programming-Algorithms-Undergraduate/dp/3030393569/), by Antti Laaksonen, which I recommend.

## Source Organization
I was treating each day's puzzle as a separate project, so originally I gave them each a unique repository. I was really just doing that to locally track my changes and thoughts, and at the time was not planning to publish. I have since merged all of them into this single repository.

There is a directory for each year, which itself contains directories for each day. The format slightly changed over time, but I typically will generate both part 1 and part 2 solutions under the same source, although some of the early ones had separate folders. In some cases there is a defined macro to enable part 2, typically for cases that part 1 was time intensive, or that the logic needed isolated for whatever reason.

<p style="text-align: center;">general layout</p>

    AoC/<year>/<day>/
    - main.cpp = source code
    - input.txt = input to process when ./main is executed
    - input-full.txt = copied to input.txt as needed
    - input-sample.txt = copied to input.txt as needed
    - out.txt = output report including answers and timestamp

<br/>

# Days worth mentioning
## [2021 Day 23 - Amphipod](https://adventofcode.com/2021/day/23)
**objective**: move pieces from rooms into a sorted order with minimal points, different pieces cause more points to move.
* completed as of 1/26/22! ~~only remaining unsolved part of 2021~~
* completed part 1 with brute force
* part 2 added more depth and pieces, so brute force is no longer feasible as execution time would exceed weeks
* I used [Dijkstra’s algorithm](https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm).  Every new board layout is considered node!!!  I was hung up for awhile trying to use each individual piece as a node.  Once this made sense to me, it was rather straightforward to implement.
* originally part 2 solution was 8s with the basic algorithm.
* improved to 400mS for both part 1 and part 2 running consecutive.

Biggest time gains were
* simplify the Board class to just a single string as a member.  Especially helpful since it is used for key lookup.
* Initially calculate the base-energy, which is the energy used if each piece could go straight home.  Then only track delta-energy against this direct path.
* only add delta-energy when **entering hall**, and account for both the steps for room->hall and for  hall->final-room, since they are constant and both will be implemented for this intermediate position.

## [2021 Day 19 - Beacon Scanner](https://adventofcode.com/2021/day/19)
**objective**: align various scanner/beacons within a 3D space.  Each scanner can have it's orientation out of alignment in 90 degrees per axis, which yields 24 unique orientations.  Using the various unaligned scanner reports you need to find the overlapping beacons and map them all to an absolute origin (scanner 1).
* first puzzle that took me multiple days to find solution
* was thrown off by the 24 orientations, as originally I was having a hard time visualizing this
* I had a defect that needed isolated which caused sample input to pass yet the full input to fail...
<pre><code> // simplification of defect
    uint64_t val;
    val = 1U << 37;  // error, U is only 32 bits, so 1 is shifted out 
    val = 1UL << 37; // correct, x is valid up to value 63
</code></pre>

## [2021 Day 6 - Lanternfish](https://adventofcode.com/2021/day/6)
**objective**: calculate the population of lanternfish when given the spawning cycle, days elapsed, and the starting quantity\state of fish.
* part 1 used brute force in under 1 min
* part 2 brute force was over 10 mins.
* modified algorithm and got part 2 down to 3mS!
* Sparked my interest and appreciation for competitive programming

## [2021 Day 8 - Seven Segment Search](https://adventofcode.com/2021/day/8)
**objective**: groups of 7 segment displays have shuffled wiring.  For each group you are given the display indication of the digits '0' through '9'.  You must identify the correct wiring and then decipher a code.
* virtual hardware, fun!
* engaging to identify a method to identify correct wiring

## [2015 Day 22 - Wizard Simulator 20XX](https://adventofcode.com/2015/day/22)
**objective**: RPG battle simulation for a wizard against a boss.  Identify the least amount of mana needed to win, given the starting status for the wizard and boss.
* enjoyed creating my own mini RPG
* needed some time troubleshooting the battle sequence
* it continued from the previous day ([21](https://adventofcode.com/2015/day/22)), which was an RPG simulator with equipment variations

<br/>

# Closing Thoughts
I really enjoy working through these puzzles and exploring competitive programming.  I plan to continue researching efficient methods and test my skills in upcoming years.

Thanks to Eric Wastl and everyone who supports the Advent of Code!
 