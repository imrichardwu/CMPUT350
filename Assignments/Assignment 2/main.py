import pandas as pd

# Load the data
df = pd.read_csv('ExperimentResults.csv')

# Map policy codes to names
policy_names = {0: 'ATTACK_WEAKEST', 1: 'ATTACK_CLOSEST', 2: 'ATTACK_MOST_DANGEROUS'}
df['RedPolicyName'] = df['RedPolicy'].map(policy_names)
df['BluePolicyName'] = df['BluePolicy'].map(policy_names)

# Group by UnitCount, RedPolicyName, BluePolicyName and Result
grouped = df.groupby(['UnitCount', 'RedPolicyName', 'BluePolicyName', 'Result']).size().reset_index(name='Count')

# Calculate total runs per scenario
total_runs = df.groupby(['UnitCount', 'RedPolicyName', 'BluePolicyName']).size().reset_index(name='TotalRuns')

# Merge and calculate win percentages
merged = pd.merge(grouped, total_runs, on=['UnitCount', 'RedPolicyName', 'BluePolicyName'])
merged['Percentage'] = (merged['Count'] / merged['TotalRuns']) * 100

# Display all results
print(merged.to_string(index=False))