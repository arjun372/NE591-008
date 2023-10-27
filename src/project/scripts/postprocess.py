import json

# Load JSON data from a file
with open('data.json', 'r') as f:
    data = json.load(f)

# Access data
inputs = data['inputs']
outputs = data['outputs']

# Accessing nested data
D = inputs['D']
cross_section = inputs['cross-section']
dimensions = inputs['dimensions']
mesh = inputs['mesh']
methods = inputs['methods']

# Accessing data from outputs
for key, value in outputs.items():
    converged = value['converged']
    iterations = value['iterations']
    iterative_error = value['iterative-error']
    max_residual = value['max-residual']
    residual = value['residual']
    solution = value['solution']
    wall_time_ns = value['wall-time-ns']
