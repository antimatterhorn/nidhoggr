def MieGruneisenMaterial(name: str):
    name = name.lower()
    if name == "granite":
        return dict(
            rho0 = 2.64,        # g/cm³
            C0 = 3.68e5,        # cm/s
            Gamma0 = 1.5,
            S = 1.34
        )
    elif name == "basalt":
        return dict(
            rho0 = 2.8,
            C0 = 1.45e5,
            Gamma0 = 1.2,
            S = 1.49
        )
    elif name == "iron":
        return dict(
            rho0 = 7.86,
            C0 = 3.93e5,
            Gamma0 = 2.0,
            S = 1.65
        )
    elif name == "aluminum":
        return dict(
            rho0 = 2.70,
            C0 = 5.32e5,
            Gamma0 = 2.0,
            S = 1.34
        )
    elif name == "copper":
        return dict(
            rho0 = 8.93,
            C0 = 3.94e5,
            Gamma0 = 2.0,
            S = 1.489
        )
    elif name == "lead":
        return dict(
            rho0 = 11.34,
            C0 = 2.16e5,
            Gamma0 = 2.5,
            S = 1.49
        )
    else:
        raise ValueError(f"Unknown Mie–Grüneisen material: {name}")
