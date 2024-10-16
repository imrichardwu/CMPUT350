#!/bin/bash

# Make sure to give execution permission:
# chmod +x run_experiments.sh

# Define arrays for unit counts and attack policies
unit_counts=(20 50 100 300)
policies=(0 1 2)  # 0: ATTACK_WEAKEST, 1: ATTACK_CLOSEST, 2: ATTACK_MOST_DANGEROUS

# Number of runs per scenario for statistical significance
runs=100

# Output file to store the results
output_file="ExperimentResults.csv"

# Write the header to the output file
echo "UnitCount,RedPolicy,BluePolicy,Seed,Result" > $output_file

# Loop over unit counts
for units in "${unit_counts[@]}"; do
    # Loop over Red team policies
    for red_pol in "${policies[@]}"; do
        # Loop over Blue team policies
        for blue_pol in "${policies[@]}"; do
            # Run simulations for the specified number of runs
            for ((i=1; i<=runs; i++)); do
                # Generate a random seed
                seed=$RANDOM

                # Run the simulator and capture the output
                result=$(./simul 700 700 0 $seed $units $units $red_pol $blue_pol 1 0 | grep "game over")

                # Parse the result
                if echo "$result" | grep -q "RED wins"; then
                    outcome="RedWin"
                elif echo "$result" | grep -q "BLUE wins"; then
                    outcome="BlueWin"
                elif echo "$result" | grep -q "draw"; then
                    outcome="Draw"
                else
                    outcome="Unknown"
                fi

                # Write the result to the output file
                echo "$units,$red_pol,$blue_pol,$seed,$outcome" >> $output_file

                # Optional: Print progress to the console
                echo "Units: $units, RedPolicy: $red_pol, BluePolicy: $blue_pol, Run: $i/$runs, Outcome: $outcome"
            done
        done
    done
done
