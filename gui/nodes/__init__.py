from .base_node import BaseNode

# Simple factory mapping
node_registry = {
    "Gravity": lambda: BaseNode("Gravity", inputs=["g"], outputs=["a"]),
    "Integrator": lambda: BaseNode("Integrator", inputs=["a"], outputs=["v"]),
    "EOS": lambda: BaseNode("EOS", inputs=["rho", "u"], outputs=["P"]),
}
