For each i or n, number starts at 0

# Formatting of network_config.txt
Network Config:

IntegrateFireType: (number)\
IntegrateFireThreshold: (number, use 0 if not necessary)\
Layers: (number)\
Inputs: (number)\
FullConfigure: (Yes or No)

### FullConfigure: Yes
Layer i (i starts at 0):\
Neuron n inputs: (number), threshold: (number)\
Neuron n+1 inputs: (number), threshold: (number)\
End

Layer i+1:\
Neuron n inputs: (number), threshold: (number)\
Neuron n+1 inputs: (number), threshold: (number)\
End

### FullConfigure: No
Layer i (i starts at 0):\
Neurons: (number)\
Threshold: (number)\
Type: (0 or 1)\
End

Layer i+1:\
Neurons: (number)\
Threshold: (number)\
Type: (0 or 1)\
End

# Formatting of stdp_config.txt
STDP Config:

C-TNN:\
Capture: (number)\
Backoff: (number)\
Search: (number)

R-TNN:\
RewardPotentiationDepression: (number)\
RewardWindow: (number)\
PunishmentPotentiationDepression: (number)\
PunishmentWindow: (number)

# Formatting of mapping_config.txt
Mapping Config:

FullyConnected: Yes

### FullyConnected: Yes (best used with FullConfigure: No)
Input Map:\
Inputs: (number)\
Layer 0 Neurons: (number)\
DefaultWeight: (number) (optional, remove if not necessary)\
End

Layer Map i (i starts at 0):\
Layer i Neurons: (number)\
Layer i+1 Neurons: (number)\
End

### FullyConnected: No (best used with FullConfigure: Yes)
Input Map:\
Input: 0, Network: 0, Weight: 4 (adding "Weight:" is optional, remove if not necessary)\
Input: 1, Network: 1\
Input: 2, Network: 2\
Input: 3, Network: 3, Weight: 2\
Input: 0, Network: 4, Weight: 4\
Input: 1, Network: 5\
Input: 2, Network: 6\
Input: 3, Network: 7, Weight: 2\
End

Layer Map 0:\
Layer 0: 0, Layer 1: 0, Weight: 4\
Layer 0: 1, Layer 1: 1, Weight: 4\
End

(this is just an example, use any numbers you like)

# Formatting of spike_config.txt
Spike Config:

Spikes:\
Input: (number), Time: (number)\
Input: (number), Time: (number)\
...\
Input: (number), Time: (number)\
Input: (number), Time: (number)\
End
